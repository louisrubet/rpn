#ifndef __stack_h__
#define __stack_h__

#include <string.h>
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
        _base_pointer = NULL;
        _total_size = 0;
        _total_count_pointer = 0;
        erase();
    }

    virtual ~stack()
    {
        if (_base != NULL)
            free(_base);
        if (_base_pointer != NULL)
            free(_base_pointer);
    }

    void erase()
    {
        _current = _base;
        _count = 0;
    }

    //
    static void copy_and_push_back(stack& from, unsigned int index_from, stack& to)
    {
        // copy a whole stack entry and push it back to another stack
        object* allocated = to.allocate_back(from.seq_len(index_from), from.seq_type(index_from));
        memcpy(allocated, from.seq_obj(index_from), from.seq_len(index_from));

        if (allocated->_type == cmd_number)
            ((number*)allocated)->_value.set_significand(((number*)allocated) + 1);
    }

    //
    static void copy_and_push_back(object* from, stack& to, unsigned int size)
    {
        // copy a whole stack entry and push it back to another stack
        object* allocated = to.allocate_back(size, from->_type);
        memcpy(allocated, from, size);

        if (allocated->_type == cmd_number)
            ((number*)allocated)->_value.set_significand(((number*)allocated) + 1);
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
            
            //TODO si realloc alors les pointeurs doivent etre recalculés !
        }
        if ((_count+1) > _total_count_pointer)
        {
            _base_pointer = (object**)realloc(_base_pointer, (_total_count_pointer * sizeof(object*)) + ALLOC_STACK_CHUNK);
            _total_count_pointer += (ALLOC_STACK_CHUNK / sizeof(object));
        }

        // manage stack itself
        _base_pointer[_count++]=(object*)_current;
        allocated = (object*)_current;
        _current += size;

        // init object
        allocated->_type = type;
        allocated->_size = size;
        if (type == cmd_number)
            ((number*)allocated)->_value.init(((number*)allocated) + 1);

        return allocated;
    }

    object* pop_back()
    {
        object* back = NULL;

        if (_count > 0)
        {
            _current = (char*)_base_pointer[_count - 1];
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
            return _base_pointer[_count - index - 1];
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
            return _base_pointer[_count - 1];
        else
            return NULL;
    }

    unsigned int get_len(unsigned int index)
    {
        if (index<_count)
            return _base_pointer[_count - index - 1]->_size;
        else
            return 0;
    }

    cmd_type_t get_type(unsigned int index)
    {
        if (index<_count)
            return _base_pointer[_count - index - 1]->_type;
        else
            return cmd_undef;
    }

    // sequential access (index is counted from front)
    object* seq_obj(unsigned int index)
    {
        if (index<_count)
            return _base_pointer[index];
        else
            return NULL;
    }

    unsigned int seq_len(unsigned int index)
    {
        if (index<_count)
            return _base_pointer[index]->_size;
        else
            return 0;
    }

    cmd_type_t seq_type(unsigned int index)
    {
        if (index<_count)
            return _base_pointer[index]->_type;
        else
            return cmd_undef;
    }

private:
    char* _base;
    char* _current;
    object** _base_pointer;

    unsigned int _count;// =_vlen.size()=_vpointer.size()
    unsigned int _total_count_pointer;//total number of possible pointers
    unsigned int _total_size;//total allocated size in bytes
};

//
class heap : public stack
{
public:
    heap() { }
    virtual ~heap() { }

    object* add(const string name, object* obj, unsigned int size)
    {        
        object* local = _map[name];

        // variable does not exist in heap or already exists but its size is too short -> allocate
        if (local==NULL || (local!=NULL && size>local->_size))
        {
            copy_and_push_back(obj, *this, size);
            _map[name] = back();
        }
        else
        {
            // variable already exists in heap but previous was larger -> don't reallocate
            // copy a whole stack entry and push it back to another stack
            memcpy(local, obj, size);
            if (local->_type == cmd_number)
                ((number*)local)->_value.set_significand(((number*)local)+1);
        }

        return local;
    }
    
    bool get(const string name, object*& obj, unsigned int& size)
    {
        bool ret = false;
        map<string, object*>::iterator i = _map.find(name);

        if (i!=_map.end() && i->second!=NULL)
        {
            obj = i->second;
            size = i->second->_size;
            ret = true;
        }
        return ret;
    }

    bool replace_value(const string name, object* obj, unsigned int size)
    {
        bool ret=false;
        map<string, object*>::iterator i = _map.find(name);

        if (i!=_map.end() && i->second!=NULL && size==i->second->_size)
        {
            (void)memcpy(i->second, obj, size);
            if (i->second->_type == cmd_number)
                ((number*)i->second)->_value.set_significand(((number*)i->second)+1);
            ret = true;
        }
    }

    bool exist(const string name)
    {
        return (_map.find(name) != _map.end());
    }

    bool get_by_index(int num, string& name, object*& obj, unsigned int& size)
    {
        if (num>=0 && num<(int)_map.size())
        {
            object* local;
            map<string, object*>::iterator i= _map.begin();

            //TODO another formulation, please
            for(int j=0;j<num;j++)
                i++;

            local = (object*)i->second;
            name = i->first;
            obj = local;
            size = local->_size;
            return true;
        }
        else
            return false;
    }

    bool erase(const string& name)
    {
        map<string, object*>::iterator i = _map.find(name);
        bool ret = false;
        
        if (i != _map.end())
        {
            _map.erase(i->first);
            ret = true;
        }
        return ret;
    }

    unsigned int size() { return _map.size(); }

private:
    map<string, object*> _map;
};

#endif // __stack_h__
