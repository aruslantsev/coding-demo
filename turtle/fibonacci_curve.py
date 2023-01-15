import turtle

turtle.speed(0)

STEP = 2


def gen_str(max_num):
    s = ['0', '01']
    for _ in range(max_num - 2):
        f = s[-1] + s[-2]
        s.append(f)

    return ''.join(s)


def draw(s):
    turtle.penup()
    turtle.setposition(-700, 450)
    turtle.pendown()
    for i, c in enumerate(s):
        turtle.forward(STEP)
        if c == "0":
            if i % 2 == 0:
                turtle.left(90)
            else:
                turtle.right(90)
    turtle.mainloop()


if __name__ == "__main__":
    fibo = gen_str(45)
    draw(fibo)
