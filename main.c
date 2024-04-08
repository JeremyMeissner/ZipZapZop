#include <stdlib.h> // For dynamic memory allocation
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#include "utils/utils.h"
#include "utils/charge/charge.h"
#include "utils/gfx/gfx.h"
#include "utils/vec2/vec2.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

int main()
{
    srand(time(NULL));
    struct gfx_context_t *ctxt = gfx_create("Zip Zap Zop", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!ctxt)
    {
        fprintf(stderr, "Graphics initialization failed!\n");
        return EXIT_FAILURE;
    }

    charge_t *charges = malloc(0 * sizeof(charge_t)); // Initially allocate memory for 2 charges
    int number_of_charges = 0;

    int field_lines_array_precision = 11; // higher leads to worse performances to the square of the number
    double vertical_unit = SCREEN_HEIGHT / field_lines_array_precision;
    double horizontal_unit = SCREEN_WIDTH / field_lines_array_precision;

    bool mode_is_negative = true;

    SDL_Keycode pressedKey;

    int *x = malloc(sizeof(int));
    int *y = malloc(sizeof(int));

    bool is_paused = true;

    while (true)
    {
        gfx_present(ctxt);
        gfx_clear(ctxt, COLOR_WHITE);

        pressedKey = gfx_keypressed(x, y);

        if (pressedKey == 27)
        { // Assuming 27 is the key code for Escape
            printf("Shutting down the app\n");
            break;
        }
        else
        {
            if (pressedKey != SDLK_UNKNOWN && pressedKey != 27)
            {
                switch (pressedKey)
                {
                case SDLK_s:
                    mode_is_negative = !mode_is_negative;
                    break;
                case SDLK_SPACE:
                    is_paused = !is_paused;
                    break;
                case SDLK_r:

                    number_of_charges = 0;
                    charges = realloc(charges, 0 * sizeof(charge_t));
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    charges = realloc(charges, (number_of_charges + 1) * sizeof(charge_t));

                    double charge_value = mode_is_negative ? 1 : -1;
                    charge_value = charge_value * (rand() % 2 + 1);

                    charges[number_of_charges] = charge_create(charge_value, vec2_create(*x, *y));
                    number_of_charges++;
                    break;
                }
            }
        }

        if (is_paused)
        {
            // Add fluctuation to the charges
            for (int i = 0; i < number_of_charges; i++)
            {
                charges[i].q += ((rand() % 2000) - 1000.0) / 1000000.0;
            }
        }
        else
        {
            update_charges(charges, number_of_charges, 0.000001);
        }

        // DRAW
        for (int y = 0; y < field_lines_array_precision; y++)
            for (int x = 0; x < field_lines_array_precision; x++)
                draw_field_lines(ctxt, charges, number_of_charges, 0.088, vec2_create(horizontal_unit * x, vertical_unit * y), 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

        draw_charges(ctxt, charges, number_of_charges, 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

        // draw circle on top right according to the mode
        draw_full_circle(ctxt, SCREEN_WIDTH - 20, 20, 10, mode_is_negative ? MAKE_COLOR(0, 0, 255) : MAKE_COLOR(255, 0, 0));
        if (mode_is_negative)
        {
            draw_line(ctxt, SCREEN_WIDTH - 20 - 5, 20, SCREEN_WIDTH - 20 + 5, 20, MAKE_COLOR(0, 0, 0));
        }
        else
        {
            draw_line(ctxt, SCREEN_WIDTH - 20, 20 - 5, SCREEN_WIDTH - 20, 20 + 5, MAKE_COLOR(0, 0, 0));
            draw_line(ctxt, SCREEN_WIDTH - 20 - 5, 20, SCREEN_WIDTH - 20 + 5, 20, MAKE_COLOR(0, 0, 0));
        }
    }

    free(charges); // Don't forget to free the dynamically allocated memory
    gfx_destroy(ctxt);
    return EXIT_SUCCESS;
}
