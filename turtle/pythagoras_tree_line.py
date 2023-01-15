import time
import turtle


ITERATIONS = 15
LINESIZE = 128
ANGLE = 45
SCALE = 1 / 2**0.5


def main():
    turtle.tracer(0, 0)
    turtle.penup()
    turtle.setposition(0, -200)
    turtle.setheading(90)
    turtle.pendown()
    linesize = LINESIZE
    pens = [turtle]
    for i in range(ITERATIONS):
        newpens = []
        for pen in pens:
            pen.hideturtle()
            pen.forward(linesize)
            newpens.append(pen.clone())
            newpens.append(pen.clone())
            newpens[-2].right(ANGLE)
            newpens[-1].left(90 - ANGLE)
        pens = newpens
        linesize *= SCALE
        turtle.update()
        time.sleep(0.3)
    turtle.mainloop()


main()
