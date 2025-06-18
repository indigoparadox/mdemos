
#include "demos.h"

void draw_primatives_iter( struct PRIMATIVES_DATA* data ) {
   struct RETROFLAT_INPUT input_evt;
   RETROFLAT_IN_KEY input = 0;
   retrogui_idc_t idc_con = RETROGUI_IDC_NONE;

   if( !data->init ) {
      data->init = 1;
   }

   input = retroflat_poll_input( &input_evt );

   retrocon_input( &(data->con), &input, &input_evt, &idc_con, NULL );

   switch( input ) {
   case RETROFLAT_KEY_RIGHT:
      data->rotate += 0.1f;
      break;

   case RETROFLAT_KEY_LEFT:
      data->rotate -= 0.1f;
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

   retroflat_ellipse( NULL, RETROFLAT_COLOR_RED,
      (retroflat_screen_w() / 2) - 20,
      (retroflat_screen_h() / 2) - 20,
      40, 40, 0 );

   /*
   for( i = 0 ; RETROFLAT_PI * 2 > i ; i += (RETROFLAT_PI / 4) ) {
      retroflat_line( NULL, RETROFLAT_COLOR_BLUE,
         (retroflat_screen_w() / 2) + (cos( i + data->rotate ) * 30),
         (retroflat_screen_h() / 2) + (sin( i + data->rotate ) * 30),
         (retroflat_screen_w() / 2) + (cos( i + data->rotate ) * 60),
         (retroflat_screen_h() / 2) + (sin( i + data->rotate ) * 60),
         0 );
   }
   */

   retrocon_display( &(data->con), NULL );

   demos_draw_timer( data );
   demos_draw_fps( data );

   retroflat_draw_release( NULL );
}

