
#include <math.h>

#include <retroflt.h>

#include "demos.h"

void draw_sine_iter( void* data ) {
   struct RETROFLAT_INPUT input_evt;
   static int x_iter = 0;
   int x = 0,
      x_prev = 0,
      input = 0;
   long unsigned int next = 0;
   double y_offset = 0;

   input = retroflat_poll_input( &input_evt );

   switch( input ) {
   case RETROFLAT_KEY_Q:
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

   retroflat_draw_release( NULL );

   next = retroflat_get_ms() + 10;
   while( retroflat_get_ms() < next ) {}

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
   case RETROFLAT_KEY_Q:
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

   retroflat_draw_release( NULL );

   m = .002f + fmod( m, 0.03f );
}

