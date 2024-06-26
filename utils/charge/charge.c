#include <math.h>
#include "charge.h"
#include "../gfx/gfx.h"

const float K = 8.9875517873681764e9;

// Compute E*qP/norm(qP)
// Return false if norm(qP) < eps
bool compute_e(charge_t c, vec2 p, double treshold, vec2 *e)
{
    vec2 qp = vec2_sub(c.pos, p);
    qp = vec2_mul(c.q, qp);

    double qpNorm = pow(vec2_norm(qp), 2);

    vec2 e_qp = vec2_mul(K, qp);

    *e = vec2_mul(pow(qpNorm, -1), e_qp);

    if (qpNorm < treshold)
    {
        return false;
    }

    return true;
}

// Compute the normalized sum of Ei*qiP/norm(qiP)
// Return false if for some qiP, norm(qiP) < epsà
bool compute_total_normalized_e(charge_t *charges, int num_charges, vec2 p, double treshold, vec2 *e)
{
    vec2 sum = vec2_create(0, 0);

    for (int i = 0; i < num_charges; i++)
    {
        vec2 ee;
        // if (!compute_e(charges[i], p, treshold, &ee))
        if (!compute_e(charges[i], p, treshold, &ee))
        {
            return false;
        }

        sum = vec2_add(sum, ee);
    }
    *e = sum;
    return true;
}

// Compute and then draw all the points belonging to a field line,
// starting from pos0.
// Returns false if pos0 is not a valid position
// (for example if pos0 is too close to a charge).
bool draw_field_line(struct gfx_context_t *ctxt, charge_t *charges, int num_charges, double dx, vec2 pos0, double x0, double x1, double y0, double y1)
{
    vec2 posSuivant = pos0;
    vec2 posNow = pos0;
    int limit_points = 10000;
    while (posSuivant.x < x1 && posSuivant.x > 0 && posSuivant.y < y1 && posSuivant.y > 0 && limit_points > 0)
    {
        vec2 e;
        limit_points--;
        if (!compute_total_normalized_e(charges, num_charges, posSuivant, 1e-3, &e))
            return false;

        posNow = posSuivant;

        double enorme = vec2_norm(e);

        vec2 delta = vec2_mul(dx, vec2_mul(pow(enorme, -1), e));

        posSuivant = vec2_add(posSuivant, delta);

        draw_full_circle(ctxt, posNow.x, posNow.y, abs(posSuivant.x - posNow.x) + 1, MAKE_COLOR(60, 60, 60));
    }

    return true;
}

bool draw_field_lines(struct gfx_context_t *ctxt, charge_t *charges, int num_charges, double dx, vec2 pos0, double x0, double x1, double y0, double y1)
{
    draw_field_line(ctxt, charges, num_charges, dx, pos0, x0, x1, y0, y1);
    draw_field_line(ctxt, charges, num_charges, -dx, pos0, x0, x1, y0, y1);
}

// Draw all the charges
// A circle with minus sign for negative charges
// A circle with a plus sign for positive charges
void draw_charges(struct gfx_context_t *context, charge_t *charges, int num_charges, double x0, double x1, double y0, double y1)
{
    for (int i = 0; i < num_charges; i++)
    {

        if (charges[i].pos.x < x0 || charges[i].pos.x > x1 || charges[i].pos.y < y0 || charges[i].pos.y > y1)
        {
            continue;
        }
        int color;
        if (charges[i].q < 0)
        {
            draw_full_circle(context, charges[i].pos.x, charges[i].pos.y, 10, MAKE_COLOR(255, 0, 0));
            draw_line(context, charges[i].pos.x, charges[i].pos.y - 5, charges[i].pos.x, charges[i].pos.y + 5, MAKE_COLOR(0, 0, 0));
            draw_line(context, charges[i].pos.x - 5, charges[i].pos.y, charges[i].pos.x + 5, charges[i].pos.y, MAKE_COLOR(0, 0, 0));
        }
        else
        {
            draw_full_circle(context, charges[i].pos.x, charges[i].pos.y, 10, MAKE_COLOR(0, 0, 255));
            draw_line(context, charges[i].pos.x - 5, charges[i].pos.y, charges[i].pos.x + 5, charges[i].pos.y, MAKE_COLOR(0, 0, 0));
        }
        // drawing a little outline
        draw_circle(context, charges[i].pos.x, charges[i].pos.y, 10 + 1, MAKE_COLOR(0, 0, 0));
    }
}

void update_charges(charge_t *charges, int num_charges, double dt)
{
    for (int i = 0; i < num_charges; i++)
    {
        vec2 f = vec2_create(0, 0);
        for (int j = 0; j < num_charges; j++)
        {
            if (i == j)
                continue;

            vec2 direction = vec2_sub(charges[j].pos, charges[i].pos);
            vec2 normalizedDirection = vec2_normalize(direction);
            double distanceSq = pow(vec2_norm(direction), 2);

            if (distanceSq < 1e-3)
                distanceSq = 1e-3;

            double magnitude = K * fabs(charges[i].q) * fabs(charges[j].q) / distanceSq;

            vec2 force = vec2_mul(magnitude, normalizedDirection);
            if (charges[i].q * charges[j].q > 0)
            {
                f = vec2_sub(f, force);
            }
            else
            {
                f = vec2_add(f, force);
            }
        }

        charges[i].pos = vec2_add(charges[i].pos, vec2_mul(dt, f));
    }
}

// void update_charges(charge_t *charges, int num_charges, double dt)
// {
//     for (int i = 0; i < num_charges; i++)
//     {
//         vec2 f = vec2_create(0, 0);
//         vec2 e = vec2_create(0, 0);
//         for (int j = 0; j < num_charges; j++)
//         {
//             if (i == j)
//                 continue;

//             if (!compute_e(charges[j], charges[i].pos, 1e-3, &e))
//             {
//                 continue;
//             }

//             vec2 direction = vec2_sub(charges[i].pos, charges[j].pos);
//             double distanceSq = pow(vec2_norm(direction), 2);
//             vec2 repulsionForce = vec2_mul(K * charges[i].q * charges[j].q / distanceSq, vec2_normalize(direction));
//         }

//         if (charges[i].q > 0)
//         {
//             f = vec2_sub(f, e);
//         }
//         else
//         {
//             f = vec2_add(f, e);
//         }

//         charges[i].pos = vec2_add(charges[i].pos, vec2_mul(dt, f));
//     }
// }

charge_t charge_create(double q, vec2 pos)
{
    charge_t c = {.q = q, .pos = pos};
    return c;
}