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

    charges[0] = charge_create(1, vec2_create(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4));
    charges[1] = charge_create(-1, vec2_create(3 * SCREEN_WIDTH / 4, 3 * SCREEN_HEIGHT / 4));

    int field_lines_array_precision = 10; //higher leads to worse performances to the square of the number
    double vertical_unit = SCREEN_HEIGHT / field_lines_array_precision;
    double horizontal_unit = SCREEN_WIDTH / field_lines_array_precision;

    while (true)
    {
        gfx_present(ctxt);
        gfx_clear(ctxt, COLOR_WHITE);

        int *x = malloc(sizeof(int));
        int *y = malloc(sizeof(int));

        if (gfx_mouseclicked(x, y))
        {
            charges = realloc(charges, (number_of_charges + 1) * sizeof(charge_t));
            charges[number_of_charges] = charge_create(((rand()%2) % 1) -1, vec2_create(*x, *y));
            number_of_charges++;
        }

        if (gfx_keypressed() == 27) 
        { // Assuming 27 is the key code for Escape
            printf("Shutting down the app\n");
            break;
        }
        //Add fluctuation to the charges
        for(int i = 0; i < number_of_charges;i ++){
           //printf("%f\n",((rand()%2000)-1000.0)/1000.0);
           charges[i].q += ((rand()%2000)-1000.0)/1000000.0;
           //printf("charge %d: %f\n",i,charges[i].q);
        }
        //printf("\n");

        //DRAW
        for(int y = 0; y < field_lines_array_precision; y++)
            for(int x = 0; x < field_lines_array_precision;x++)
                draw_field_lines(ctxt, charges, number_of_charges, 0.088, vec2_create(horizontal_unit * x,vertical_unit * y), 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);

        draw_charges(ctxt, charges, number_of_charges, 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
        
        //Debug Drawing
        /*
        draw_line(ctxt,500,250,500,750,0);
        draw_line(ctxt,250,500,750,500,0);

        draw_circle(ctxt,500,500,250,0);
        */
    }

    free(charges); // Don't forget to free the dynamically allocated memory
    gfx_destroy(ctxt);
    return EXIT_SUCCESS;
}
