#!/bin/env python3

import random
import math

r = random.uniform(0, 1)

theta = random.uniform(0, 2 * math.pi)

x = r * math.cos(theta)
y = r * math.sin(theta)

print(x, y)
