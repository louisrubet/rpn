#!/bin/bash
# Programs and variables examples

echo "=== Programs and Variables ==="
echo

echo "Store a value in variable 'x'"
rpn "42 'x' sto x"
echo

echo "Arithmetic on variables: add 10 to x"
rpn "10 'x' sto+ x"
echo

echo "Store a program (square function)"
echo "Program: << dup * >>"
rpn "<< dup * >> 'square' sto 7 square"
echo

echo "Store a program (cube function)"
echo "Program: << dup dup * * >>"
rpn "<< dup dup * * >> 'cube' sto 5 cube"
echo

echo "Store and use conditional"
echo "Program: << 0 > << 'positive' >> << 'negative' >> ifte >>"
rpn "<< 0 > << 'positive' >> << 'negative' >> ifte >> 'sign' sto"
rpn "5 sign"
rpn "-3 sign"
echo

echo "Quadratic formula: (-b ± √(b²-4ac)) / 2a"
echo "For equation: x² - 5x + 6 = 0 (a=1, b=-5, c=6)"
echo "Program: << 'c' sto 'b' sto 'a' sto"
echo "            b neg b sq 4 a * c * - sqrt +"
echo "            2 a * / >>"
rpn "<<
  'c' sto 'b' sto 'a' sto
  b neg b sq 4 a * c * - sqrt +
  2 a * /
>> 'quad+' sto"

echo "Solution 1 (using +):"
rpn "1 -5 6 quad+"
echo

echo "Store quad- for the other root"
rpn "<<
  'c' sto 'b' sto 'a' sto
  b neg b sq 4 a * c * - sqrt -
  2 a * /
>> 'quad-' sto"

echo "Solution 2 (using -):"
rpn "1 -5 6 quad-"
echo
