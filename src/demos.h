
#ifndef DEMOS_H
#define DEMOS_H

#include <retroflt.h>

#define STATUS_SZ 255

#define SINE_AMPLIFIER 5

#define STARLINE_RADIUS_MIN 10
#define STARLINE_RADIUS_MAX (retroflat_screen_h())
#define STARLINE_ANG_INC 0.01f
#define STARLINE_START_AT 10000
#define STARLINE_FLICKER_ODDS 1000

#define STARLINES_SZ 200

#define RAYMAP_W 6
#define RAYMAP_H 7

#define RAYCAST_MINI_CX 50
#define RAYCAST_MINI_CY 30
#define RAYCAST_PLANE_DIST 25
#define RAYCAST_FOV 90
#define RAYCAST_WALL_H 100
#define RAYCAST_MINIMAP_SCALE 10

#define SPHERE_RADIUS 40

#define PI 3.14159

#define PI_4 (PI / 4)

/* Convert Pico-8 "turns" into radians. */
#define psin( t ) sin( ((t * 6.28318) / 1.0f) )
#define pcos( t ) cos( ((t * 6.28318) / 1.0f) )

struct SINE_DATA {
   int init;
   int x_iter;
};

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
   float facing;
   uint8_t* map;
   float plane_dist;
   float ray_inc;
   int pos_x;
   int pos_y;
};

struct PRIMATIVES_DATA {
   float rotate;
   int init;
};

#define DEMOS_LIST( f ) \
   f( sine, struct SINE_DATA ) \
   f( sphere, struct SPHERE_DATA ) \
   f( starlines, struct STARLINE_DATA ) \
   f( primatives, struct PRIMATIVES_DATA ) \
   f( raycast, struct RAYCAST_DATA )

#define DEMOS_LIST_PROTOS( name, data_struct ) \
   void draw_ ## name ## _iter( data_struct* data );

DEMOS_LIST( DEMOS_LIST_PROTOS )

#ifdef DEMOS_C

const uint8_t gc_raymap[RAYMAP_H][RAYMAP_W] = {
   { 5, 5, 5, 5, 5, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 5, 0, 0, 0, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 5, 5, 5, 5, 5 }
};

#define DEMOS_LIST_NAMES( name, data_struct ) #name,

const char* gc_demo_names[] = {
   DEMOS_LIST( DEMOS_LIST_NAMES )
   ""
};

#define DEMOS_LIST_LOOPS( name, data_struct ) \
   (retroflat_loop_iter)draw_ ## name ## _iter,

retroflat_loop_iter gc_demo_loops[] = {
   DEMOS_LIST( DEMOS_LIST_LOOPS )
   NULL
};

#define DEMOS_LIST_DATA_SZ( name, data_struct ) sizeof( data_struct ),

size_t gc_demo_data_sz[] = {
   DEMOS_LIST( DEMOS_LIST_DATA_SZ )
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

