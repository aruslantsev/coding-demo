import turtle

turtle.speed(0)
turtle.hideturtle()

# Plot window
RE_START = -2.5
RE_END = 1
IM_START = -1
IM_END = 1

MIN_X = -400
MAX_X = 400
MIN_Y = -300
MAX_Y = 300
MAX_ITER = 255


def mandelbrot(c, max_iter):
    z = 0
    n = 0
    while abs(z) <= 2 and n < max_iter:
        z = z ** 2 + c
        n += 1
    return n


def get_color(color):
    color = (color, color**2, color**0.5)
    return color


def draw(func, min_x, max_x, min_y, max_y, re_start, re_end, im_start, im_end, max_iter):
    width = max_x - min_x
    height = max_y - min_y
    turtle.getscreen()
    turtle.tracer(0, 0)
    turtle.penup()
    for x in range(min_x, max_x):
        for y in range(min_y, max_y):
            # Convert pixel coordinate to complex number
            c = complex(
                re_start + ((x - min_x) / width) * (re_end - re_start),
                im_start + ((y - min_y) / height) * (im_end - im_start),
            )
            # Compute the number of iterations
            m = func(c, max_iter=max_iter)
            color = m / max_iter  # 255 - int(m * 255 / MAX_ITER)
            color = get_color(color)
            turtle.setposition(x, y)
            turtle.pendown()
            turtle.color(color)
            turtle.dot(1)
            turtle.penup()
        turtle.update()
    turtle.mainloop()


if __name__ == "__main__":
    draw(
        func=mandelbrot,
        min_x=MIN_X,
        max_x=MAX_X,
        min_y=MIN_Y,
        max_y=MAX_Y,
        re_start=RE_START,
        re_end=RE_END,
        im_start=IM_START,
        im_end=IM_END,
        max_iter=MAX_ITER,
    )
