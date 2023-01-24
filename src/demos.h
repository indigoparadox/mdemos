
#ifndef DEMOS_H
#define DEMOS_H

#include <retroflt.h>
#include <retrodep.h>

#define STATUS_SZ 255

#define SINE_AMPLIFIER 5

#define STARLINE_RADIUS_MIN 10
#define STARLINE_RADIUS_MAX (retroflat_screen_h())
#define STARLINE_ANG_INC 0.01f
#define STARLINE_START_AT 10000
#define STARLINE_FLICKER_ODDS 1000

#define STARLINES_SZ 200

#define RAYMAP_W 6
#define RAYMAP_H 6

#define RAYCAST_MINI_CX 50
#define RAYCAST_MINI_CY 30
#define RAYCAST_PLANE_DIST 25
#define RAYCAST_FOV 90

#define SPHERE_RADIUS 40

#define PI 3.14159

/* Convert Pico-8 "turns" into radians. */
#define psin( t ) sin( ((t * 6.28318) / 1.0f) )
#define pcos( t ) cos( ((t * 6.28318) / 1.0f) )

struct STARLINE {
   double ang_min;
   double ang_max;
   double ang_inc;
   int flicker;
   int flicker_odd;
   int radius;
   RETROFLAT_COLOR color;
};

struct RAYCAST_DATA {
   uint8_t* map;
   float plane_dist;
};

void draw_sine_iter( void* data );
void draw_sphere_iter( void* data );
void draw_starlines_iter( void* data );
void draw_raycast_iter( struct RAYCAST_DATA* data );

#ifdef DEMOS_C

const uint8_t gc_raymap[RAYMAP_W][RAYMAP_H] = {
   { 5, 5, 5, 5, 5, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 5, 0, 0, 0, 5 },
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
   (retroflat_loop_iter)draw_raycast_iter,
   NULL
};

size_t gc_demo_data_sz[] = {
   0,
   0,
   0,
   sizeof( struct RAYCAST_DATA ),
   0
};

int g_timer = 0;

#else

extern const char* gc_demo_names[];
extern retroflat_loop_iter gc_demo_loops[];
extern int g_timer;
extern size_t gc_demo_data_sz[];

#endif

#endif /* !DEMOS_H */

