import networkx as nx
import matplotlib.pyplot as plt
from collections import defaultdict
import re
import sys

g = nx.Graph()
file = sys.argv[1]
limit = 10

with open(file) as f:
    ys = dict()
    nodes, conns = re.split("###", f.read())

    nodes = nodes.split("\n")
    nodes = filter(lambda x: x != "", nodes)
    nodes = map(lambda x: tuple(x.rstrip().split(" ")), nodes)
    nodes = sorted(nodes, key=lambda x: x[1])
    d = defaultdict(int)
    for n, l in nodes:
        d[l] += 1

    nodes = map(lambda x: (x[0], x[1], limit / (d[x[1]] + 1)), nodes)
    nodes = list(nodes)

    for (n, l, offset) in nodes:
        g.add_node(n, pos=(int(l), d[l] * offset))
        d[l] -= 1

    conns = conns.split("\n")
    conns = filter(lambda x: x != "", conns)
    conns = map(lambda x: x.rstrip().split(" "), conns)

    for t, f, w in conns:
        g.add_edge(t, f, weight=w)

pos = nx.get_node_attributes(g, 'pos')
nx.draw_networkx(g, pos)
plt.show()
