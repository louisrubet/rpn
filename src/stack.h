#ifndef __stack_h__
#define __stack_h__

#include <assert.h>
#include <vector>
#include <map>
using namespace std;

#define ALLOC_BLOB (128*1024)
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
		_base = (char*)malloc(ALLOC_BLOB);
		_total_size = ALLOC_BLOB;
		_current = _base;
		_count = 0;
	}	
	virtual ~stack() { free(_base); }

	void push_back(void* obj, unsigned int size, int type = 0)
	{
		if (_current + size > _base + _total_size)
		{
			//TODO gérer les pbs de mémoire
			_total_size += ALLOC_BLOB;
			_base = (char*)realloc(_base, _total_size);
		}

		memcpy(_current, obj, size);
		_vlen.push_back(size);
		_vpointer.push_back(_current);
		_vtype.push_back(type);
		_count++;
		_current += size;
	}

	void pop_back()
	{
		if (_count > 0)
		{
			_current = _vpointer[_count - 1];
			_vlen.pop_back();
			_vpointer.pop_back();
			_vtype.pop_back();
			_count--;
		}
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

private:
	char* _base;
	char* _current;
	vector<unsigned int> _vlen;// size of each entry in bytes
	vector<char*> _vpointer;//pointer on each entry
	vector<int> _vtype;//type of each entry
	unsigned int _count;// =_vlen.size()=_vpointer.size()=_vtype.size()
	unsigned int _total_size;//total allocated size in bytes

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

	void add(const string name, void* obj, unsigned int size, int type = 0)
	{
		struct local_var* blob = _map[name];
		if (blob == NULL)
		{
			//TODO gérer les pbs de mémoire
			blob = (struct local_var*)malloc(size + sizeof(local_var));
			_map[name] = blob;
		}
		else if (size != blob->length)
		{
			//TODO gérer les pbs de mémoire
			blob = (struct local_var*)realloc(blob, size + sizeof(local_var));
			_map[name] = blob;
		}
		blob->length = size;
		blob->type= type;
		memcpy(&blob->blob, obj, size);
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

	bool erase(const string name)
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
