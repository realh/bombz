#include "config.h"
#include "hsdl/Application.h"
#include "hsdl/Exception.h"
#include "hsdl/Image.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <png.h>
#include "SDL.h"

using namespace std;

static int png_colortype_from_surface(SDL_Surface *surface)
{
    int colortype = PNG_COLOR_MASK_COLOR;
    if (surface->format->palette)
        colortype |= PNG_COLOR_MASK_PALETTE;
    else if (surface->format->Amask)
        colortype |= PNG_COLOR_MASK_ALPHA;
    return colortype;
}

static void png_user_warn(png_structp ctx, png_const_charp str)
{
    fprintf(stderr, "libpng: warning: %s\n", str);
}

void png_user_error(png_structp ctx, png_const_charp str)
{
    fprintf(stderr, "libpng: error: %s\n", str);
}

static void surf_to_png(SDL_Surface *surf, const char *filename)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        perror("Unable to save file");
        exit(1);
    }
    
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
            NULL, png_user_error, png_user_warn);
    if (!png_ptr)
    {
        fprintf(stderr, "png_create_write_struct error\n");
        exit(1);
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
        printf("png_create_info_struct error\n");
        exit(1);
    }
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        exit(1);
    }
    
    png_init_io(png_ptr, fp);
    int colortype = png_colortype_from_surface(surf);
    png_set_IHDR(png_ptr, info_ptr, surf->w, surf->h, 8, colortype,
            PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);
    png_set_packing(png_ptr);
    
    /*
    printf("Rmask = 0x%x Gmask = 0x%x Bmask = 0x%x Amask = 0x%x\n",
            surf->format->Rmask,
            surf->format->Gmask,
            surf->format->Bmask,
            surf->format->Amask);
    */
    if ((SDL_BYTEORDER == SDL_LIL_ENDIAN &&
            surf->format->Rmask > surf->format->Bmask) ||
        (SDL_BYTEORDER == SDL_BIG_ENDIAN &&
            surf->format->Rmask < surf->format->Bmask))
    {
        png_set_bgr(png_ptr);
    }
    
    png_bytep *row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * surf->h);
    for (int i = 0; i < surf->h; i++)
    {
        row_pointers[i] = (png_bytep)
                (((Uint8 *) surf->pixels) + i * surf->pitch);
    }
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, info_ptr);

    free(row_pointers);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

}

int main(int argc, char **argv)
{
    hsdl::Application application(argc, argv, 0);
    hgame::Platform *platform;
    hgame::Font *font;
    hsdl::Image *img;
    try
    {
        platform = application.getPlatform();
        font = platform->loadFont(64);
        img = (hsdl::Image *) font->render(hgame::Colour(0x8000),
                "Hello world!");
#if ENABLE_SHADOWS
        hsdl::Image *plain = img;
        img = (hsdl::Image *) plain->createShadow(8);
        delete plain;
#endif
    }
    catch (hsdl::Exception e)
    {
        fprintf(stderr, "%s\n", e.what());
        exit(1);
    }
    surf_to_png(img->getSurface(), "hello_world.png");
    return 0;
}
