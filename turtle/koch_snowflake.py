import turtle

LOGIC = {'F': 'F-F++F-F'}
ANGLE = 60

LINESIZE = 3**5
ITERATIONS = 5


def main():
    turtle.getscreen()
    turtle.penup()
    turtle.setposition(-420, 180)
    turtle.pendown()
    turtle.speed("fastest")
    axiom = "F++F++F++"
    tempAx = ""
    linesize = LINESIZE
    for i in range(ITERATIONS):
        for j in axiom:
            if j in LOGIC:
                tempAx += LOGIC[j]
            else:
                tempAx += j
        axiom, tempAx = tempAx, ""
        draw(axiom, linesize)
        # turtle.reset()
        linesize = linesize // 3


def draw(axiom, linesize):
    for k in axiom:
        if k == '+':
            turtle.right(ANGLE)
        elif k == '-':
            turtle.left(ANGLE)
        else:
            turtle.forward(linesize)


if __name__ == "__main__":
    main()
