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
    virtual ~rpnstack() {}

    /// @brief copy a whole stack entry and push it back to another stack
    ///
    /// @param from copy from
    /// @param index_from index t ocopy from
    /// @param to copy to
    ///
    static void copy_and_push_back(rpnstack& from, unsigned int index_from, rpnstack& to) {
        to.push_back(from[index_from]);
    }

    static void copy_and_push_front(rpnstack& from, unsigned int index_from, rpnstack& to) {
        to.push_front(from[index_from]);
    }

    /// @brief pop back several entries
    ///
    /// @param levels nb of entries
    ///
    void pop_back(int levels) {
        for (int i = 0; i < levels; i++) deque::pop_back();
    }

    /// @brief pop back 1 entry
    ///
    /// @return retrieved object
    ///
    object* pop_back() {
        object* o = back();
        pop_back(1);
        return o;
    }
};

/// @brief heap object, storing variables (=named object)
///
class heap : public map<string, object*> {
   public:
    heap() {}
    virtual ~heap() {}

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
