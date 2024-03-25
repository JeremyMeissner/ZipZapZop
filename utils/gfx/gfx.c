/// @file gfx.c
/// @author Florent Gluck
/// @date November 6, 2016
/// Helper routines to render pixels in fullscreen graphic mode.
/// Uses the SDL2 library.

#include "gfx.h"
#include <assert.h>

/// Create a fullscreen graphic window.
/// @param title Title of the window.
/// @param width Width of the window in pixels.
/// @param height Height of the window in pixels.
/// @return a pointer to the graphic context or NULL if it failed.
struct gfx_context_t *gfx_create(char *title, uint32_t width, uint32_t height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        goto error;
    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING, width, height);
    uint32_t *pixels = malloc(width * height * sizeof(uint32_t));
    struct gfx_context_t *ctxt = malloc(sizeof(struct gfx_context_t));

    if (!window || !renderer || !texture || !pixels || !ctxt)
        goto error;

    ctxt->renderer = renderer;
    ctxt->texture = texture;
    ctxt->window = window;
    ctxt->width = width;
    ctxt->height = height;
    ctxt->pixels = pixels;

    SDL_ShowCursor(SDL_DISABLE);
    gfx_clear(ctxt, COLOR_BLACK);

    SDL_ShowCursor(SDL_ENABLE);
    return ctxt;

error:
    return NULL;
}

/// Draw a pixel in the specified graphic context.
/// @param ctxt Graphic context where the pixel is to be drawn.
/// @param column X coordinate of the pixel.
/// @param row Y coordinate of the pixel.
/// @param color Color of the pixel.
void gfx_putpixel(struct gfx_context_t *ctxt, uint32_t column, uint32_t row, uint32_t color)
{
    if (column < ctxt->width && row < ctxt->height)
        ctxt->pixels[ctxt->width * row + column] = color;
}

/// Clear the specified graphic context.
/// @param ctxt Graphic context to clear.
/// @param color Color to use.
void gfx_clear(struct gfx_context_t *ctxt, uint32_t color)
{
    int n = ctxt->width * ctxt->height;
    while (n)
        ctxt->pixels[--n] = color;
}

/// Display the graphic context.
/// @param ctxt Graphic context to clear.
void gfx_present(struct gfx_context_t *ctxt)
{
    SDL_UpdateTexture(
        ctxt->texture, NULL, ctxt->pixels, ctxt->width * sizeof(uint32_t));
    SDL_RenderCopy(ctxt->renderer, ctxt->texture, NULL, NULL);
    SDL_RenderPresent(ctxt->renderer);
}

/// Destroy a graphic window.
/// @param ctxt Graphic context of the window to close.
void gfx_destroy(struct gfx_context_t *ctxt)
{
    SDL_ShowCursor(SDL_ENABLE);
    SDL_DestroyTexture(ctxt->texture);
    SDL_DestroyRenderer(ctxt->renderer);
    SDL_DestroyWindow(ctxt->window);
    free(ctxt->pixels);
    ctxt->texture = NULL;
    ctxt->renderer = NULL;
    ctxt->window = NULL;
    ctxt->pixels = NULL;
    SDL_Quit();
    free(ctxt);
}

/// If a key was pressed, returns its key code (non blocking call).
/// List of key codes: https://wiki.libsdl.org/SDL_Keycode
/// @return the key that was pressed or 0 if none was pressed.
SDL_Keycode gfx_keypressed()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
            return event.key.keysym.sym;
    }
    return 0;
}

// If the mouse was clicked, returns the x and y coordinates of the click.
// @param x Pointer to the variable where the x coordinate will be stored.
// @param y Pointer to the variable where the y coordinate will be stored.
// @return true if the mouse was clicked, false otherwise.
bool gfx_mouseclicked(int *x, int *y)
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            *x = event.button.x;
            *y = event.button.y;
            return true;
        }
    }
    return false;
}

void gfx_draw_line(struct gfx_context_t *ctxt, coordinates_t p0, coordinates_t p1, uint32_t color)
{
    // Todo
}
void gfx_draw_circle(struct gfx_context_t *ctxt, coordinates_t c, uint32_t r, uint32_t color)
{
    // Todo
}

/// Draw a full circle using Andres's discrete circle algorithm.
/// @param ctxt Graphic context to clear.
/// @param c_column X coordinate of the circle center.
/// @param c_row Y coordinate of the circle center.
/// @param r The radius of circle (in pixels).
/// @param color Color to use.
void draw_full_circle(struct gfx_context_t *ctxt, uint32_t c_column, uint32_t c_row, uint32_t r, uint32_t color)
{
    int32_t x = 0, y = r, d = r - 1;
    while (y >= x)
    {
        gfx_putpixel(ctxt, c_column + x, c_row + y, color);
        gfx_putpixel(ctxt, c_column + y, c_row + x, color);
        gfx_putpixel(ctxt, c_column - x, c_row + y, color);
        gfx_putpixel(ctxt, c_column - y, c_row + x, color);

        gfx_putpixel(ctxt, c_column + x, c_row - y, color);
        gfx_putpixel(ctxt, c_column + y, c_row - x, color);
        gfx_putpixel(ctxt, c_column - x, c_row - y, color);
        gfx_putpixel(ctxt, c_column - y, c_row - x, color);

        if ((2 * x) <= d)
        {
            d -= 2 * x + 1;
            x += 1;
        }
        else if (d < (2 * (((int32_t)r) - y)))
        {
            d += 2 * y - 1;
            y -= 1;
        }
        else
        {
            d -= 2 * (x - y + 1);
            y -= 1;
            x += 1;
        }
    }
    if (r > 0)
        draw_full_circle(ctxt, c_column, c_row, r - 1, color);
}

// Those are illegal to use in the final product

void draw_circle(struct gfx_context_t *ctxt, uint32_t c_column, uint32_t c_row, uint32_t r, uint32_t color)
{
    int32_t x = 0, y = r, d = r - 1;
    while (y >= x)
    {
        gfx_putpixel(ctxt, c_column + x, c_row + y, color);
        gfx_putpixel(ctxt, c_column + y, c_row + x, color);
        gfx_putpixel(ctxt, c_column - x, c_row + y, color);
        gfx_putpixel(ctxt, c_column - y, c_row + x, color);

        gfx_putpixel(ctxt, c_column + x, c_row - y, color);
        gfx_putpixel(ctxt, c_column + y, c_row - x, color);
        gfx_putpixel(ctxt, c_column - x, c_row - y, color);
        gfx_putpixel(ctxt, c_column - y, c_row - x, color);

        if ((2 * x) <= d)
        {
            d -= 2 * x + 1;
            x += 1;
        }
        else if (d < (2 * (((int32_t)r) - y)))
        {
            d += 2 * y - 1;
            y -= 1;
        }
        else
        {
            d -= 2 * (x - y + 1);
            y -= 1;
            x += 1;
        }
    }
}

void draw_line(struct gfx_context_t *ctxt, int x0, int y0, int x1, int y1, uint32_t color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    while (1)
    {
        gfx_putpixel(ctxt, x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        { /* e_xy+e_x > 0 */
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        { /* e_xy+e_y < 0 */
            err += dx;
            y0 += sy;
        }
    }
}