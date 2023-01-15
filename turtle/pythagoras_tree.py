import time
import turtle
from collections import deque
import math

ITERATIONS = 15
ANGLE = 45


def draw_square(d0, d1):
    angle = ANGLE / 180 * math.pi
    dx = d1[0] - d0[0]
    dy = d1[1] - d0[1]
    d2 = (d0[0] - dy, d0[1] + dx)
    d3 = (d1[0] - dy, d1[1] + dx)

    dx_0 = dx * math.cos(angle) * math.cos(angle)
    dy_0 = dy * math.cos(angle) * math.cos(angle)
    dx_1 = dx * math.cos(angle) * math.sin(angle)
    dy_1 = dy * math.cos(angle) * math.sin(angle)
    d4_0 = (d2[0] + dx_0, d2[1] + dy_0)
    d4 = (d4_0[0] - dy_1, d4_0[1] + dx_1)

    turtle.penup()
    turtle.setposition(d0)
    turtle.pendown()
    turtle.setposition(d1)
    turtle.setposition(d3)
    turtle.setposition(d2)
    turtle.setposition(d0)
    turtle.penup()
    return d2, d3, d4


def main():
    turtle.tracer(0, 0)
    turtle.hideturtle()
    turtle.penup()
    squares = deque()
    squares.append([(-96, -400), (96, -400)])
    for i in range(ITERATIONS):
        new_squares = deque()
        while squares:
            d0, d1 = squares.popleft()
            d2, d3, d4 = draw_square(d0, d1)
            new_squares.append((d2, d4))
            new_squares.append((d4, d3))
        squares = new_squares
        turtle.update()
        time.sleep(0.1)
    turtle.mainloop()


main()
