import turtle
import random


def main():
    pen = turtle.Turtle()
    pen.speed(0)
    pen.speed("fastest")
    pen.color("green")
    pen.penup()
    pen.hideturtle()

    x = 0
    y = 0
    for n in range(100000):
        turtle.tracer(0, 0)
        if n % 100 == 0:
            print(n)
            turtle.update()
        pen.goto(70 * x, 70 * y - 250)
        pen.pendown()
        pen.dot(1)
        pen.penup()
        r = random.random()
        if r < 0.01:  # 0.01 Stem
            x, y =  0.00 * x + 0.00 * y,  0.00 * x + 0.16 * y + 0.00
        elif r < 0.86:  # 0.86 Successively smaller leaflet
            x, y =  0.85 * x + 0.04 * y, -0.04 * x + 0.85 * y + 1.60
        elif r < 0.93:  # 0.93 Largest left-hand leaflet
            x, y =  0.20 * x - 0.26 * y,  0.23 * x + 0.22 * y + 1.60
        else:  # Largest right-hand leaflet
            x, y = -0.15 * x + 0.28 * y,  0.26 * x + 0.24 * y + 0.44

    turtle.mainloop()

main()
