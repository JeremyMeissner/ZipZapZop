/// @file gfx.c
/// @author Florent Gluck
/// @date November 6, 2016
/// Helper routines to render pixels in fullscreen graphic mode.
/// Uses the SDL2 library.

#include "gfx.h"
#include <assert.h>
#include "../vec2/vec2.h"

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
SDL_Keycode gfx_keypressed(int *x, int *y)
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            return event.key.keysym.sym;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            *x = event.button.x;
            *y = event.button.y;
            return SDL_MOUSEBUTTONDOWN;
        }
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

void draw_rectangle(struct gfx_context_t *ctxt, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color)
{
    for (uint32_t i = 0; i < width; i++)
    {
        for (uint32_t j = 0; j < height; j++)
        {
            gfx_putpixel(ctxt, x + i, y + j, color);
        }
    }
}

// Those are illegal to use in the final product

void draw_circle(struct gfx_context_t *ctxt, uint32_t c_column, uint32_t c_row, uint32_t r, uint32_t color)
{
    // Check if the graphics context pointer is valid
    if (ctxt == NULL)
    {
        printf("Error: Invalid graphics context pointer\n");
        return;
    }

    // Midpoint circle algorithm
    int x = r;
    int y = 0;
    int err = 0;

    while (x >= y)
    {
        gfx_putpixel(ctxt, c_column + x, c_row + y, color);
        gfx_putpixel(ctxt, c_column + y, c_row + x, color);
        gfx_putpixel(ctxt, c_column - y, c_row + x, color);
        gfx_putpixel(ctxt, c_column - x, c_row + y, color);
        gfx_putpixel(ctxt, c_column - x, c_row - y, color);
        gfx_putpixel(ctxt, c_column - y, c_row - x, color);
        gfx_putpixel(ctxt, c_column + y, c_row - x, color);
        gfx_putpixel(ctxt, c_column + x, c_row - y, color);

        if (err <= 0)
        {
            y += 1;
            err += 2 * y + 1;
        }
        if (err > 0)
        {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

int truncateFloat(float number)
{
    int integerPart = (int)number;
    return integerPart;
}

void draw_line(struct gfx_context_t *ctxt, int x0, int y0, int x1, int y1, uint32_t color)
{
    vec2 start_point = vec2_create(0, 0);
    vec2 end_point = vec2_create(0, 0);
    if (x0 < x1)
    {
        start_point = vec2_create(x0, y0);
        end_point = vec2_create(x1, y1);
    }
    else
    {
        start_point = vec2_create(x1, y1);
        end_point = vec2_create(x0, y0);
    }

    gfx_putpixel(ctxt, start_point.x, start_point.y, color);
    gfx_putpixel(ctxt, end_point.x, end_point.y, color);

    // This calculate how much we need to increase the y value for each x step
    double xDiff = (end_point.x - start_point.x);
    double yDiff = (end_point.y - start_point.y);

    double step = yDiff / xDiff;
    if (xDiff == 0)
        step = yDiff;

    int currentX = start_point.x;
    double currentY = start_point.y;

    for (int i = 0; i <= xDiff; i++)
    {
        int heightToDraw = abs(truncateFloat(step));
        for (int j = 0; j < heightToDraw; j++)
        {
            if (step < 0)
            {
                currentY--;
            }
            else
            {
                currentY++;
            }
            gfx_putpixel(ctxt, currentX, truncateFloat(currentY), color);
        }

        currentY += step - truncateFloat(step);
        if (xDiff != 0)
            currentX++;
        gfx_putpixel(ctxt, currentX, truncateFloat(currentY), color);
    }
}