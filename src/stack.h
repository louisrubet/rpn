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
#define LOCAL_COPY_PLACES 3
#define LOCAL_COPY_SIZE 128

//
class stack
{
private:
    struct local_copy
    {
        unsigned int length;
        int type;
        int blob;
    };

public:
    stack()
    {
        _base = (char*)malloc(ALLOC_STACK_CHUNK);
        _blob = (char*)malloc(ALLOC_BLOB_CHUNK);
        _total_size = ALLOC_STACK_CHUNK;
        _current = _base;
        _current_blob = _base;
        _count = 0;
    }   
    virtual ~stack() { free(_base); }

    void push_back(void* obj, unsigned int size, int type = 0, bool dont_copy = false)
    {
        if (_current + size > _base + _total_size)
        {
            //TODO gerer les pbs de memoire
            _total_size += ALLOC_STACK_CHUNK;
            _base = (char*)realloc(_base, _total_size);
        }

        if (!dont_copy)
            memcpy(_current, obj, size);
        _vlen.push_back(size);
        _vpointer.push_back(_current);
        _vlen_blob.push_back(0);
        _vtype.push_back(type);
        _count++;
        _current += size;
    }

    void* allocate_back(unsigned int size, int type, unsigned int blob_size = 0, void** blob = NULL)
    {
        void* allocated;

        if (_current + size > _base + _total_size)
        {
            //TODO gerer les pbs de memoire
            _total_size += ALLOC_STACK_CHUNK;
            _base = (char*)realloc(_base, _total_size);
        }
        if (_current_blob + blob_size > _blob + _total_blob_size)
        {
            //TODO gerer les pbs de memoire
            _total_blob_size += ALLOC_BLOB_CHUNK;
            _blob = (char*)realloc(_base, _total_blob_size);
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

    int seq_type(unsigned int index)
    {
        if (index<_count)
            return _vtype[index];
        else
            return 0;
    }

    // local objects copy
    void copy_obj_to_local(unsigned int index, unsigned int to_place)
    {
        assert(to_place < LOCAL_COPY_PLACES);
        struct local_copy* local = (struct local_copy*)_places[to_place];
        local->length = get_len(index);
        local->type= get_type(index);
        memcpy(&local->blob, get_obj(index), local->length);
    }

    void push_obj_from_local(unsigned int from_place)
    {
        assert(from_place < LOCAL_COPY_PLACES);
        struct local_copy* local = (struct local_copy*)_places[from_place];
        push_back(&local->blob, local->length, local->type);
    }

    void dump(void)
    {
        dump8((unsigned char*)_base, 0, (unsigned long)(_current - _base));
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

    char _places[LOCAL_COPY_PLACES][LOCAL_COPY_SIZE];
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
