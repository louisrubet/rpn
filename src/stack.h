#ifndef __stack_h__
#define __stack_h__

#include <string.h>

#include <assert.h>
#include <vector>
#include <map>
using namespace std;

// allocation base size
#define ALLOC_STACK_CHUNK (64*1024)

//
class stack
{
public:
    stack()
    {
        _base = NULL;
        _total_size = 0;
        erase();
    }

    virtual ~stack()
    {
        if (_base != NULL)
            free(_base);
    }

    void erase()
    {
        _current = _base;
        _vpointer.clear();
        _vlen.clear();
        _count = 0;
    }

    //
    static void copy_and_push_back(stack& from, unsigned int index_from, stack& to)
    {
        // copy a whole stack entry and push it back to another stack
        object* allocated = to.allocate_back(from.seq_len(index_from), from.seq_type(index_from));
        memcpy(allocated, from.seq_obj(index_from), from.seq_len(index_from));

        if (allocated->_type == cmd_number)
            ((number*)allocated)->_value.set_significand(((number*)allocated)+1);
    }

    //
    static void copy_and_push_back(object* from, stack& to, unsigned int size)
    {
        // copy a whole stack entry and push it back to another stack
        object* allocated = to.allocate_back(size, from->_type);
        memcpy(allocated, from, size);

        if (allocated->_type == cmd_number)
            ((number*)allocated)->_value.set_significand(((number*)allocated)+1);
    }
    
    object* allocate_back(unsigned int size, cmd_type_t type)
    {
        object* allocated;

        // manage memory allocation
        if (_current + size > _base + _total_size)
        {
            unsigned long offset = _current - _base;
            _total_size += ALLOC_STACK_CHUNK;
            _base = (char*)realloc(_base, _total_size);
            _current = _base + offset;
        }

        // manage stack itself
        _vlen.push_back(size);
        _vpointer.push_back((object*)_current);
        allocated = (object*)_current;
        _current += size;
        _count++;

        // init object
        allocated->_type = type;
        allocated->_size = size;
        if (type == cmd_number)
            ((number*)allocated)->_value.init(((number*)allocated)+1);

        return allocated;
    }

    object* pop_back()
    {
        object* back = NULL;

        if (_count > 0)
        {
            _current = (char*)_vpointer[_count - 1];
            _vlen.pop_back();
            _vpointer.pop_back();

            _count--;

            back = (object*)_current;
        }

        return back;
    }

    unsigned int size()
    {
        return _count;
    }

    // stack access (index is counted from back)
    object* get_obj(unsigned int index)
    {
        if (index<_count)
            return _vpointer[_count-index-1];
        else
            return NULL;
    }

    object* operator[](unsigned int index)
    {
        return get_obj(index);
    }
    
    object* back()
    {
        if (_count>0)
            return _vpointer[_count-1];
        else
            return NULL;
    }

    unsigned int get_len(unsigned int index)
    {
        if (index<_count)
            return _vlen[_count-index-1];
        else
            return 0;
    }

    cmd_type_t get_type(unsigned int index)
    {
        if (index<_count)
            return _vpointer[_count-index-1]->_type;
        else
            return cmd_undef;
    }

    // sequential access (index is counted from front)
    object* seq_obj(unsigned int index)
    {
        if (index<_count)
            return _vpointer[index];
        else
            return NULL;
    }

    unsigned int seq_len(unsigned int index)
    {
        if (index<_count)
            return _vlen[index];
        else
            return 0;
    }

    cmd_type_t seq_type(unsigned int index)
    {
        if (index<_count)
            return _vpointer[index]->_type;
        else
            return cmd_undef;
    }

private:
    char* _base;
    char* _current;

    vector<object*> _vpointer;//pointer on each entry
    vector<unsigned int> _vlen;// size of each entry in bytes
    unsigned int _count;// =_vlen.size()=_vpointer.size()
    unsigned int _total_size;//total allocated size in bytes
};

//
class heap
{
private:
    struct local_var
    {
        unsigned int length;
        object obj[0];
    };

public:
    heap() { }
    virtual ~heap()
    {
        for(map<string, struct local_var*>::iterator i=_map.begin(); i!=_map.end(); i++)
            free(i->second);
    }

    object* add(const string name, object* obj, unsigned int size, int type = 0)
    {
        struct local_var* local = _map[name];
        if (local == NULL)
        {
            //TODO gerer les pbs de memoire
            local = (struct local_var*)malloc(size + sizeof(local_var));
            _map[name] = local;
        }
        else if (size != local->length)
        {
            //TODO gerer les pbs de memoire
            local = (struct local_var*)realloc(local, size + sizeof(local_var));
            _map[name] = local;
        }
        local->length = size;

        if (obj != NULL)
        {
            memcpy(local->obj, obj, size);
            if (local->obj->_type == cmd_number)
                ((number*)local->obj)->_value.set_significand(((number*)local->obj)+1);
        }
        
        return local->obj;
    }
    
    bool get(const string name, object*& obj, unsigned int& size)
    {
        map<string, struct local_var*>::iterator i = _map.find(name);
        if (i != _map.end())
        {
            if (i->second != NULL)
            {
                obj = i->second->obj;
                size = i->second->length;
            }
            return true;
        }
        else
            return false;
    }

    bool replace_value(const string name, object* obj, unsigned int size)
    {
        bool ret=false;
        map<string, struct local_var*>::iterator i = _map.find(name);

        if (i != _map.end())
        {
            if (i->second != NULL && size==i->second->length)
            {
                (void)memcpy(i->second->obj, obj, size);
                if (i->second->obj->_type == cmd_number)
                    ((number*)i->second->obj)->_value.set_significand(((number*)i->second->obj)+1);
            }
            return true;
        }
        return ret;
    }

    bool exist(const string name)
    {
        return (_map.find(name) != _map.end());
    }

    bool get_by_index(int num, string& name, object*& obj, unsigned int& size)
    {
        if (num>=0 && num<(int)_map.size())
        {
            struct local_var* local;
            map<string, struct local_var*>::iterator i=_map.begin();

            //TODO moche moche moche
            for(int j=0;j<num;j++)
                i++;

            local = (struct local_var*)i->second;
            assert(local != NULL);

            name = i->first;
            obj = local->obj;
            size = local->length;
            return true;
        }
        else
            return false;
    }

    bool erase(const string& name)
    {
        map<string, struct local_var*>::iterator i = _map.find(name);
        if (i != _map.end())
        {
            free(i->second);
            _map.erase(i->first);
            return true;
        }
        return false;
    }

    unsigned int size() { return _map.size(); }

private:
    map<string, struct local_var*> _map;
};

#endif // __stack_h__
