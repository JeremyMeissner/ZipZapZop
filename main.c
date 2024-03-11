#include <stdint.h>

#include "utils/utils.h"

#include "utils/gfx/gfx.h"

#include "utils/vec2/vec2.h"

typedef struct
{
    uint32_t row;
    uint32_t column;
} caca;

void gfx_draw_line(struct gfx_context_t *ctxt, caca p0, caca p1, uint32_t color);

void gfx_draw_circle(struct gfx_context_t *ctxt, caca c, uint32_t r, uint32_t color);