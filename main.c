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

        draw_line(ctxt, 0, 0, SCREEN_HEIGHT, SCREEN_HEIGHT, MAKE_COLOR(255, 255, 255));
    }

    gfx_destroy(ctxt);
    return EXIT_SUCCESS;
}
