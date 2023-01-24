
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
#define RAYCAST_WALL_H 100
#define RAYCAST_MINIMAP_SCALE 10

#define SPHERE_RADIUS 40

#define PI 3.14159

/* Convert Pico-8 "turns" into radians. */
#define psin( t ) sin( ((t * 6.28318) / 1.0f) )
#define pcos( t ) cos( ((t * 6.28318) / 1.0f) )

struct SPHERE_DATA {
   int init;
   RETROFLAT_COLOR color;
   int pulse;
   int x_v;
   int y_v;
   int x_c;
   int y_c;
   float m;
};

struct STARLINE_DATA {
   int init;
   uint32_t start_at;
   double ang_min[STARLINES_SZ];
   double ang_max[STARLINES_SZ];
   double ang_inc[STARLINES_SZ];
   int flicker[STARLINES_SZ];
   int flicker_odd[STARLINES_SZ];
   int radius[STARLINES_SZ];
   RETROFLAT_COLOR color[STARLINES_SZ];
};

struct RAYCAST_DATA {
   int init;
   uint8_t* map;
   float plane_dist;
   float ray_inc;
   int pos_x;
   int pos_y;
};

void draw_sine_iter( void* data );
void draw_sphere_iter( struct SPHERE_DATA* data );
void draw_starlines_iter( struct STARLINE_DATA* data );
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
   (retroflat_loop_iter)draw_sine_iter,
   (retroflat_loop_iter)draw_sphere_iter,
   (retroflat_loop_iter)draw_starlines_iter,
   (retroflat_loop_iter)draw_raycast_iter,
   NULL
};

size_t gc_demo_data_sz[] = {
   0,
   sizeof( struct SPHERE_DATA ),
   sizeof( struct STARLINE_DATA ),
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

