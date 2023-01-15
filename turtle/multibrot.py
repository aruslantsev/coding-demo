from mandelbrot import draw

RE_START = -2.5
RE_END = 2.5
IM_START = -2.5
IM_END = 2.5

MIN_X = -400
MAX_X = 400
MIN_Y = -400
MAX_Y = 400
MAX_ITER = 255


def multibrot(c, max_iter):
    z = 0
    n = 0
    while abs(z) <= 2 and n < max_iter:
        if abs(z) > 0:
            z = z ** 2.5 + c
        else:
            z = z + c
        n += 1
    return n


if __name__ == "__main__":
    draw(
        func=multibrot,
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
