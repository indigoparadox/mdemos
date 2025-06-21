
#ifndef DEMOS_H
#define DEMOS_H

#include <maug.h>
#include <retroflt.h>
#include <retrocfg.h>
#include <retrofnt.h>
#include <retrogui.h>
#include <retrowin.h>
#include <retrocon.h>
#include <retroani.h>

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

#define ANIMATIONS_MAX 5

#define DEMOS_PI 3.14159

#define DEMOS_PI_4 (DEMOS_PI / 4)

/* Convert Pico-8 "turns" into radians. */
#define psin( t ) sin( ((t * 6.28318) / 1.0f) )
#define pcos( t ) cos( ((t * 6.28318) / 1.0f) )

struct BASE_DATA {
   int init;
   MAUG_MHANDLE font_h;
   struct RETROCON con;
};

struct SINE_DATA {
   int init;
   MAUG_MHANDLE font_h;
   struct RETROCON con;
   retroflat_pxxy_t x_iter;
};

struct SPHERE_DATA {
   int init;
   MAUG_MHANDLE font_h;
   struct RETROCON con;
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
   MAUG_MHANDLE font_h;
   struct RETROCON con;
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
   MAUG_MHANDLE font_h;
   struct RETROCON con;
   float facing;
   uint8_t* map;
   float plane_dist;
   float ray_inc;
   int pos_x;
   int pos_y;
};

struct PRIMATIVES_DATA {
   int init;
   MAUG_MHANDLE font_h;
   struct RETROCON con;
   float rotate;
};

struct RETROANI_DATA {
   int init;
   MAUG_MHANDLE font_h;
   struct RETROCON con;
   struct MDATA_VECTOR animations;
};

struct SOFTCUBE_DATA {
   int init;
   MAUG_MHANDLE font_h;
   struct RETROCON con;
};

union MDEMOS_DATA {
   struct BASE_DATA base;
   struct SINE_DATA sine;
   struct SPHERE_DATA sphere;
   struct STARLINE_DATA starline;
   struct RAYCAST_DATA raycast;
   struct PRIMATIVES_DATA primatives;
   struct RETROANI_DATA retroani;
   struct SOFTCUBE_DATA softcube;
};

#define DEMOS_LIST( f ) \
   f( sine, struct SINE_DATA ) \
   f( sphere, struct SPHERE_DATA ) \
   f( starlines, struct STARLINE_DATA ) \
   f( primatives, struct PRIMATIVES_DATA ) \
   f( raycast, struct RAYCAST_DATA ) \
   f( retroani, struct RETROANI_DATA ) \
   f( softcube, struct SOFTCUBE_DATA )

#define DEMOS_LIST_PROTOS( name, data_struct ) \
   void draw_ ## name ## _iter( data_struct* data );

DEMOS_LIST( DEMOS_LIST_PROTOS )

void demos_draw_fps( void* v_data );
void demos_draw_timer( void* v_data );

#ifdef DEMOS_C

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

