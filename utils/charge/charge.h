#ifndef _CHARGE_H_
#define _CHARGE_H_

#include "../vec2/vec2.h"

typedef struct
{
  double q;
  vec2 pos;
} charge_t;

bool compute_e(charge_t c, vec2 p, double eps, vec2 *e);

bool compute_total_normalized_e(charge_t *charges, int num_charges, vec2 p, double eps, vec2 *e);

static bool draw_field_line(struct gfx_context_t *ctxt, charge_t *charges,int num_charges, double dx, vec2 pos0, double x0, double x1, double y0, double y1);

static void draw_charges(struct gfx_context_t *context, charge_t *charges,int num_charges, double x0, double x1, double y0, double y1);