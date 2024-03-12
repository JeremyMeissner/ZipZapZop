#include <stdint.h>

#include "utils/utils.h"

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

    /* DEBUG PLAYGROUND END */

    while (true)
    {
        gfx_present(ctxt);
        gfx_clear(ctxt, COLOR_BLACK);

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

        /*DISPLAY START*/
        draw_line(ctxt, 0, 0, SCREEN_HEIGHT, SCREEN_HEIGHT, MAKE_COLOR(255, 255, 255));
        draw_circle(ctxt, SCREEN_WIDTH / 2,SCREEN_HEIGHT/2,SCREEN_WIDTH / 10,MAKE_COLOR(125,125,125));
        /*DISPLAY END*/
    }
    gfx_destroy(ctxt);
    return EXIT_SUCCESS;
}
