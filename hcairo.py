import cairo
import rsvg

def svg_to_cairo(source, width, height, alpha = True, SCALE = 4):
    # SVGs don't plot well at the small sizes we're using so
    # use enlarged versions and scale down the bitmaps afterwards
    svg = rsvg.Handle(file = source)
    if alpha:
        format = cairo.FORMAT_ARGB32
    else:
        format = cairo.FORMAT_RGB24
    surf = cairo.ImageSurface(format, width * SCALE, height * SCALE)
    cr = cairo.Context(surf)
    cr.set_antialias(cairo.ANTIALIAS_NONE)
    cr.scale(float(width * SCALE) / svg.props.width,
            float(height * SCALE) / svg.props.height)
    svg.render_cairo(cr)
    if SCALE == 1:
        return surf
    surf2 = cairo.ImageSurface(format, width, height)
    cr = cairo.Context(surf2)
    cr.set_antialias(cairo.ANTIALIAS_DEFAULT)
    cr.set_antialias(cairo.ANTIALIAS_GRAY)
    cr.scale(1.0 / SCALE, 1.0 / SCALE)
    cr.set_source_surface(surf, 0, 0)
    cr.paint()
    return surf2


def make_array(cols, rows):
    a = []
    for row in range(rows):
        r = []
        for col in range(cols):
            r.append([0, 0])
        a.append(r)
    return a
    

def montage(format, objects):
    """ Lays out multiple objects in a grid.
    format is a cairo format constant.
    objects is a 2D array, each row must contain the same number of members. An
    object may span more than 1 column and/or row by specifying [object, cols,
    rows]. Spanned cells should contain None. """
    # First calculate the size of the content of each cell
    rows = len(objects)
    columns = len(objects[0])
    sizes = make_array(columns, rows)
    for row in range(rows):
        if len(objects[row]) != columns:
            raise Exception( \
                    "Table of montage objects has non-uniform row lengths")
        for col in range(columns):
            o = objects[row][col]
            if o == None:
                continue
            elif isinstance(o, cairo.Surface):
                span_x = 1
                span_y = 1
            else:
                o, span_x, span_y = o
            for y in range(span_y):
                for x in range(span_x):
                    sizes[row + y][col + x] = \
                            [o.get_width() / span_x, o.get_height() / span_y]
    # Then make sure each column is as wide as its widest element
    for col in range(columns):
        biggest = 0
        for row in range(rows):
            s = sizes[row][col][0]
            if s > biggest:
                biggest = s
        for row in range(rows):
            sizes[row][col][0] = biggest
    # Do the same for rows
    for row in range(rows):
        biggest = 0
        for col in range(columns):
            s = sizes[row][col][1]
            if s > biggest:
                biggest = s
        for col in range(columns):
            sizes[row][col][1] = biggest
    # Now work out offsets from sizes
    offsets = make_array(columns, rows)
    for row in range(rows):
        for col in range(columns):
            if col > 0:
                offsets[row][col][0] = \
                        offsets[row][col - 1][0] + sizes[row][col - 1][0]
            if row > 0:
                offsets[row][col][1] = \
                        offsets[row - 1][col][1] + sizes[row - 1][col][1]
    # Now we can work out final size
    width = offsets[-1][-1][0] + sizes[-1][-1][0]
    height = offsets[-1][-1][1] + sizes[-1][-1][1]
    # Create a new surface...
    surf = cairo.ImageSurface(format, width, height)
    cr = cairo.Context(surf)
    # ...and render objects into it
    for row in range(rows):
        for col in range(columns):
            o = objects[row][col]
            if o == None:
                continue
            elif not isinstance(o, cairo.Surface):
                o = o[0]
            x0 = offsets[row][col][0]
            y0 = offsets[row][col][1]
            cr.set_source_surface(o, x0, y0)
            cr.rectangle(x0, y0, sizes[row][col][0],  sizes[row][col][1])
            cr.fill()
    return surf


def extend_edges(surf):
    width = surf.get_width()
    height = surf.get_height()
    dest = cairo.ImageSurface(surf.get_format(), width + 2, height + 2)
    cr = cairo.Context(dest)
    cr.set_antialias(cairo.ANTIALIAS_NONE)
    for y in (-1, 0, 1):
        for x in (-1, 0, 1):
            if y == -1:
                y0 = 0
                h = 1
                y1 = 0
            elif y == 0:
                y0 = 1
                h = height
                y1 = 1
            elif y == 1:
                y0 = height + 1
                h = 1
                y1 = 2
            if x == -1:
                x0 = 0
                w = 1
                x1 = 0
            elif x == 0:
                x0 = 1
                w = width
                x1 = 1
            elif x == 1:
                x0 = width + 1
                w = 1
                x1 = 2
            cr.set_source_surface(surf, x1, y1)
            cr.rectangle(x0, y0, w, h)
            cr.fill()
    return dest


#surf1 = svg_to_cairo("svgs/chrome_br.svg", 36, 36)
#surf2 = svg_to_cairo("svgs/chrome_tl.svg", 36, 36)
#surf3 = svg_to_cairo("svgs/floor.svg", 36, 36)
#surf = montage(cairo.FORMAT_ARGB32, [[surf1, surf3], [surf3, surf2]])

surf = svg_to_cairo("svgs/droidup.svg", 72, 72)
surf = extend_edges(surf)
surf.write_to_png("bomb1.png")
