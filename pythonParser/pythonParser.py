import re
from pprint import pprint
from blessings import Terminal
from time import sleep
import sys

t = Terminal()


def clean_scene(width, height):
    return [["   " for _ in range(width)] for _ in range(height)]


def print_scene(scene):
    hline_top = "\u250C" + "\u252C".join(
        ["\u2500" * 3 for i in range(len(scene[0]))]) + "\u2510" + "\n"
    hline_inner = "\u251c" + "\u253C".join(
        ["\u2500" * 3 for i in range(len(scene[0]))]) + "\u2524" + "\n"
    hline_bottom = "\u2514" + "\u2534".join(
        ["\u2500" * 3 for i in range(len(scene[0]))]) + "\u2518" + "\n"

    print(hline_top + hline_inner.join(
        map(lambda row: "\u2502" + "\u2502".join(map(lambda x: x, row)) + "\u2502\n",
            scene)) + hline_bottom)


def parse_locations(coords, scene):
    for coord in coords:
        # print(coord)
        x, y = re.match(r"(\d*),(\d*)", coord).groups()
        scene[int(y)][int(x)] = " \u2573 "

    return scene


def main():
    file = sys.argv[1]
    print(file)
    with open(file) as f:
        training_sessions = re.findall(
            r"(Board.*?)###", f.read(), flags=re.S | re.M)
        pprint(training_sessions)
        parsed = []
        for session in training_sessions:
            width, height = re.match(r"Board: (\d*),(\d*)", session).groups()
            iterations = re.match(r"Board: \d*,\d*(.*)@@@", session,
                                  re.S).groups()[0]
            iterations = map(
                lambda x: list(filter(lambda y: y != '', x.split("\n"))),
                re.split(r"@@@", iterations))
            iterations = list(map(lambda iteration: parse_locations(iteration, clean_scene(int(width), int(height))), iterations))
            parsed.append(iterations)

            for session in parsed:
                for iteration in session:
                    print_scene(iteration)
                    print(t.clear)
                    print_scene(iteration)
                    sleep(1)


main()
