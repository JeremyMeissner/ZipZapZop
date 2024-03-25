#include <stdint.h>

#include "utils/utils.h"
#include "utils/charge/charge.h"

#include "utils/gfx/gfx.h"

#include "utils/vec2/vec2.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

int main()
{
    srand(time(NULL));
    struct gfx_context_t *ctxt =
        gfx_create("Zip Zap Zop", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!ctxt)
    {
        fprintf(stderr, "Graphics initialization failed!\n");
        return EXIT_FAILURE;
    }

    /* DEBUG PLAYGROUND START */

    // You can put stuff with printf's here before the simulation starts

    charge_t ch1 = charge_create(5, vec2_create(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4));
    charge_t ch2 = charge_create(-5, vec2_create(3 * SCREEN_WIDTH / 4, 3 * SCREEN_HEIGHT / 4));

    charge_t charges[] = {ch1, ch2};
    int number_of_charges = 2;

    /* DEBUG PLAYGROUND END */

    while (true)
    {
        gfx_present(ctxt);
        gfx_clear(ctxt, COLOR_WHITE);

        int pressedKey = gfx_keypressed();
        if (pressedKey == 27)
        { // Assuming 27 is the key code for Escape
            printf("Shutting down the app\n");
            break;
        }

        if (pressedKey != SDLK_UNKNOWN && pressedKey != 27)
        {
            switch (pressedKey)
            {
            // Add more cases for other keys as needed
            default:
                printf("Unhandled key pressed\n");
                break;
            }
        }

        draw_charges(ctxt, charges, number_of_charges, 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

        draw_field_line(ctxt, charges, number_of_charges, 0.01, vec2_create(SCREEN_WIDTH / 4.2, SCREEN_HEIGHT / 2.5), 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
        // draw_field_line(ctxt, charges, number_of_charges, 0.0001, vec2_create(SCREEN_WIDTH / 2.4, SCREEN_HEIGHT / 1.5), 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
        // draw_field_line(ctxt, charges, number_of_charges, 0.0001, vec2_create(SCREEN_WIDTH / 3.7, SCREEN_HEIGHT / 2.2), 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
        // draw_field_line(ctxt, charges, number_of_charges, 0.0001, vec2_create(SCREEN_WIDTH / 4.9, SCREEN_HEIGHT / 7), 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

        /*DISPLAY START*/
        // draw_line(ctxt, 0, 0, SCREEN_HEIGHT, SCREEN_HEIGHT, MAKE_COLOR(255, 255, 255));
        // draw_circle(ctxt, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 10, MAKE_COLOR(125, 125, 125));
        /*DISPLAY END*/
    }
    gfx_destroy(ctxt);
    return EXIT_SUCCESS;
}
