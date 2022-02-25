// Copyright (c) 2014-2022 Louis Rubet

#ifndef SRC_MPREAL_OUT_HPP_
#define SRC_MPREAL_OUT_HPP_

#include <mpreal.h>
using mpfr::mpreal;

#include <string>
#include <ostream>
using std::string, std::ostream;

ostream& MprealOutput10Base(ostream& out, const string& fmt, const mpreal& value);
ostream& MprealOutputNBase(ostream& out, int base, const mpreal& value);

#endif  // SRC_MPREAL_OUT_HPP_
