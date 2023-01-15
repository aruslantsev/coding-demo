import turtle
import time

LINESIZE = 600
ITERATIONS = 6
DRAW_ONLY_FINAL = False
CLEAR = True

# Koch
LOGIC = {'F': 'F-F++F-F'}
AXIOM = 'F'
ANGLE = 60  # 85
SCALE = 1 / 3

# LOGIC = {'F': 'F-F+F+F-F'}
# AXIOM = 'F'
# ANGLE = 90
# SCALE = 1 / 3

# Minkowski
# LOGIC = {'F': 'F-F+F+FF-F-F+F'}
# AXIOM = 'F'
# ANGLE = 90
# SCALE = 1 / 4

# Levi
# LOGIC = {'F': '-F++F-'}
# AXIOM = 'F'
# ANGLE = 45
# SCALE = 1 / 2**0.5

# Sierpinski triangle
# LOGIC = {
#     'F': 'F-G+F+G-F',
#     'G': 'GG'
# }
# AXIOM = 'F-G-G-'
# ANGLE = 120
# SCALE = 1 / 2

# LOGIC = {
#     'F': 'G-F-G',
#     'G': 'F+G+F'
# }
# AXIOM = 'F'
# ANGLE = 60
# SCALE = 1 / 2

# Sierpinsky curve
# LOGIC = {'X': 'XF+F+XF--F--XF+F+X'}  # +F+ -> +G+ for different length
# AXIOM = "F--XF--F--XF"
# SCALE = 1 / 2
# ANGLE = 45

# Sierpinsky carpet
# LOGIC = {'F': 'F+F-F-F-f+F+F+F-F', "f": "fff"}
# AXIOM = "F"
# SCALE = 1 / 3
# ANGLE = 90

# Dragon
# LOGIC = {
#     'X': 'X+YF+',
#     'Y': '-FX-Y'
# }
# AXIOM = 'FX'
# ANGLE = 90
# SCALE = 1 / 2

# Peano
# LOGIC = {
#     'L': 'LFRFL-F-RFLFR+F+LFRFL',
#     'R': 'RFLFR+F+LFRFL-F-RFLFR'
# }
# AXIOM = "L"
# SCALE = 1 / 4
# ANGLE = 90

# Hilbert
# LOGIC = {
#     'A': '-BF+AFA+FB-',
#     'B': '+AF-BFB-FA+'
# }
# AXIOM = "A"
# SCALE = 1 / 4
# ANGLE = 90
# Moore curve
# AXIOM = "AFA+F+AFA"

# Fractal plant
# LOGIC = {
#     'X': 'F-[[X]+X]+F[+FX]-X',
#     'F': 'FF'
# }
# AXIOM = "X"
# SCALE = 1 / 2
# ANGLE = 25

# Gosper curve
# LOGIC = {
#     'F': 'F-G--G+F++FF+G-',
#     'G': '+F-GG--G-F++F+G'
# }
# AXIOM = 'F'
# ANGLE = 60
# SCALE = 1 / 3


def main():
    turtle.getscreen()
    turtle.speed("fastest")
    axiom = AXIOM
    tempAx = ""
    linesize = LINESIZE
    for i in range(ITERATIONS):
        if not DRAW_ONLY_FINAL:
            draw(axiom, linesize)
            linesize = linesize * SCALE
            time.sleep(0.5)
            if CLEAR:
                turtle.clear()

        for j in axiom:
            if j in LOGIC:
                tempAx += LOGIC[j]
            else:
                tempAx += j
        axiom, tempAx = tempAx, ""

    draw(axiom, linesize)
    time.sleep(5)


def draw(axiom, linesize):
    turtle.penup()
    turtle.setposition(-200, 0)
    turtle.setheading(0)
    turtle.pendown()
    stack = []
    for k in axiom:
        if k == '+':
            turtle.right(ANGLE)
        elif k == '-':
            turtle.left(ANGLE)
        elif k in ['F', 'G']:
            turtle.forward(linesize)
        elif k in ['f', 'g']:
            turtle.penup()
            turtle.forward(linesize)
            turtle.pendown()
        elif k == "[":
            pos = turtle.position()
            angle = turtle.heading()
            stack.append((pos, angle))
        elif k == "]":
            turtle.penup()
            pos, angle = stack.pop()
            turtle.setposition(pos)
            turtle.setheading(angle)
            turtle.pendown()


if __name__ == "__main__":
    main()
