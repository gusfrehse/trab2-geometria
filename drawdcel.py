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

[numVertices, numEdges] = list(map(int, input().split()))

points = []

for i in range(0, numVertices):
    points.append(list(map(float, input().split())))

print(points)

for i in range(numEdges):
    [pa, pb] = list(map(int, input().split()))
    print(pa, pb)

    [xa, ya] = points[pa]
    [xb, yb] = points[pb]

    plt.plot([xa, xb], [ya, yb], colors[i % len(colors)] + '-')
    plt.annotate(str(pa), xy=(xa, ya)) # point

plt.show()