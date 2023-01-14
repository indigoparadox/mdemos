
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define DEMOS_C
#include "demos.h"

static void demos_draw_timer() {
   char status[STATUS_SZ] = { 0 };

   if( g_timer ) {
      retroflat_snprintf( status, STATUS_SZ, "%lu", retroflat_get_ms() );
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
   const float b_vals[] = { 0, 0.5f, 0.02f };
   const float a_vals[] = { 0, 0.5f, 0.03f };
   static float m = 0;
   double x = 0;
   double y = 0;
   struct RETROFLAT_INPUT input_evt;
   int input = 0,
      a = 0,
      b = 0;

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

   /* Adapted from a demo for another platform; trying to figure out how it
    * works!
    */
   for( b = 0 ; 3 > b ; b++ ) {
      for( a = 0 ; 3 > a ; a++ ) {
         x = (double)(retroflat_screen_w() / 2) 
            + cos( a_vals[a] - m ) * sin( b_vals[b] ) * 40.0f;
         y = (double)(retroflat_screen_h() / 2) 
            + cos( b_vals[b] ) * 40.0f;
         retroflat_rect(
            NULL, RETROFLAT_COLOR_WHITE, x, y, 1, 1,
            RETROFLAT_FLAGS_FILL );
      }
   }

   demos_draw_timer();

   retroflat_draw_release( NULL );

   m = .002f + fmod( m, 0.03f );
}

void create_starline( struct STARLINE* starline ) {
   starline->rad_inc = fmod( (rand() * 0.1), 0.3 );
   starline->radius = 10 + (rand() % (retroflat_screen_h() - 20));
   switch( rand() % 3 ) {
   case 0:
      starline->color = RETROFLAT_COLOR_WHITE;
      break;
   case 1:
      starline->color = RETROFLAT_COLOR_BLUE;
      break;
   case 2:
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
   double rad = 0;
   static struct STARLINE starlines[STARLINES_SZ];
   static int init = 0;
   static int starlines_sz = 0;

   if( !init ) {
      /* Do initial setup. */
      memset( starlines, '\0', STARLINES_SZ * sizeof( struct STARLINE ) );
      init = 1;
   }

   printf( "x\n" );

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

   if( STARLINES_SZ > starlines_sz && rand() % 20 > 10 ) {
      create_starline( &(starlines[starlines_sz]) );
      starlines_sz++;
   }

   for( i = 0 ; starlines_sz > i ; i++ ) {
      /* Iterate the points on an arc for each starline. */
      for(
         rad = starlines[i].rad_inc ;
         starlines[i].rad_max > rad ;
         rad += starlines[i].rad_inc
      ) {
         /* Calculate previous x/y of a point in the starline using cos/sin. */
         x_prev = (retroflat_screen_w() / 2) + cos( rad - starlines[i].rad_inc )
            * starlines[i].radius;
         y_prev = retroflat_screen_h() + sin( rad - starlines[i].rad_inc )
            * starlines[i].radius;

         /* Calculate x/y of a point in the starline using cos/sin. */
         x = (retroflat_screen_w() / 2) /* Center on X. */
            + cos( rad ) * starlines[i].radius;
         y = retroflat_screen_h() /* Y at the bottom of the screen. */
            + sin( rad ) * starlines[i].radius;

         /* Draw the line from prev point to current point. */
         retroflat_line( NULL, starlines[i].color, x_prev, y_prev, x, y, 0 );
      }

      /* Increment radians if not a full half-circle yet. */
      if( 10 > starlines[i].rad_max ) {
         starlines[i].rad_max += starlines[i].rad_inc;
      }
   }

   demos_draw_timer();

   retroflat_draw_release( NULL );
}

