
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define DEMOS_C
#include "demos.h"

static void demos_draw_timer() {
   char status[STATUS_SZ] = { 0 };

   if( g_timer ) {
      maug_snprintf( status, STATUS_SZ, "%lu", retroflat_get_ms() );
      retroflat_string( NULL, RETROFLAT_COLOR_WHITE, status, 0, NULL, 0, 0, 0 );
   }
}

void draw_sine_iter( void* data ) {
   struct RETROFLAT_INPUT input_evt;
   static int x_iter = 0;
   int x = 0,
      x_prev = 0,
      input = 0;
   double y_offset = 0;

   input = retroflat_poll_input( &input_evt );

   switch( input ) {
   case RETROFLAT_KEY_ESC:
      retroflat_quit( 0 );
      break;
   }

   /* Drawing */

   retroflat_draw_lock( NULL );

   retroflat_rect(
      NULL, RETROFLAT_COLOR_BLACK, 0, 0,
      retroflat_screen_w(), retroflat_screen_h(),
      RETROFLAT_FLAGS_FILL );

   /* Draw lines up to the current x_iter with each pixel as X. */
   for( x = 1 ; x_iter > x ; x++ ) {
      x_prev = x - 1;

      retroflat_line(
         NULL, RETROFLAT_COLOR_WHITE,
         x_prev, (retroflat_screen_h() / 2) + (sin( x_prev ) * SINE_AMPLIFIER),
         x, (retroflat_screen_h() / 2) + (sin( x ) * SINE_AMPLIFIER),
         0 );

      /* As the x coordinate increases, the y_offset goes down then up again! */
      y_offset = 100.0f * sin( x / (double)(retroflat_screen_w() / 3) );

      /* A single, screen-wide big arc made of a tinier sine wave. */
      retroflat_line(
         NULL, RETROFLAT_COLOR_RED,
         x_prev, y_offset + (sin( x_prev ) * SINE_AMPLIFIER),
         x, y_offset + (sin( x ) * SINE_AMPLIFIER), 0 );
   }

   demos_draw_timer();

   retroflat_draw_release( NULL );

   x_iter++;
   if( 320 <= x_iter ) {
      x_iter = 0;
   }
}

void draw_sphere_iter( void* data ) {
   static float m = 0;
   double x = 0,
      y = 0,
      ir = 0,
      or = 0;
   struct RETROFLAT_INPUT input_evt;
   int input = 0;
   RETROFLAT_COLOR color;
   static int x_v = 4;
   static int y_v = 4;
   static int x_c = SPHERE_RADIUS + 1;
   static int y_c = SPHERE_RADIUS + 1;

   input = retroflat_poll_input( &input_evt );

   assert( 0 == retroflat_screen_w() % x_v );
   assert( 0 == retroflat_screen_h() % y_v );

   switch( input ) {
   case RETROFLAT_KEY_ESC:
      retroflat_quit( 0 );
      break;
   }

   /* Drawing */

   retroflat_draw_lock( NULL );

   retroflat_rect(
      NULL, RETROFLAT_COLOR_BLACK, 0, 0,
      retroflat_screen_w(), retroflat_screen_h(),
      RETROFLAT_FLAGS_FILL );

   x_c += x_v;
   if( 
      x_c + SPHERE_RADIUS + x_v >= retroflat_screen_w() ||
      x_c - SPHERE_RADIUS + x_v < 0
   ) {
      x_v *= -1;
   }

   y_c += y_v;
   if(
      y_c + SPHERE_RADIUS + y_v >= retroflat_screen_h() ||
      y_c - SPHERE_RADIUS + y_v < 0
   ) {
      y_v *= -1;
   }

   for( or = 0 ; 2 * PI > or ; or += 0.4 ) {
      for( ir = 0 ; 2 * PI > ir ; ir += 0.4 ) {
         /* Multiple by sin( ir - m ) on X to give a cyclicle leftward(?)
          * bias to the inner circles, creating sphere illusion.
          */
         x = (double)x_c + (cos( or ) * sin( ir - m ) * SPHERE_RADIUS);
         y = (double)y_c + (sin( or ) * SPHERE_RADIUS);

         color = or + ir + (rand() % 3) > 8 ?
            RETROFLAT_COLOR_WHITE : RETROFLAT_COLOR_GRAY;

         retroflat_rect(
            NULL, color, x, y, 1, 1,
            RETROFLAT_FLAGS_FILL );
      }
   }

   demos_draw_timer();

   retroflat_draw_release( NULL );

   /* Increment the rotation for each frame. sin( mod 0.3 ) "jumps,"
    * so sin( 0.4 ) seems to be the sweet spot.
    */
   m = .0126f + fmod( m, 0.4f );
}

void create_starline( struct STARLINE* starline ) {
   starline->ang_inc = STARLINE_ANG_INC;
   starline->radius = STARLINE_RADIUS_MIN + (rand() % STARLINE_RADIUS_MAX);
   starline->ang_min = fmod( (rand() * 0.1), PI ) 
      - PI; /* Make angle negative since we're facing up. */
   starline->ang_max = starline->ang_min;
   starline->flicker = rand() % STARLINE_FLICKER_ODDS;
   starline->flicker_odd = rand() % 2;
   switch( rand() % 2 ) {
   case 0:
      starline->color = RETROFLAT_COLOR_WHITE;
      break;
   case 1:
      starline->color = RETROFLAT_COLOR_CYAN;
      break;
   }
}

void draw_starlines_iter( void* data ) {
   int x = 0,
      y = 0,
      x_prev = 0,
      y_prev = 0,
      i = 0;
   struct RETROFLAT_INPUT input_evt;
   int input = 0;
   double angle = 0;
   static struct STARLINE starlines[STARLINES_SZ];
   static int init = 0;
   static uint32_t start_at = 0;

   if( !init ) {
      /* Do initial setup. */
      memset( starlines, '\0', STARLINES_SZ * sizeof( struct STARLINE ) );

      for( i = 0 ; STARLINES_SZ > i ; i++ ) {
         create_starline( &(starlines[i]) );
      }

      start_at = retroflat_get_ms() + STARLINE_START_AT;

      init = 1;
   }

   input = retroflat_poll_input( &input_evt );

   switch( input ) {
   case RETROFLAT_KEY_ESC:
      retroflat_quit( 0 );
      break;
   }

   /* Drawing */

   retroflat_draw_lock( NULL );

   retroflat_rect(
      NULL, RETROFLAT_COLOR_BLACK, 0, 0,
      retroflat_screen_w(), retroflat_screen_h(),
      RETROFLAT_FLAGS_FILL );

   for( i = 0 ; STARLINES_SZ > i ; i++ ) {

      /* Draw initial star. */
      x = (retroflat_screen_w() / 2) /* Center on X. */
         + cos( starlines[i].ang_min ) * starlines[i].radius;
      y = retroflat_screen_h() /* Y at the bottom of the screen. */
         + sin( starlines[i].ang_min ) * starlines[i].radius;

      if( 
         /* Don't draw stars after spinning starts. */
         start_at >= retroflat_get_ms() &&
         /* Flicker some stars. */
         (10 < starlines[i].flicker || 
         starlines[i].flicker_odd == retroflat_get_ms() % 2)
      ) {
         retroflat_rect(
            NULL, starlines[i].color, x, y, 1, 1, RETROFLAT_FLAGS_FILL );
      }

      /* Iterate the points on an arc for each starline. */
      for(
         angle = starlines[i].ang_min ;
         starlines[i].ang_max > angle ;
         angle += starlines[i].ang_inc
      ) {
         /* Calculate previous x/y of a point in the starline using cos/sin. */
         x_prev =
            (retroflat_screen_w() / 2) + cos( angle - starlines[i].ang_inc )
            * starlines[i].radius;
         y_prev = retroflat_screen_h() + sin( angle - starlines[i].ang_inc )
            * starlines[i].radius;

         /* Calculate x/y of a point in the starline using cos/sin. */
         x = (retroflat_screen_w() / 2) /* Center on X. */
            + cos( angle ) * starlines[i].radius;
         y = retroflat_screen_h() /* Y at the bottom of the screen. */
            + sin( angle ) * starlines[i].radius;

         /* Draw the line from prev point to current point. */
         retroflat_line( NULL, starlines[i].color, x_prev, y_prev, x, y, 0 );
      }

      /* Increment radians if not a full circle yet. */
      if(
         start_at < retroflat_get_ms() &&
         /* 2PI is a full circle. */
         starlines[i].ang_min + (2 * PI) > starlines[i].ang_max
      ) {
         starlines[i].ang_max += starlines[i].ang_inc;
      }
   }

   demos_draw_timer();

   retroflat_draw_release( NULL );
}

int cast_ray( int sx, int sy, float ray, int depth ) {
   int x = 0,
      y = 0;
   float frac_depth = depth * 0.1; /* Which tenth of the wall are we hitting? */
   
   y = sy + (sinf( ray ) * frac_depth);
   if( y >= RAYMAP_H || y < 0 ) {
      return 0; /* Ray is off the map on Y. */
   }

   x = sx + (cosf( ray ) * frac_depth);
   if( x >= RAYMAP_W || x < 0 ) {
      return 0; /* Ray is off the map on X. */
   }

   if( 0 < gc_raymap[y][x] ) {
      return depth;
   } else {
      return cast_ray( sx, sy, ray, depth + 1 );
   }
}

void draw_raycast_iter( struct RAYCAST_DATA* data ) {
   int x = 0;
   struct RETROFLAT_INPUT input_evt;
   int input = 0;
   static int init = 0;
   float ray = 0;
   float wall_dist[320];
   static float facing = 0;
   float wall_dist_corrected = 0; /* Correct for fisheye distortion. */
   int wall_line = 0;

   if( !init ) {
      /* Do initial setup. */

      /* Find projection plane dist using trianble between POV and plane. */
      data->plane_dist = (retroflat_screen_w() / 2) / tan( RAYCAST_FOV / 2 );

      init = 1;
   }

   input = retroflat_poll_input( &input_evt );

   switch( input ) {
   case RETROFLAT_KEY_RIGHT:
      facing += 0.1f;
      break;

   case RETROFLAT_KEY_LEFT:
      facing -= 0.1f;
      break;

   case RETROFLAT_KEY_ESC:
      retroflat_quit( 0 );
      break;
   }

   /* Drawing */

   retroflat_draw_lock( NULL );

   retroflat_rect(
      NULL, RETROFLAT_COLOR_BLACK, 0, 0,
      retroflat_screen_w(), retroflat_screen_h(),
      RETROFLAT_FLAGS_FILL );

   for( x = 0 ; retroflat_screen_w() > x ; x++ ) {
      /* Ray is a fraction of Pi, for 180 FOV. */
      /* Ray angle is pixel X over screen W cross-multiplied by ? over Pi. */
      ray = facing + (x * 3.14159 / retroflat_screen_w());

      wall_dist[x] = cast_ray( 3, 2, ray, 0 );

      if( 0 < wall_dist[x] ) {
         wall_dist_corrected = wall_dist[x] * cos( ray );

         wall_line = RAYCAST_PLANE_DIST * 10 / wall_dist_corrected;

         retroflat_line( NULL, RETROFLAT_COLOR_WHITE,
            x,
            (retroflat_screen_h() / 2) - (wall_line / 2),
            x,
            (retroflat_screen_h() / 2) + (wall_line / 2),
            0 );
      }
   }

   /* Draw minimap. */
   for( x = 0 ; 320 > x ; x++ ) {
      ray = facing + (x * 3.14159 / retroflat_screen_w());
      retroflat_line( NULL, RETROFLAT_COLOR_BLUE,
         RAYCAST_MINI_CX, RAYCAST_MINI_CY,
         /* Convert angles to coords and multiply by stored depths. */
         RAYCAST_MINI_CX + cosf( ray ) * wall_dist[x],
         RAYCAST_MINI_CY + sinf( ray ) * wall_dist[x], 0 );
   }

   demos_draw_timer();

   retroflat_draw_release( NULL );
}

