#!/bin/bash
# Fibonacci sequence using RPN

echo "=== Fibonacci Sequence Generator ==="
echo "Calculates first N Fibonacci numbers using for loop"
echo

# Fibonacci using for loop
# Initialize: 0 1 on stack
# Loop from 1 to N, each iteration: dup2 + (duplicate top 2, add them)

echo "First 10 Fibonacci numbers:"
rpn "
  del
  0 1
  1 10 for i
    dup2 +
  next
  drop
"
echo

echo "First 20 Fibonacci numbers (see how they grow!):"
rpn "
  del
  0 1
  1 20 for i
    dup2 +
  next
  drop
"
echo
