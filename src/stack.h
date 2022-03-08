// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_STACK_HPP_
#define SRC_STACK_HPP_

#include <deque>
#include <map>
#include <string>
#include <algorithm>
using std::deque, std::map, std::string;

#include "object.h"

/// @brief stack object, parens of program, storing execution stack values or programs
///
class rpnstack : public deque<Object*> {
 public:
    rpnstack() {}
    virtual ~rpnstack() {
        for_each(begin(), end(), [](Object* o) { delete o; });
        deque::erase(begin(), end());
    }

    // stack manipulation
    void erase(size_t first = 0, size_t nb = 1, bool del = true) {
        size_t last = std::min(first + nb, size());
        if (del) for_each(begin() + first, begin() + last, [](Object* o) { delete o; });
        deque::erase(begin() + first, begin() + last);
    }

    void pop() { erase(); }

    // access helpers
    //
    ObjectType type(int level) {
        // carefull: caller must ensure that level is correct
        return at(level)->type;
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

    void push(Object* o) { deque<Object*>::push_front(o); }
};

/// @brief heap object, storing variables (=named object)
///
class heap : public map<string, Object*> {
 public:
    heap() {}
    virtual ~heap() { clear(); }

    void clear() {
        for_each(begin(), end(), [](auto it) { delete it.second; });
        map::erase(begin(), end());
    }

    bool get(const string name, Object*& obj) {
        auto i = find(name);
        if (i != end()) {
            obj = i->second;
            return true;
        }
        return false;
    }
};

#endif  // SRC_STACK_HPP_
