import turtle
from mandelbrot import mandelbrot, RE_START, RE_END, IM_START, IM_END

MIN_X = -50
MAX_X = 50
MIN_Y = -50
MAX_Y = 50
MAX_ITER = 20

turtle.speed(0)
turtle.hideturtle()


def draw(func, min_x, max_x, min_y, max_y, re_start, re_end, im_start, im_end, max_iter):
    width = max_x - min_x
    height = max_y - min_y
    turtle.getscreen()
    turtle.tracer(0, 0)
    turtle.penup()
    for iteration in range(1, max_iter):
        print(iteration)
        for x in range(MIN_X, MAX_X):
            for y in range(MIN_Y, MAX_Y):
                # Convert pixel coordinate to complex number
                c = complex(
                    re_start + ((x - min_x) / width) * (re_end - re_start),
                    im_start + ((y - min_y) / height) * (im_end - im_start),
                )
                # Compute the number of iterations
                m = func(c, iteration)
                color = (max_iter - m) / max_iter  # 255 - int(m * 255 / MAX_ITER)
                turtle.setposition(x, y)
                turtle.pendown()
                turtle.color((color, color, color))
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
