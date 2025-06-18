
#include "demos.h"

void draw_sphere_iter( struct SPHERE_DATA* data ) {
   double x = 0,
      y = 0,
      ir = 0,
      or = 0,
      or_pulse = 0;
   struct RETROFLAT_INPUT input_evt;
   RETROFLAT_IN_KEY input = 0;
   RETROFLAT_COLOR color;
   retrogui_idc_t idc_con = RETROGUI_IDC_NONE;

   input = retroflat_poll_input( &input_evt );

   if( !(data->init) ) {
      data->x_v = 5;
      data->y_v = 5;
      data->x_c = SPHERE_RADIUS;
      data->y_c = SPHERE_RADIUS;
      data->pulse = 0;

      data->init = 1;
   }

   retrocon_input( &(data->con), &input, &input_evt, &idc_con, NULL );

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
   for( or = 0 ; 2 * DEMOS_PI > or ; or += 0.4 ) {
      for( ir = 0 ; 2 * DEMOS_PI > ir ; ir += 0.4 ) {
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

   retrocon_display( &(data->con), NULL );

   demos_draw_timer( data );
   demos_draw_fps( data );

   retroflat_draw_release( NULL );

   /* Increment the rotation for each frame. sin( mod 0.3 ) "jumps,"
    * so sin( 0.4 ) seems to be the sweet spot.
    */
   data->m = .0126f + fmod( data->m, 0.4f );
}

