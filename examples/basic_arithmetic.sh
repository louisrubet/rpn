#!/bin/bash
# Basic arithmetic examples for rpn

echo "=== Basic Arithmetic Examples ==="
echo

echo "Addition: 2 + 3"
rpn "2 3 +"
echo

echo "Subtraction: 10 - 7"
rpn "10 7 -"
echo

echo "Multiplication: 6 × 7"
rpn "6 7 *"
echo

echo "Division: 22 ÷ 7"
rpn "22 7 /"
echo

echo "Power: 2^10"
rpn "2 10 ^"
echo

echo "Square root: √144"
rpn "144 sqrt"
echo

echo "Factorial: 10!"
rpn "10 fact"
echo

echo "Complex expression: (5 + 3) × 2 - 4"
rpn "5 3 + 2 * 4 -"
echo

echo "Percentage change: 100 to 150"
rpn "100 150 %CH"
echo
