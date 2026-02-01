#!/bin/bash
# Complex number examples

echo "=== Complex Number Examples ==="
echo

echo "Addition: (1,2) + (3,4)"
rpn "(1,2) (3,4) +"
echo

echo "Multiplication: (1,2) × (3,4)"
rpn "(1,2) (3,4) *"
echo

echo "Conjugate of (3,4)"
rpn "(3,4) conj"
echo

echo "Magnitude (absolute value) of (3,4)"
rpn "(3,4) abs"
echo

echo "Argument (angle) of (1,1)"
rpn "(1,1) arg"
echo

echo "Natural logarithm: ln(1+2i)"
rpn "(1,2) ln"
echo

echo "Exponential: e^(1+2i)"
rpn "(1,2) exp"
echo

echo "Sine of complex number: sin(1+2i)"
rpn "(1,2) sin"
echo

echo "Rectangular to polar: (1,1) -> (r,θ)"
rpn "(1,1) r->p"
echo

echo "Polar to rectangular: (√2, π/4) -> (x,y)"
rpn "2 sqrt pi 4 / r->c"
echo
