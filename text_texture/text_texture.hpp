#ifndef TEXT_TEXTURE_H
#define TEXT_TEXTURE_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <cairo.h>
#include <pango/pangocairo.h>
#include <string.h>

enum {TEXTLENGTH = 80}; // Max amount of characters (including null
                        // character) that can be written on texture

class TextTexture {
    public:
        SDL_Rect txt_rect; // SDL Rect describing the position of
                           // text relative to bg. If there is no
                           // bg just describes the width and height
                           // of the texture
        SDL_Surface *bg_img; // Background image of surface
        SDL_Surface *surface; // SDL Surface that bg img and text 
                              // is blitted onto prior to display
        SDL_Surface *txt_surface; // SDL Surface text is drawn onto
        char text[TEXTLENGTH];

        cairo_surface_t *cairo_surface;
        cairo_t *cr;
        PangoLayout *layout;
        PangoFontDescription *font_description;

        GLuint texture_id; // Handle to openGL texture that will contain
                           // this text

        TextTexture(SDL_Rect txt_rect, SDL_Surface *bg_img = NULL);
        void set_text(const char *text_input);
};

#endif
