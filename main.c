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

    charge_t *charges = malloc(2 * sizeof(charge_t)); // Initially allocate memory for 2 charges
    int number_of_charges = 2;

    charges[0] = charge_create(5, vec2_create(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4));
    charges[1] = charge_create(-5, vec2_create(3 * SCREEN_WIDTH / 4, 3 * SCREEN_HEIGHT / 4));

    while (true)
    {
        gfx_present(ctxt);
        gfx_clear(ctxt, COLOR_WHITE);

        int *x = malloc(sizeof(int));
        int *y = malloc(sizeof(int));

        if (gfx_mouseclicked(x, y))
        {
            charges = realloc(charges, (number_of_charges + 1) * sizeof(charge_t));
            charges[number_of_charges] = charge_create((rand() % 11) - 5, // Random magnitude between -5 and 5
                                                       vec2_create(*x, *y));
            number_of_charges++;
        }

        if (gfx_keypressed() == 27)
        { // Assuming 27 is the key code for Escape
            printf("Shutting down the app\n");
            break;
        }

        draw_charges(ctxt, charges, number_of_charges, 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

        for (int i = 0; i < 50; i++)
            draw_field_lines(ctxt, charges, number_of_charges, 0.1, vec2_create(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT), 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    }

    free(charges); // Don't forget to free the dynamically allocated memory
    gfx_destroy(ctxt);
    return EXIT_SUCCESS;
}
