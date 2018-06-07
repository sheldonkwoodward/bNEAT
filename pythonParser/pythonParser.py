import re
from pprint import pprint
from blessings import Terminal
from time import sleep
import sys
import pygame as pyg
from functools import reduce

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
    w, h = len(grid[0]), len(grid)

    for i in range(h * w):
        if grid[int(i / w)][i % w] == 0:
            color = color_palette["white"]
        elif grid[int(i / w)][i % w] == 1:
            color = color_palette["red"]
        else:
            color = color_palette["green"],

        pyg.draw.rect(screen, color,
                      ((i % w) * square_size, int(i / w) * square_size,
                       square_size - 2, square_size - 2))


def clean_scene(width, height):
    return [[0 for _ in range(width)] for _ in range(height)]


def parse_locations(coords, scene):
    for coord in coords:
        if re.match(r"food:(\d*),(\d*)", coord):
            x, y = re.match(r"food:(\d*),(\d*)", coord).groups()
            scene[int(y)][int(x)] = 1
        else:
            x, y = re.match(r"(\d*),(\d*)", coord).groups()
            scene[int(y)][int(x)] = -1

    return scene


def get_size(session):
    return re.match(r"Board: (\d*),(\d*)", session).groups()


def get_iterations(session):
    return re.match(r"Board: \d*,\d*(.*)@@@", session, re.S).groups()[0]


def split_iterations(iterations):
    return re.split(r"@@@", iterations)


def split_locations(iterations):
    return map(lambda x: list(filter(lambda y: y != '', x.split("\n"))),
               iterations)


def create_scences(iterations, width, height):
    return list(map(lambda iteration: parse_locations(iteration, clean_scene(int(width), int(height))), iterations))


def parse_sessions(training_sessions):
    parsed = []
    for session in training_sessions:
        width, height = get_size(session)
        iterations = get_iterations(session)
        iterations = split_iterations(iterations)
        iterations = split_locations(iterations)
        iterations = create_scences(iterations, width, height)
        parsed.append(iterations)
    return parsed


def main():

    file = sys.argv[1]
    with open(file) as f:
        training_sessions = re.findall(
            r"(Board.*?)###", f.read(), flags=re.S | re.M)

        parsed = parse_sessions(training_sessions)

        clock = pyg.time.Clock()
        for i, session in enumerate(parsed):
            print(i)
            for iteration in session:
                width, height = len(iteration[0]), len(iteration)
                screen = pyg.display.set_mode((width * square_size,
                                               height * square_size))

                screen.fill(color_palette["black"])
                draw_grid(iteration, screen, square_size)
                pyg.display.flip()
                clock.tick(30)


main()
