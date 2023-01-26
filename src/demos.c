
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

void draw_sine_iter( struct SINE_DATA* data ) {
   struct RETROFLAT_INPUT input_evt;
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
   for( x = 1 ; data->x_iter > x ; x++ ) {
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

   (data->x_iter)++;
   if( 320 <= data->x_iter ) {
      data->x_iter = 0;
   }
}

void draw_sphere_iter( struct SPHERE_DATA* data ) {
   double x = 0,
      y = 0,
      ir = 0,
      or = 0,
      or_pulse = 0;
   struct RETROFLAT_INPUT input_evt;
   int input = 0;
   RETROFLAT_COLOR color;

   input = retroflat_poll_input( &input_evt );

   if( !(data->init) ) {
      data->x_v = 5;
      data->y_v = 5;
      data->x_c = SPHERE_RADIUS;
      data->y_c = SPHERE_RADIUS;
      data->pulse = 0;

      data->init = 1;
   }

   assert( 0 == retroflat_screen_w() % data->x_v );
   assert( 0 == retroflat_screen_h() % data->y_v );

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

   /* Move sphere along bounce trajectory. */
   data->x_c += data->x_v;
   if( 
      data->x_c + SPHERE_RADIUS + data->x_v >= retroflat_screen_w() ||
      data->x_c - SPHERE_RADIUS + data->x_v < 0
   ) {
      data->x_v *= -1;
   }

   data->y_c += data->y_v;
   if(
      data->y_c + SPHERE_RADIUS + data->y_v >= retroflat_screen_h() ||
      data->y_c - SPHERE_RADIUS + data->y_v < 0
   ) {
      data->y_v *= -1;
   }

   /* Draw sphere. */
   for( or = 0 ; 2 * PI > or ; or += 0.4 ) {
      for( ir = 0 ; 2 * PI > ir ; ir += 0.4 ) {
         if( 0 < data->pulse ) {
            /* Pulse dots in and out a bit? */
            or_pulse = or + ((-1 * data->pulse) + (rand() % (2 * data->pulse)));
         } else {
            or_pulse = or;
         }

         x = (double)data->x_c + (cos( or_pulse ) * SPHERE_RADIUS *
         /* Multiple by sin( ir - m ) on X to give a cyclicle leftward(?)
          * bias to the inner circles, creating sphere illusion.
          */
            sin( ir - data->m ));
         y = (double)data->y_c + (sin( or_pulse ) * SPHERE_RADIUS);

         /* Vary shade depending on position. */
         color = or + ir + (rand() % 3) > 8 ?
            RETROFLAT_COLOR_WHITE : RETROFLAT_COLOR_GRAY;

         retroflat_px( NULL, color, x, y, 0 );
      }
   }

   demos_draw_timer();

   retroflat_draw_release( NULL );

   /* Increment the rotation for each frame. sin( mod 0.3 ) "jumps,"
    * so sin( 0.4 ) seems to be the sweet spot.
    */
   data->m = .0126f + fmod( data->m, 0.4f );
}

void create_starlines( struct STARLINE_DATA* starlines ) {
   int i = 0;

   for( i = 0 ; STARLINES_SZ > i ; i++ ) {
      starlines->ang_inc[i] = STARLINE_ANG_INC;
      starlines->radius[i] = 
         STARLINE_RADIUS_MIN + (rand() % STARLINE_RADIUS_MAX);
      starlines->ang_min[i] = fmod( (rand() * 0.1), PI ) 
         - PI; /* Make angle negative since we're facing up. */
      starlines->ang_max[i] = starlines->ang_min[i];
      starlines->flicker[i] = rand() % STARLINE_FLICKER_ODDS;
      starlines->flicker_odd[i] = rand() % 2;
      switch( rand() % 2 ) {
      case 0:
         starlines->color[i] = RETROFLAT_COLOR_WHITE;
         break;
      case 1:
         starlines->color[i] = RETROFLAT_COLOR_CYAN;
         break;
      }
   }
}

void draw_starlines_iter( struct STARLINE_DATA* data ) {
   int x = 0,
      y = 0,
      x_prev = 0,
      y_prev = 0,
      i = 0;
   struct RETROFLAT_INPUT input_evt;
   int input = 0;
   double angle = 0;

   if( !(data->init) ) {
      /* Do initial setup. */

      data->start_at = retroflat_get_ms() + STARLINE_START_AT;

      create_starlines( data );

      data->init = 1;
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
         + cos( data->ang_min[i] ) * data->radius[i];
      y = retroflat_screen_h() /* Y at the bottom of the screen. */
         + sin( data->ang_min[i] ) * data->radius[i];

      if( 
         /* Don't draw stars after spinning starts. */
         data->start_at >= retroflat_get_ms() &&
         /* Flicker some stars. */
         (10 < data->flicker[i] || 
         data->flicker_odd[i] == retroflat_get_ms() % 2)
      ) {
         retroflat_rect(
            NULL, data->color[i], x, y, 1, 1, RETROFLAT_FLAGS_FILL );
      }

      /* Iterate the points on an arc for each starline. */
      for(
         angle = data->ang_min[i] ;
         data->ang_max[i] > angle ;
         angle += data->ang_inc[i]
      ) {
         /* Calculate previous x/y of a point in the starline using cos/sin. */
         x_prev =
            (retroflat_screen_w() / 2) + cos( angle - data->ang_inc[i] )
            * data->radius[i];
         y_prev = retroflat_screen_h() + sin( angle - data->ang_inc[i] )
            * data->radius[i];

         /* Calculate x/y of a point in the starline using cos/sin. */
         x = (retroflat_screen_w() / 2) /* Center on X. */
            + cos( angle ) * data->radius[i];
         y = retroflat_screen_h() /* Y at the bottom of the screen. */
            + sin( angle ) * data->radius[i];

         /* Draw the line from prev point to current point. */
         retroflat_line( NULL, data->color[i], x_prev, y_prev, x, y, 0 );
      }

      /* Increment radians if not a full circle yet. */
      if(
         data->start_at < retroflat_get_ms() &&
         /* 2PI is a full circle. */
         data->ang_min[i] + (2 * PI) > data->ang_max[i]
      ) {
         data->ang_max[i] += data->ang_inc[i];
      }
   }

   demos_draw_timer();

   retroflat_draw_release( NULL );
}

int cast_ray( int sx, int sy, float ray, int depth ) {
   int x = 0,
      y = 0;
   /* Which tenth of the wall are we hitting? */
   float frac_depth = depth * (1.0f / RAYCAST_WALL_H);
   
   y = sy + (sin( ray ) * frac_depth);
   if( y >= RAYMAP_H || y < 0 ) {
      return 0; /* Ray is off the map on Y. */
   }

   x = sx + (cos( ray ) * frac_depth);
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
   float ray = 0;
   float ray_next = 0;
   float wall_dist[320];
   int wall_line = 0;
   RETROFLAT_COLOR wall_color;

   if( !(data->init) ) {
      /* Do initial setup. */

      /* Find projection plane dist using trianble between POV and plane. */
      data->plane_dist = (retroflat_screen_w() / 2) / tan( RAYCAST_FOV / 2 );

      /* Each ray is this many radians off to the right of the last. */
      data->ray_inc = 3.14159 / retroflat_screen_w();

      /* Start off at this position on the tilemap. */
      data->pos_x = 3;
      data->pos_y = 2;

      data->init = 1;
   }

   input = retroflat_poll_input( &input_evt );

   switch( input ) {
   case RETROFLAT_KEY_RIGHT:
      data->facing += 0.1f;
      break;

   case RETROFLAT_KEY_LEFT:
      data->facing -= 0.1f;
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

   for( x = 0 ; retroflat_screen_w() > x ; x += 2 ) {
      /* Ray is a fraction of Pi, for 180 FOV. */
      /* Ray angle is pixel X over screen W cross-multiplied by ? over Pi. */
      ray = data->facing + (x * data->ray_inc);
      ray_next = ray + data->ray_inc;

      wall_dist[x] = cast_ray( data->pos_x, data->pos_y, ray, 0 );
      wall_dist[x + 1] = cast_ray( data->pos_x, data->pos_y, ray_next, 0 );

      if( 0 >= wall_dist[x] ) {
         /* Something weird happened. */
         continue;
      }

      /* Compare ray distances, shade accordingly. */
      if( fabs( wall_dist[x] ) > fabs( wall_dist[x + 1] ) ) {
         wall_color = RETROFLAT_COLOR_WHITE;
      } else {
         wall_color = RETROFLAT_COLOR_GRAY;
      }

      /* Draw the wall. */
      wall_line = data->plane_dist * RAYCAST_WALL_H / wall_dist[x];
      retroflat_line( NULL, wall_color,
         x,
         (retroflat_screen_h() / 2) - (wall_line / 2),
         x,
         (retroflat_screen_h() / 2) + (wall_line / 2),
         0 );

      wall_line = data->plane_dist * RAYCAST_WALL_H / wall_dist[x + 1];
      retroflat_line( NULL, wall_color,
         x + 1,
         (retroflat_screen_h() / 2) - (wall_line / 2),
         x + 1,
         (retroflat_screen_h() / 2) + (wall_line / 2),
         0 );
   }

   /* Draw minimap. */
   for( x = 0 ; 320 > x ; x++ ) {
      ray = data->facing + (x * 3.14159 / retroflat_screen_w());
      retroflat_line( NULL, RETROFLAT_COLOR_BLUE,
         RAYCAST_MINI_CX, RAYCAST_MINI_CY,
         /* Convert angles to coords and multiply by stored depths. */
         RAYCAST_MINI_CX + cos( ray ) * (wall_dist[x] / RAYCAST_MINIMAP_SCALE),
         RAYCAST_MINI_CY + sin( ray ) * (wall_dist[x] / RAYCAST_MINIMAP_SCALE),
         0 );
   }

   demos_draw_timer();

   retroflat_draw_release( NULL );
}

void draw_primatives_iter( struct PRIMATIVES_DATA* data ) {
   struct RETROFLAT_INPUT input_evt;
   int input = 0;
   double i = 0;

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

   retroflat_ellipse( NULL, RETROFLAT_COLOR_RED,
      (retroflat_screen_w() / 2) - 20,
      (retroflat_screen_h() / 2) - 20,
      40, 40, 0 );

   for( i = 0 ; RETROFLAT_PI * 2 > i ; i += (RETROFLAT_PI / 4) ) {
      retroflat_line( NULL, RETROFLAT_COLOR_BLUE,
         (retroflat_screen_w() / 2) + (cos( i ) * 30),
         (retroflat_screen_h() / 2) + (sin( i ) * 30),
         (retroflat_screen_w() / 2) + (cos( i ) * 60),
         (retroflat_screen_h() / 2) + (sin( i ) * 60),
         0 );
   }

   retroflat_draw_release( NULL );
}

