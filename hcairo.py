import cairo
import rsvg

def svg_to_cairo(source, width, height, alpha = True, SCALE = 4):
    """ SVGs don't plot well at the small sizes we're using so
    use enlarged versions (default SCALE 4) and scale down the bitmaps
    afterwards """
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


def load_image(*args):
    if args[0].endswith(".svg"):
        return svg_to_cairo(*args)
    else:
        return cairo.ImageSurface.create_from_png(args[0])


def composite(obj1, obj2):
    """ Plots obj2 over a copy of obj1 (so obj1 is not altered).
    They should be the same size. """
    surf = cairo.ImageSurface(obj1.get_format(),
            obj1.get_width(), obj1.get_height())
    cr = cairo.Context(surf)
    cr.set_antialias(cairo.ANTIALIAS_NONE)
    cr.set_source_surface(obj1, 0, 0)
    cr.paint()
    cr.set_source_surface(obj2, 0, 0)
    cr.paint()
    return surf


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


def make_atlas(alpha, size, textures):
    """ Makes a PNG containing an atlas suitable for OpenGL with each texture
    having its edges and corners doubled-up.
    alpha is boolean, whether dest will have alpha channel. If false, the first
    object should be the floor, over which subsequent tiles are composited.
    size: size (width = height) of each tile.
    textures is similar to the montage() function's "objects" except each object
    may be an SVG filename instead of a Cairo surface. If it's a surface it
    should be a montage of foreground chrome subsections.
    The textures argument will be altered by this function. """
    if alpha:
        format = cairo.FORMAT_ARGB32
    else:
        format = cairo.FORMAT_RGB24
    for y in range(len(textures)):
        row = textures[y]
        for x in range(len(row)):
            obj = row[x]
            if obj == None:
                continue
            elif isinstance(obj, tuple) or isinstance(obj, list):
                lt = True
                s = obj[0]
            else:
                lt = False
                s = obj
            if isinstance(s, basestring):
                s = load_image(s, size, size, True)
            s = extend_edges(s)
            if not alpha:
                if x == 0 and y == 0:
                    floor = s
                else:
                    s = composite(floor, s)
            if lt:
                obj[0] = s
            else:
                row[x] = s
    return montage(format, textures)


def make_tile_atlas(dest, sources, size = 72, columns = 7):
    """ Makes an atlas of the tiles.
    dest is the filename where the PNG will be saved.
    sources is a flat list of SVG filenames; the first must be floor and it must
    end with the chrome subsections in order tl, tr, bl, br, horiz, vert. PNGs
    may be use dinstead, but then you must use .svg suffix (lower case is
    compulsory) for SVGs. """
    chromes = [load_image(c, size/ 2, size/ 2) for c in sources[-6:]]
    tl, tr, bl, br, horiz, vert = chromes
    sources = sources[:-6]
    def mc(*a):
        sources.append(montage(cairo.FORMAT_ARGB32,
                [[a[0], a[1]], [a[2], a[3]]]))
    mc(tl, horiz, vert, tl)
    mc(horiz, horiz, horiz, horiz)
    mc(horiz, tr, tr, vert)
    mc(vert, vert, vert, vert)
    mc(vert, bl, bl, horiz)
    mc(br, vert, horiz, br)
    mc(tl, tr, vert, vert)
    mc(tl, horiz, bl, horiz)
    mc(br, bl, tr, tl)
    mc(horiz, tr, horiz, br)
    mc(vert, vert, bl, br)
    mc(br, bl, horiz, horiz)
    mc(br, vert, tr, vert)
    mc(horiz, horiz, tr, tl)
    mc(vert, bl, vert, tl)
    mc(tl, tr, bl, br)
    table = []
    col = 0
    for s in sources:
        if col == 0:
            row = []
            table.append(row)
        row.append(s)
        col += 1
        if col == columns:
            col = 0
    # Last row may need padding
    while col < columns:
        row.append(None)
        col += 1
    atlas = make_atlas(False, size, table)
    atlas.write_to_png(dest)
    

svgs = ["svgs/floor.svg", "texture/dirt.png"] + \
    ["svgs/%s.svg" % s for s in "match picket bomb1 bomb2".split() + \
    ["explo%02d" % e for e in range(1, 12)] + \
    ["chrome_%s" % c for c in "tl tr bl br horiz vert".split()]]
make_tile_atlas("atlas.png", svgs)
