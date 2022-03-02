#!/bin/bash

if [ -z "${1}" ]
  then
    echo "Syntax: ${0} <rpn path>"
    exit 1
fi

rpn=${1}

FG_RED="\033[0;31m"
FG_GREEN="\033[0;32m"
COLOR_OFF="\033[0m"

failed=0

function checkmem {
    echo -n "${*}"
    valgrind ${rpn} "${*}" 2>&1 | grep "in use at exit: 0 bytes in 0 blocks" >/dev/null
    if [ ${?} -eq 0 ]; then
        echo -en " .. ${FG_GREEN}" && echo -n "ok" && echo -e "${COLOR_OFF}"
    else
        echo -en " .. ${FG_RED}" && echo -n "FAILED" && echo -e "${COLOR_OFF}"
        failed=1
    fi
}

quick_tests=(
    "1.2 \"string\" 'ok' (2,3) << 9 nop >>"                        # base types inputs
    "nop help version uname history quit"                          # general commands
    "38 std 38 fix 38 sci 10 prec 2 default \"toward zero\" round" # modes
    "1 2 + 2 3 * / 4 - inv neg chs"                                # base operations on numbers
    "(1,2) (2,3) + (4,5) * (4,2) / (8,8) - inv neg chs"            # base operations on complexes
    "\"ab\" \" cd\" +"                                             # base operations on strings
    "2 3 ^ 2 ^ 2 pow"                                              # usual operations on numbers 1
    "(2,3) (1,1) ^ 2 ^ 2 (2,3) ^"                                  # usual operations on complexes 1
    "1 2 3 swap"                                                   # stack operations
)

functional_tests=($(cat all.md | grep "^@include" | awk '{print $2}'))

# echo "Quick rpn memory checks"
# for i in ${!quick_tests[@]}; do
#     checkmem "${quick_tests[$i]}"
# done

echo "Functional rpn memory checks"
for i in ${!functional_tests[@]}; do
    checkmem "\"${functional_tests[$i]}\" test"
done
exit ${failed}
