#!/bin/env python3

import random
import math
import sys

if len(sys.argv) != 2:
    print("Usage: geninput.py <n>")
    sys.exit(1)

n = int(sys.argv[1])

r = 10
theta = 0

print(n)

for i in range(n):
    r *= random.uniform(0.2, 1.8)
    r = max(0.5, min(r, 2))
    theta += 2 * math.pi / n

    x = r * math.cos(theta)
    y = r * math.sin(theta)

    print(x, y)
