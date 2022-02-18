#ifndef __stack_h__
#define __stack_h__

#include <string.h>

#include <algorithm>
#include <deque>
#include <map>

#include "object.hpp"
using namespace std;

/// @brief stack object, parens of program, storing execution stack values or programs
///
class rpnstack : public deque<object*> {
   public:
    rpnstack() {}
    virtual ~rpnstack() {
        for_each(begin(), end(), [](object* o) { delete o; });
        deque::erase(begin(), end());
    }

    /// @brief copy a whole stack entry and push it back to another stack
    ///
    /// @param from copy from
    /// @param index_from index t ocopy from
    /// @param to copy to
    ///
    static void copy_and_push_front(rpnstack& from, unsigned int index_from, rpnstack& to) {
        // carefull: caller must ensure that index_from is correct
        to.push_front(from[index_from]->clone());
    }

    /// @brief erase a stack entry from it index
    ///
    /// @param first index to start
    /// @param last index to stop
    ///
    void erase(size_t first = 0, size_t nb = 1, bool del = true) {
        size_t last = std::min(first + nb, size());
        if (del)
            for_each(begin() + first, begin() + last, [](object* o) { delete o; });
        deque::erase(begin() + first, begin() + last);
    }

    /// @brief pop front several entries
    ///
    /// @param levels nb of entries
    ///
    void pop_front(size_t levels = 1) { erase(0, levels); }
    void pop() { erase(); }

    // access helpers
    cmd_type_t type(int level) {
        // carefull: caller must ensure that level is correct
        return at(level)->_type;
    }

    template <class objectType>
    auto obj(int level) {
        // carefull: caller must ensure that level is correct
        return static_cast<objectType*>(at(level));
    }

    template <class objectType>
    auto& value(int level) {
        // carefull: caller must ensure that level is correct
        return static_cast<objectType*>(at(level))->value;
    }

    void push(object* o) { deque<object*>::push_front(o); }
};

/// @brief heap object, storing variables (=named object)
///
class heap : public map<string, object*> {
   public:
    heap() {}
    virtual ~heap() { clear(); }

    void clear() {
        for_each(begin(), end(), [](auto it) { delete it.second; });
        map::erase(begin(), end());
    }

    /// @brief get a variable
    ///
    /// @param name the variable name
    /// @param obj the variable content
    /// @return true the variable was found
    /// @return false the variable was not found
    ///
    bool get(const string name, object*& obj) {
        bool ret = false;
        auto i = find(name);
        if (i != end()) {
            obj = i->second;
            ret = true;
        }
        return ret;
    }

    /// @brief get a variable by its index in heap
    ///
    /// @param num the variable index
    /// @param name the variable name
    /// @param obj the variable content
    /// @return true the variable was found
    /// @return false the variable was not found
    ///
    bool get_by_index(int num, string& name, object*& obj) {
        if (num >= 0 && num < (int)size()) {
            object* local;
            auto i = begin();
            for (; num > 0; num--, i++)
                ;
            obj = i->second;
            return true;
        } else
            return false;
    }
};

#endif  // __stack_h__
