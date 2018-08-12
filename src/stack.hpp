#ifndef __stack_h__
#define __stack_h__

#include <string.h>
#include <map>
using namespace std;

// allocation base size
#define ALLOC_STACK_CHUNK (64 * 1024)

/// @brief stack object, parens of program, storing execution stack values or programs
///
class stack {
   public:
    stack() {
        _base = NULL;
        _base_pointer = NULL;
        _total_size = 0;
        _total_count_pointer = 0;
        erase();
    }

    virtual ~stack() {
        if (_base != NULL) free(_base);
        if (_base_pointer != NULL) free(_base_pointer);
    }

    /// @brief remove all the stack elements
    ///
    void erase() {
        _current = _base;
        _count = 0;
    }

    /// @brief copy a whole stack entry and push it back to another stack
    ///
    /// @param from copy from
    /// @param index_from index t ocopy from
    /// @param to copy to
    ///
    static void copy_and_push_back(stack& from, unsigned int index_from, stack& to) {
        object* allocated = to.allocate_back(from.seq_len(index_from), from.seq_type(index_from));
        memcpy(allocated, from.seq_obj(index_from), from.seq_len(index_from));

        if (allocated->_type == cmd_number)
            ((number*)allocated)->move();
        else if (allocated->_type == cmd_complex)
            ((complex*)allocated)->move();
    }

    /// @brief copy a whole stack entry and push it back to another stack
    ///
    /// @param from copy from
    /// @param index_from index t ocopy from
    /// @param to copy to
    ///
    static void copy_and_push_back(object* from, stack& to, unsigned int size) {
        object* allocated = to.allocate_back(size, from->_type);
        memcpy(allocated, from, size);

        if (allocated->_type == cmd_number)
            ((number*)allocated)->move();
        else if (allocated->_type == cmd_complex)
            ((complex*)allocated)->move();
    }

    /// @brief allocate one object back on an already populated (or not) stack
    /// the object function move is called on every reallocated object on the stack
    /// the object function init is called on the new allocated object if its type is cmd_number or cmd_complex
    ///
    /// @param size the object size in bytes
    /// @param type the object type
    /// @return object* the allocated object
    ///
    object* allocate_back(unsigned int size, cmd_type_t type) {
        object* allocated;
        bool data_is_reallocated = false;
        char* old_base;

        // manage data memory allocation (add as much as memory it is needed)
        if (((_current - _base) + size) > _total_size) {
            // calc nb of needed pages
            unsigned long page_number = 1 + ((_current - _base) + size - _total_size) / ALLOC_STACK_CHUNK;
            _total_size += page_number * ALLOC_STACK_CHUNK;

            old_base = _base;
            _base = (char*)realloc(_base, _total_size);

            _current = _base + (_current - old_base);
            data_is_reallocated = true;
        }

        // manage pointers memory allocation (add one page if needed)
        if ((_count + 1) > _total_count_pointer) {
            _base_pointer =
                (object**)realloc(_base_pointer, (_total_count_pointer * sizeof(object*)) + ALLOC_STACK_CHUNK);
            _total_count_pointer += (ALLOC_STACK_CHUNK / sizeof(object));
        }

        // recalc object pointers in case of base reallocation
        if (data_is_reallocated) {
            for (int i = 0; i < _count; i++) {
                _base_pointer[i] = (object*)(_base + ((char*)_base_pointer[i] - old_base));
                if (_base_pointer[i]->_type == cmd_number)
                    ((number*)_base_pointer[i])->move();
                else if (_base_pointer[i]->_type == cmd_complex)
                    ((complex*)_base_pointer[i])->move();
            }
        }

        // manage stack itself
        _base_pointer[_count++] = (object*)_current;
        allocated = (object*)_current;
        _current += size;

        // init object
        allocated->_type = type;
        allocated->_size = size;
        if (type == cmd_number)
            ((number*)allocated)->init();
        else if (type == cmd_complex)
            ((complex*)allocated)->init();

        return allocated;
    }

    object* pop_back(int pop_count = 1) {
        object* back = NULL;

        // pop several entries, return the last
        while (pop_count-- > 0) {
            if (_count > 0) {
                _current = (char*)_base_pointer[--_count];
                back = (object*)_current;
            }
        }

        return back;
    }

    /// @brief the number of objects on stack
    ///
    /// @return unsigned int
    ///
    unsigned int size() { return _count; }

    /// @brief stack access (stack_level=0=first out)
    ///
    /// @param stack_level the object stack level
    /// @return object* pointer on object at this stack level
    ///
    object* get_obj(unsigned int stack_level) { return seq_obj(_count - stack_level - 1); }

    /// @brief same as get_obj
    ///
    /// @param stack_level the object stack level
    /// @return object* pointer on object at this stack level
    ///
    object* operator[](unsigned int stack_level) { return seq_obj(_count - stack_level - 1); }

    /// @brief returns the last object on stack
    ///
    /// @return object* the object
    ///
    object* back() {
        object* obj = NULL;
        if (_count > 0) obj = _base_pointer[_count - 1];
        return obj;
    }

    /// @brief get an object len
    ///
    /// @param index the object stack level
    /// @return unsigned int the length in bytes
    ///
    unsigned int get_len(unsigned int index) { return seq_len(_count - index - 1); }

    /// @brief get an object type
    ///
    /// @param index the object stack level
    /// @return cmd_type_t the object type
    ///
    cmd_type_t get_type(unsigned int index) { return seq_type(_count - index - 1); }

    /// @brief sequential object access (index is counted from front)
    ///
    /// @param index object index from front
    /// @return object* the object pointer
    ///
    object* seq_obj(unsigned int index) {
        object* obj = NULL;
        if (index < _count) obj = _base_pointer[index];
        return obj;
    }

    /// @brief get an object len
    ///
    /// @param index the object stack level from front
    /// @return unsigned int the length in bytes
    ///
    unsigned int seq_len(unsigned int index) {
        unsigned int len = 0;
        if (index < _count) len = _base_pointer[index]->_size;
        return len;
    }

    /// @brief get an object len
    ///
    /// @param index the object stack level from front
    /// @return cmd_type_t the object type
    ///
    cmd_type_t seq_type(unsigned int index) {
        cmd_type_t type = cmd_undef;
        if (index < _count) type = _base_pointer[index]->_type;
        return type;
    }

   private:
    char* _base;
    char* _current;
    object** _base_pointer;

    unsigned int _count;                //< stack count
    unsigned int _total_count_pointer;  //< total number of possible pointers
    unsigned int _total_size;           //< total allocated data size in bytes
};

/// @brief heap object, storing variables (=named object)
///
class heap : public stack {
   public:
    heap() {}
    virtual ~heap() {}

    /// @brief add a variable on the heap
    /// 
    /// @param name the variable name
    /// @param obj the variable content
    /// @param size the variable size in bytes
    /// @return object* 
    ///
    object* add(const string name, object* obj, unsigned int size) {
        map<string, unsigned int>::iterator i = _map.find(name);
        object* local = NULL;

        // variable does not exist in heap or already exists but its size is too
        // short -> allocate
        if (i != _map.end()) local = seq_obj(i->second);

        if (local == NULL || (local != NULL && size > local->_size)) {
            copy_and_push_back(obj, *this, size);
            _map[name] = ((stack*)this)->size() - 1;
        } else {
            // variable already exists in heap but previous was larger -> don't
            // reallocate copy a whole stack entry and push it back to another stack
            memcpy(local, obj, size);
            if (local->_type == cmd_number)
                ((number*)local)->move();
            else if (local->_type == cmd_complex)
                ((complex*)local)->move();
        }

        return local;
    }

    /// @brief get a variable
    /// 
    /// @param name the variable name
    /// @param obj the variable content
    /// @param size the variable size in bytes
    /// @return true the variable was found
    /// @return false the variable was not found
    ///
    bool get(const string name, object*& obj, unsigned int& size) {
        bool ret = false;
        map<string, unsigned int>::iterator i = _map.find(name);

        if (i != _map.end()) {
            obj = seq_obj(i->second);
            size = obj->_size;
            ret = true;
        }
        return ret;
    }

    /// @brief replace a variable value by another
    /// 
    /// @param name the variable name
    /// @param obj the new value
    /// @param size the variable size in bytes
    /// @return true the variable was found
    /// @return false the variable was not found
    ///
    bool replace_value(const string name, object* obj, unsigned int size) {
        bool ret = false;
        map<string, unsigned int>::iterator i = _map.find(name);

        if (i != _map.end()) {
            object* obj_dst = seq_obj(i->second);
            if (size <= obj_dst->_size) {
                (void)memcpy(obj_dst, obj, size);
                if (obj_dst->_type == cmd_number)
                    ((number*)obj_dst)->move();
                else if (obj_dst->_type == cmd_complex)
                    ((complex*)obj_dst)->move();
                ret = true;
            }
        }
    }

    /// @brief whether a variable exists in heap or not
    /// 
    /// @param name the variable name
    /// @return true the variable exists
    /// @return false variable does not exist
    ///
    bool exist(const string name) { return (_map.find(name) != _map.end()); }

    /// @brief get a variable by its index in heap
    /// 
    /// @param num the variable index
    /// @param name the variable name
    /// @param obj the variable content
    /// @param size the variable size in bytes
    /// @return true the variable was found
    /// @return false the variable was not found
    ///
    bool get_by_index(int num, string& name, object*& obj, unsigned int& size) {
        if (num >= 0 && num < (int)_map.size()) {
            object* local;
            map<string, unsigned int>::iterator i = _map.begin();

            for (int j = 0; j < num; j++) i++;

            local = (object*)seq_obj(i->second);
            name = i->first;
            obj = local;
            size = local->_size;
            return true;
        } else
            return false;
    }

    /// @brief erase a variable
    /// 
    /// @param name the variable name
    /// @return true the variable was found
    /// @return false the variable was not found
    ///
    bool erase(const string& name) {
        map<string, unsigned int>::iterator i = _map.find(name);
        bool ret = false;

        if (i != _map.end()) {
            // remove variable from map
            _map.erase(i->first);
            ret = true;

            // TODO: remove unused stack entries
        }
        return ret;
    }

    /// @brief erase all variables
    /// 
    void erase_all(void) {
        // map
        _map.erase(_map.begin(), _map.end());

        // and stack
        ((stack*)this)->erase();
    }

    /// @brief get the variables nb
    /// 
    /// @return unsigned int the variables nb
    ///
    unsigned int count_vars() { return _map.size(); }

   private:
    map<string, unsigned int> _map;
};

#endif  // __stack_h__
