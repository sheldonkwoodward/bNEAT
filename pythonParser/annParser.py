import networkx as nx
import matplotlib.pyplot as plt
from collections import defaultdict
import re
import sys


def process_node_txt(text):
    nodes = text.split("\n")
    nodes = filter(lambda x: x != "", nodes)
    nodes = map(lambda x: tuple(x.rstrip().split(" ")), nodes)
    nodes = sorted(nodes, key=lambda x: x[1])
    return nodes


def accumulate_sums(nodes):
    d = defaultdict(int)
    for n, l in nodes:
        d[l] += 1

    return d


def apply_visual_weights(nodes, d, limit):
    nodes = map(lambda x: (x[0], x[1], limit / (d[x[1]] + 1)), nodes)
    nodes = list(nodes)
    return nodes


def add_n_to_graph(g, nodes, d):
    for (n, l, offset) in nodes:
        g.add_node(n, pos=(int(l), d[l] * offset))
        d[l] -= 1


def process_conns_text(text):
    conns = text.split("\n")
    conns = filter(lambda x: x != "", conns)
    conns = map(lambda x: x.rstrip().split(" "), conns)
    return conns


def add_c_to_graph(g, conns):
    for t, f, w in conns:
        g.add_edge(t, f, weight=w)


def main():

    g = nx.Graph()
    file = sys.argv[1]
    limit = 10

    with open(file) as f:
        n_text, c_text = re.split("###", f.read())

        conns = process_conns_text(c_text)
        nodes = process_node_txt(n_text)

        d = accumulate_sums(nodes)

        nodes = apply_visual_weights(nodes, d, limit)

        add_n_to_graph(g, nodes, d)
        add_c_to_graph(g, conns)

    pos = nx.get_node_attributes(g, 'pos')
    nx.draw_networkx(g, pos)
    plt.show()


main()
