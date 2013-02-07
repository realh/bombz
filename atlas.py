#!/usr/bin/env python

import os
import sys

import cairo
import rsvg

def svg_to_cairo(source, width, height, alpha = True, SCALE = 4,
        w2 = 0, h2 = 0):
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
    if SCALE == 1 and (not w2 or w2 == width) and (not h2 or h2 == height):
        return surf
    if not w2:
        w2 = width
    if not h2:
        h2 = height
    surf2 = cairo.ImageSurface(format, w2, h2)
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


def round_to_powerof2(a):
    b = 2
    while b < a:
        b <<= 1
    return b


def montage(format, objects, powerof2 = False):
    """ Lays out multiple objects in a grid.
    format is a cairo format constant.
    objects is a 2D array, each row must contain the same number of members. An
    object may span more than 1 column and/or row by specifying [object, cols,
    rows]. Spanned cells should contain None.
    If powerof2 is True, final sizes will be rounded up to a power of 2. """
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
    if powerof2:
        width = round_to_powerof2(width)
        height = round_to_powerof2(height)
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
                xscale = o[1]
                yscale = o[2]
                o = o[0]
            else:
                xscale = 1
                yscale = 1
            x0 = offsets[row][col][0]
            y0 = offsets[row][col][1]
            cr.set_source_surface(o, x0, y0)
            w = sizes[row][col][0]
            h = sizes[row][col][1]
            cr.rectangle(x0, y0, w * xscale, h * yscale)
            cr.fill()
    return surf


"""
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
"""


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
            #s = extend_edges(s)
            if not alpha:
                if x == 0 and y == 0:
                    floor = s
                else:
                    s = composite(floor, s)
            if lt:
                obj[0] = s
            else:
                row[x] = s
    return montage(format, textures, True)


def load_and_clip(source, top, left, size = 72):
    """ Chrome quarters need to be double width and/or height to make shadows
    work properly when rendering SVG, then we clip out the section we need.
    top and left are the offsets of clip origin from source origin in multiples
    of size / 2/ """
    src = svg_to_cairo(source, size, size, True)
    dest = cairo.ImageSurface(cairo.FORMAT_ARGB32, size / 2, size / 2)
    cr = cairo.Context(dest)
    cr.set_antialias(cairo.ANTIALIAS_NONE)
    cr.set_source_surface(src, -top * size / 2, -left * size / 2)
    cr.paint()
    return dest



def make_game_tile_atlas(dest, sources, size = 72, columns = 6):
    """ Makes an atlas of the game tiles.
    dest = output PNG filename.
    sources is a flat list of SVG filenames; the first must be floor and it must
    end with the chrome subsections in order tl, tr, bl, br, horiz, vert. PNGs
    may be used instead, but then you must use .svg suffix (lower case is
    compulsory) for SVGs. """
    tl = load_and_clip(sources[-6], 0, 0, size)
    tr = load_and_clip(sources[-5], 1, 0, size)
    bl = load_and_clip(sources[-4], 0, 1, size)
    br = load_and_clip(sources[-3], 1, 1, size)
    horiz = load_and_clip(sources[-2], 1, 1, size)
    vert = load_and_clip(sources[-1], 1, 1, size)
    sources = sources[:-6]
    def mc(*a):
        sources.append(montage(cairo.FORMAT_ARGB32,
                [[a[0], a[1]], [a[2], a[3]]]))
    # 00: tl corner             08: cross
    # 01: horiz straight        09: right end
    # 02: tr corner             10: bottom end
    # 03: vert straight         11: inverted T
    # 04: bl corner             12: T with leg facing left
    # 05: br corner             13: T
    # 06: top end               14: T with leg facing right
    # 07: left end              15: island
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


def make_game_alpha_atlas(dest, sources, size = 72):
    """ Arranges the graphics with alpha into a texture atlas.
    dest = output PNG filename.
    The order of sources (list of SVG filenames) should be
    explo00, 4 droids, 2 bombs.
    """
    explo0 = svg_to_cairo(sources[0], size * 3, size * 3)
    table = [[[explo0, 3, 3], None, None, sources[1], sources[2]],
            [None, None, None, sources[3], sources[4]],
            [None, None, None, sources[5], sources[6]]]
    atlas = make_atlas(True, size, table)
    atlas.write_to_png(dest)


def make_title_logo(dest, source, width, height):
    w2 = round_to_powerof2(width)
    h2 = round_to_powerof2(height)
    surf = svg_to_cairo(source, width, height, True, 1, w2, h2)
    surf.write_to_png(dest)


# We don't need this because it turns out to return the same value for every
# pixel size
def get_best_packing(size, ntiles, limit = 2048):
    """ Works out the most efficient way to pack ntiles squares of uniform
    size into a texture atlas,returning the number of columns. limit is
    the maximum number of pixels in either direction. """
    best_pix = limit * limit
    best_cols = int(math.sqrt(ntiles))
    for cols in range(best_cols, ntiles + 1):
        if cols * size > limit:
            break
        rows = int(ntiles / cols)
        if ntiles % cols:
            rows += 1
        pix = round_to_powerof2(cols * size) * round_to_powerof2(rows * size)
        if pix < best_pix:
            best_cols = cols
            best_pix = pix
    return best_cols


def omd(d):
    if not os.path.isdir(d):
        os.makedirs(d)


def omdp(d):
    omd(os.path.dirname(d))


def builder(m, dest, size):
    if m == 'tiles':
        omdp(dest)
        make_game_tile_atlas(dest,
                ["svgs/floor.svg", "texture/dirt.png"] + \
                ["svgs/%s.svg" % s \
                    for s in "match picket bomb1 bomb2".split() + \
                        ["explo%02d" % e for e in range(1, 12)] + \
                        ["chrome_%s" % c \
                            for c in "tl tr bl br horiz vert".split()]],
                size)
    elif m == 'alpha':
        omdp(dest)
        make_game_alpha_atlas(dest,
                ["svgs/%s.svg" % s for s in ["explo00"] + \
                    ["droid%s" % d for d in "left right up down".split()] \
                            + ["bomb1", "bomb2"]],
                size)
    elif m == 'logo':
        omdp(dest)
        make_title_logo(dest, "svgs/title_logo.svg", size * 16, size * 4)
    elif m == 'assets':
        builder('tiles', dest + "/tile_atlas.png", size)
        builder('alpha', dest + "/alpha_atlas.png", size)
        builder('logo', dest + "/title_logo.png", size)
    elif m == 'vpad':
        omdp(dest)
        svg_to_cairo("svgs/vpad.svg", size, size).write_to_png(dest)
    elif m == 'vpads':
        for ds in (('ldpi', 96), ('mdpi', 128), ('hdpi', 192), ('xhdpi', 256)):
            builder('vpad', dest + "/drawable-" + ds[0] + "/vpad.png", ds[1])
    elif m == 'all':
        builder('assets', dest + "/assets/pngs/%d" % size, size)
        builder('vpads', dest + "/res", size)


if __name__ == "__main__":
    m = sys.argv[1]
    dest = sys.argv[2]
    size = int(sys.argv[3])
    builder(m, dest, size)
