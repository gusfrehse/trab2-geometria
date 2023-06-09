#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
from matplotlib.lines import Line2D

# Format: 
# <number of vertices> <space> <number of edges>
# <x> <space> <y>
# ... 
# <index of start vertex> <space> <index of end vertex>
# ...

colors = ['r', 'g', 'b']

numVertices= int(input())

points = []

[x0, y0] = list(map(float, input().split()))
[xp, yp] = [x0, y0]

plt.annotate('0', xy=(x0, y0))

for i in range(1, numVertices):
    [x, y] = list(map(float, input().split()))

    plt.plot([xp, x], [yp, y], colors[i % len(colors)] + '-')
    plt.annotate(str(i), xy=(x, y))

    xp = x
    yp = y

plt.plot([xp, x0], [yp, y0], 'b-')

plt.show()
