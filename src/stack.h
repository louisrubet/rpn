#ifndef __stack_h__
#define __stack_h__

#include <string.h>

#include <assert.h>
#include <vector>
#include <map>
using namespace std;

#include "debug.h"

// allocation base size
#define ALLOC_STACK_CHUNK (64*1024)
#define ALLOC_BLOB_CHUNK (64*1024)

//
class stack
{
public:
    stack()
    {
        _base = NULL;
        _blob = NULL;
        _total_size = 0;
        _total_blob_size = 0;
        erase();
    }

    virtual ~stack()
    {
        if (_base != NULL)
            free(_base);
        if (_blob != NULL)
            free(_blob);
    }

    void erase()
    {
        _current = _base;
        _current_blob = _blob;
        _vpointer.clear();
        _vpointer_blob.clear();
        _vlen.clear();
        _vlen_blob.clear();
        _vtype.clear();
        _count = 0;
    }
    
    void push_back(void* obj, unsigned int size, int type = 0, bool dont_copy = false, void* blob = NULL, unsigned int blob_size = 0)
    {
        void* allocated_blob;
        void* allocated = allocate_back(size, type, blob_size, &allocated_blob);

        if (!dont_copy)
        {
            if (size > 0)
                memcpy(allocated, obj, size);
            if (blob != NULL && blob_size > 0)
                memcpy(allocated_blob, blob, blob_size);
        }
    }

    // index_from is counted from back, last entry is 0
    static void copy_and_push_back(stack& from, unsigned int index_from, stack& to)
    {
        // copy a whole stack entry (with blob) and push it back to another stack
        void* allocated_blob;
        void* allocated = to.allocate_back(from.seq_len(index_from), from.seq_type(index_from), from.seq_blob_size(index_from), &allocated_blob);

        memcpy(allocated, from.seq_obj(index_from), from.seq_len(index_from));
        memcpy(allocated_blob, from.seq_blob(index_from), from.seq_blob_size(index_from));
    }

    void* allocate_back(unsigned int size, int type, unsigned int blob_size = 0, void** blob = NULL)
    {
        void* allocated;

        if (_current + size > _base + _total_size)
        {
            unsigned long offset = _current - _base;
            _total_size += ALLOC_STACK_CHUNK;
            _base = (char*)realloc(_base, _total_size);
            _current = _base + offset;
        }
        if (_current_blob + blob_size > _blob + _total_blob_size)
        {
            unsigned long offset = _current_blob - _blob;
            _total_blob_size += ALLOC_BLOB_CHUNK;
            _blob = (char*)realloc(_blob, _total_blob_size);
            _current_blob = _blob + offset;
        }

        _vlen.push_back(size);
        _vpointer.push_back(_current);
        allocated = _current;
        _current += size;

        _vlen_blob.push_back(blob_size);
        _vpointer_blob.push_back(_current_blob);
        if (blob != NULL)
            *blob = _current_blob;
        _current_blob += blob_size;

        _vtype.push_back(type);
        _count++;

//cout<<__FUNCTION__<<": -> stack size is "<<_count<<endl;//lru
        return allocated;
    }

    void* pop_back()
    {
        void* back = NULL;

        if (_count > 0)
        {
            _current = _vpointer[_count - 1];
            _vlen.pop_back();
            _vpointer.pop_back();
            
            _current_blob = _vpointer_blob[_count - 1];
            _vlen_blob.pop_back();
            _vpointer_blob.pop_back();

            _vtype.pop_back();
            _count--;

            back = _current;
        }

        return back;
    }

    unsigned int size()
    {
        return _count;
    }

    // stack access (index is counted from back)
    void* get_obj(unsigned int index)
    {
        if (index<_count)
            return _vpointer[_count-index-1];
        else
            return NULL;
    }

    void* get_blob(unsigned int index)
    {
        if (index<_count)
            return _vpointer_blob[_count-index-1];
        else
            return NULL;
    }

    void* operator[](unsigned int index)
    {
        return get_obj(index);
    }
    
    void* back()
    {
        if (_count>0)
            return _vpointer[_count-1];
        else
            return NULL;
    }

    void* back_blob()
    {
        if (_count>0)
            return _vpointer_blob[_count-1];
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

    int get_type(unsigned int index)
    {
        if (index<_count)
            return _vtype[_count-index-1];
        else
            return 0;
    }

    // sequential access (index is counted from front)
    void* seq_obj(unsigned int index)
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

    void* seq_blob(unsigned int index)
    {
        if (index<_count)
            return _vpointer_blob[index];
        else
            return NULL;
    }

    unsigned int seq_blob_size(unsigned int index)
    {
        if (index<_count)
            return _vlen_blob[index];
        else
            return 0;
    }

    int seq_type(unsigned int index)
    {
        if (index<_count)
            return _vtype[index];
        else
            return 0;
    }

private:
    char* _base;
    char* _blob;
    char* _current;
    char* _current_blob;

    vector<char*> _vpointer;//pointer on each entry
    vector<char*> _vpointer_blob;//pointer on each entry blob
    vector<unsigned int> _vlen;// size of each entry in bytes
    vector<unsigned int> _vlen_blob;// size of each blob entry in bytes
    vector<int> _vtype;//type of each entry
    unsigned int _count;// =_vlen.size()=_vpointer.size()=_vtype.size()
    unsigned int _total_size;//total allocated size in bytes
    unsigned int _total_blob_size;//total allocated blob size in bytes
};

//
class heap
{
private:
    struct local_var
    {
        unsigned int length;
        int type;
        int blob;
    };

public:
    heap() { }
    virtual ~heap()
    {
        for(map<string, struct local_var*>::iterator i=_map.begin(); i!=_map.end(); i++)
            free(i->second);
    }

    void* add(const string name, void* obj, unsigned int size, int type = 0)
    {
        struct local_var* blob = _map[name];
        if (blob == NULL)
        {
            //TODO gerer les pbs de memoire
            blob = (struct local_var*)malloc(size + sizeof(local_var));
            _map[name] = blob;
        }
        else if (size != blob->length)
        {
            //TODO gerer les pbs de memoire
            blob = (struct local_var*)realloc(blob, size + sizeof(local_var));
            _map[name] = blob;
        }
        blob->length = size;
        blob->type= type;
        
        if (obj != NULL)
            memcpy(&blob->blob, obj, size);

        return (void*)&blob->blob;
    }
    
    bool get(const string name, void*& obj, unsigned int& size, int& type)
    {
        map<string, struct local_var*>::iterator i = _map.find(name);
        if (i != _map.end())
        {
            if (i->second != NULL)
            {
                obj = &i->second->blob;
                size = i->second->length;
                type = i->second->type;
            }
            return true;
        }
        else
            return false;
    }

    bool exist(const string name)
    {
        return (_map.find(name) != _map.end());
    }

    bool get_by_index(int num, string& name, void*& obj, unsigned int& size, int& type)
    {
        if (num>=0 && num<(int)_map.size())
        {
            struct local_var* blob;
            map<string, struct local_var*>::iterator i=_map.begin();

            //TODO moche moche moche
            for(int j=0;j<num;j++)
                i++;

            blob = (struct local_var*)i->second;
            assert(blob != NULL);

            name = i->first;
            obj = &blob->blob;
            size = blob->length;
            type = blob->type;
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
