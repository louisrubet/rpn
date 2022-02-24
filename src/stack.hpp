// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_STACK_HPP_
#define SRC_STACK_HPP_

#include <algorithm>
#include <deque>
#include <map>
#include <string>
using namespace std;

#include "object.hpp"

/// @brief stack object, parens of program, storing execution stack values or programs
///
class rpnstack : public deque<object*> {
 public:
    rpnstack() {}
    virtual ~rpnstack() {
        for_each(begin(), end(), [](object* o) { delete o; });
        deque::erase(begin(), end());
    }

    // stack manipulation
    void erase(size_t first = 0, size_t nb = 1, bool del = true) {
        size_t last = std::min(first + nb, size());
        if (del) for_each(begin() + first, begin() + last, [](object* o) { delete o; });
        deque::erase(begin() + first, begin() + last);
    }

    void pop() { erase(); }

    // access helpers
    //
    cmd_type_t type(int level) {
        // carefull: caller must ensure that level is correct
        return at(level)->_type;
    }

    template <class objectType>
    auto& obj(int level) {
        // carefull: caller must ensure that level is correct
        return static_cast<objectType&>(*at(level));
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

    bool get(const string name, object*& obj) {
        auto i = find(name);
        if (i != end()) {
            obj = i->second;
            return true;
        }
        return false;
    }
};

#endif  // SRC_STACK_HPP_
