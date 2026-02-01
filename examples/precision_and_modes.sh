#!/bin/bash
# Precision and display mode examples

echo "=== Precision and Display Modes ==="
echo

echo "Default precision (128 bits, 38 decimal digits)"
rpn "1 3 /"
echo

echo "Increase precision to 256 bits"
rpn "256 prec 1 3 /"
echo

echo "High precision calculation: e^π - π"
rpn "512 prec pi exp pi -"
echo

echo "Fixed-point mode: 6 decimal places"
rpn "6 fix pi"
echo

echo "Fixed-point mode: 10 decimal places"
rpn "10 fix pi"
echo

echo "Scientific notation: 12 significant digits"
rpn "12 sci 1 1000000 /"
echo

echo "Scientific notation: small numbers"
rpn "10 sci 0.000000123456789"
echo

echo "Standard mode (auto-select best format)"
rpn "std pi"
echo

echo "Demonstrating arbitrary precision: 1/3 × 3"
echo "With default precision:"
rpn "default 1 3 / 3 *"
echo

echo "With very high precision (512 bits):"
rpn "512 prec 1 3 / 3 *"
echo

echo "Calculate π to many digits"
rpn "512 prec 64 fix pi"
echo

echo "Reset to defaults"
rpn "default std pi"
echo
