
#ifndef DEMOS_H
#define DEMOS_H

#include <retroflt.h>
#include <retrodep.h>

#define STATUS_SZ 255

#define SINE_AMPLIFIER 5

#define STARLINE_RADIUS 40
#define STARLINE_RAD_INC 0.1

#define STARLINES_SZ 100

#define RAYMAP_W 6
#define RAYMAP_H 6

/* Convert Pico-8 "turns" into radians. */
#define psin( t ) sin( ((t * 6.28318) / 1.0f) )
#define pcos( t ) cos( ((t * 6.28318) / 1.0f) )

struct STARLINE {
   double rad_max;
   double rad_inc;
   int radius;
   RETROFLAT_COLOR color;
};

struct RAYMAP {
   uint8_t* map;
};

void draw_sine_iter( void* data );
void draw_sphere_iter( void* data );
void draw_starlines_iter( void* data );
void draw_raycast_iter( void* data );

#ifdef DEMOS_C

const uint8_t gc_raymap[RAYMAP_W][RAYMAP_H] = {
   { 5, 5, 5, 5, 5, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 5, 5, 5, 5, 5 }
};

const char* gc_demo_names[] = {
   "sine",
   "sphere",
   "starlines",
   "raycast",
   ""
};

retroflat_loop_iter gc_demo_loops[] = {
   draw_sine_iter,
   draw_sphere_iter,
   draw_starlines_iter,
   draw_raycast_iter,
   NULL
};

int g_timer = 0;

#else

extern const char* gc_demo_names[];
extern retroflat_loop_iter gc_demo_loops[];
extern int g_timer;

#endif

#endif /* !DEMOS_H */

