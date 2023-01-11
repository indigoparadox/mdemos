
#ifndef DEMOS_H
#define DEMOS_H

#include <retroflt.h>

#define STATUS_SZ 255

#define SINE_AMPLIFIER 5

#define STARLINE_RADIUS 40
#define STARLINE_RAD_INC 0.1

#define STARLINES_SZ 100

struct STARLINE {
   double rad_max;
   double rad_inc;
   int radius;
   RETROFLAT_COLOR color;
};

void draw_sine_iter( void* data );
void draw_sphere_iter( void* data );
void draw_starlines_iter( void* data );

#ifdef DEMOS_C

const char* gc_demo_names[] = {
   "sine",
   "sphere",
   "starlines",
   ""
};

retroflat_loop_iter gc_demo_loops[] = {
   draw_sine_iter,
   draw_sphere_iter,
   draw_starlines_iter,
   NULL
};

#else

extern const char* gc_demo_names[];
extern retroflat_loop_iter gc_demo_loops[];

#endif

#endif /* !DEMOS_H */

