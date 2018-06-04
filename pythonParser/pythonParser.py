import re
from pprint import pprint
from blessings import Terminal
from time import sleep
import sys
import pygame as pyg
pyg.init()

t = Terminal()

color_palette = {
    "black": (0, 0, 0),
    "green": (0, 255, 0),
    "white": (255, 255, 255),
    "red": (255, 0, 0)
}
square_size = 50


def draw_grid(grid, screen, square_size):
    w, h = len(grid), len(grid[0])

    for i in range(h * w):
        if grid[int(i / w)][i % w] == "   ":
            pyg.draw.rect(screen, color_palette["white"],
                          ((i % w) * square_size, int(i / w) * square_size,
                           square_size - 2, square_size - 2))
        elif grid[int(i / w)][i % w] == " @ ":
            pyg.draw.rect(screen, color_palette["red"],
                          ((i % w) * square_size, int(i / w) * square_size,
                           square_size - 2, square_size - 2))
        else:
            pyg.draw.rect(screen, color_palette["green"],
                          ((i % w) * square_size, int(i / w) * square_size,
                           square_size - 2, square_size - 2))


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
        if re.match(r"food:(\d*),(\d*)", coord):
            x,y = re.match(r"food:(\d*),(\d*)", coord).groups()
            scene[int(y)][int(x)] = " @ "
        else:
            x, y = re.match(r"(\d*),(\d*)", coord).groups()
            scene[int(y)][int(x)] = " \u2573 "

    return scene


def main():
    file = sys.argv[1]
    with open(file) as f:
        training_sessions = re.findall(
            r"(Board.*?)###", f.read(), flags=re.S | re.M)

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

                    screen = pyg.display.set_mode((len(iteration[0])*square_size,
                                                   len(iteration)*square_size))
                    clock = pyg.time.Clock()
                    # pprint(iteration)
                    # print(t.clear)

                    screen.fill(color_palette["black"])
                    draw_grid(iteration, screen, square_size)

                    pyg.display.flip()
                    clock.tick(10)


main()
