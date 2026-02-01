#!/bin/bash
# Number base examples

echo "=== Number Base Examples ==="
echo

echo "Hexadecimal: 0xFF + 0x10"
rpn "0xFF 0x10 +"
echo

echo "Binary: 0b1010 (10) × 0b0110 (6)"
rpn "0b1010 0b0110 *"
echo

echo "Convert 255 to hex"
rpn "255 hex"
echo

echo "Convert 15 to binary"
rpn "15 bin"
echo

echo "Back to decimal"
rpn "dec 42"
echo

echo "Base 36 (uses digits 0-9 and letters A-Z)"
rpn "36b10 36b20 +"
echo

echo "Base 8 (octal): 8b100 (64 in decimal)"
rpn "8b100"
echo

echo "Mixing bases: 0xFF (255) + 0b11111111 (255)"
rpn "0xFF 0b11111111 +"
echo

echo "Set display to hex for multiple operations"
rpn "hex 16 16 * 2 /"
echo

echo "Binary addition table"
rpn "bin 0b1 0b1 +"
echo
