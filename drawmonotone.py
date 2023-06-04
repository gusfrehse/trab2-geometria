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

[numVertices, numEdges] = list(map(int, input().split()))

points = []

for i in range(0, numVertices):
    points.append(list(map(int, input().split())))

print(points)

for i in range(numEdges):
    [pa, pb] = list(map(int, input().split()))

    [xa, ya] = points[pa]
    [xb, yb] = points[pb]

    plt.plot([xa, xb], [ya, yb], 'b-')
    # TODO: figure this out
    #plt.annotate(str(pa), xy=(0.3 * xa + 0.7 * xb, 0.3 * ya + 0.7 * yb))

plt.show()
