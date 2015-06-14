#include "text_texture.hpp"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	enum { RMASK = 0xff000000, GMASK = 0x00ff0000,
        BMASK = 0x0000ff00,	AMASK = 0x000000ff};
#else
	enum { RMASK = 0x000000ff,	GMASK = 0x0000ff00,
        BMASK = 0x00ff0000,	AMASK = 0xff000000};
#endif

TextTexture::TextTexture(
    SDL_Rect txt_rect,
    SDL_Surface *bg_img) :
        txt_rect(txt_rect),
        bg_img(bg_img)
{
    int w, h;
    if (bg_img) {
        w = bg_img->w;
        h = bg_img->h;
    } else {
        w = txt_rect.w;
        h = txt_rect.h;
    }

    surface = SDL_CreateRGBSurface(
        SDL_SWSURFACE,
        w,
        h,
        32,
        RMASK,
        GMASK,
        BMASK,
        AMASK);

    if(!surface){
        fprintf(stderr, "Error creating a surface for the sprite\n");
    }

    txt_surface = SDL_CreateRGBSurface(
        SDL_SWSURFACE,
        txt_rect.w,
        txt_rect.h,
        32,
        RMASK,
        GMASK,
        BMASK,
        AMASK);

    if(SDL_MUSTLOCK(txt_surface))
        SDL_LockSurface(txt_surface);

    cairo_surface = cairo_image_surface_create_for_data(
            static_cast<unsigned char *>(txt_surface->pixels),
            CAIRO_FORMAT_RGB24,
            txt_surface->w,
            txt_surface->h,
            txt_surface->pitch);

    cr = cairo_create(cairo_surface);

    font_description = pango_font_description_new();
    pango_font_description_set_family(font_description, "serif");
    pango_font_description_set_weight(font_description,
            PANGO_WEIGHT_BOLD);
    pango_font_description_set_absolute_size(
            font_description,
            20 * PANGO_SCALE);

    layout = pango_cairo_create_layout(cr);
    pango_layout_set_font_description(layout, font_description);
    pango_layout_set_width(layout, w * PANGO_SCALE); 
    pango_layout_set_height(layout, h * PANGO_SCALE); 
    pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);

    if(SDL_MUSTLOCK(txt_surface))
        SDL_UnlockSurface(txt_surface);

    text[0] = '\0';

    glGenTextures(1, &texture_id);
    glBindTexture(
            GL_TEXTURE_2D,
            texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w,
            h,
            0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    return;
}

void TextTexture::set_text(const char *text_input)
{
    strncpy(text, text_input, TEXTLENGTH - 1);

    SDL_BlitSurface(bg_img, NULL, surface, NULL);
    if(SDL_MUSTLOCK(txt_surface)) {
        SDL_LockSurface(txt_surface);
    }

    pango_layout_set_markup(layout, text, -1);
    pango_cairo_show_layout(cr, layout);

    if(SDL_MUSTLOCK(txt_surface))
        SDL_UnlockSurface(txt_surface);

    SDL_BlitSurface(txt_surface, NULL, surface, &txt_rect);

    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w,
            surface->h,
            0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
}
