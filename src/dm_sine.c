
#include "demos.h"

void draw_sine_iter( struct SINE_DATA* data ) {
   struct RETROFLAT_INPUT input_evt;
   retroflat_pxxy_t x_prev = 0;
   RETROFLAT_IN_KEY input = 0;
   double y_offset = 0;
   retrogui_idc_t idc_con = RETROGUI_IDC_NONE;

   if( !data->init ) {
      data->init = 1;
      retroflat_rect(
         NULL, RETROFLAT_COLOR_BLACK, 0, 0,
         retroflat_screen_w(), retroflat_screen_h(),
         RETROFLAT_FLAGS_FILL );
   }

   input = retroflat_poll_input( &input_evt );

   retrocon_input( &(data->con), &input, &input_evt, &idc_con, NULL );

   switch( input ) {
   case RETROFLAT_KEY_ESC:
      retroflat_quit( 0 );
      break;
   }

   /* Drawing */

   retroflat_draw_lock( NULL );

   /* Draw lines up to the current x_iter with each pixel as X. */
   x_prev = data->x_iter - 1;

   retroflat_line(
      NULL, RETROFLAT_COLOR_WHITE,
      x_prev, (retroflat_screen_h() / 2) + (sin( x_prev ) * SINE_AMPLIFIER),
      data->x_iter,
         (retroflat_screen_h() / 2) + (sin( data->x_iter ) * SINE_AMPLIFIER),
      0 );

   /* As the x coordinate increases, the y_offset goes down then up again! */
   y_offset = 100.0f * sin( data->x_iter / (double)(retroflat_screen_w() / 3) );

   /* A single, screen-wide big arc made of a tinier sine wave. */
   retroflat_line(
      NULL, RETROFLAT_COLOR_RED,
      x_prev, y_offset + (sin( x_prev ) * SINE_AMPLIFIER),
      data->x_iter, y_offset + (sin( data->x_iter ) * SINE_AMPLIFIER), 0 );

   retrocon_display( &(data->con), NULL );

   demos_draw_timer( data );
   demos_draw_fps( data );

   retroflat_draw_release( NULL );

   (data->x_iter)++;
   if( retroflat_screen_w() <= data->x_iter ) {
      debug_printf( 1, "reset from " SIZE_T_FMT " to 0", data->x_iter );
      data->x_iter = 0;
      retroflat_rect(
         NULL, RETROFLAT_COLOR_BLACK, 0, 0,
         retroflat_screen_w(), retroflat_screen_h(),
         RETROFLAT_FLAGS_FILL );
   }
}


