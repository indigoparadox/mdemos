
#include "demos.h"

void draw_retroani_iter( struct RETROANI_DATA* data ) {
   struct RETROFLAT_INPUT input_evt;
   RETROFLAT_IN_KEY input = 0;
   retrogui_idc_t idc_con = RETROGUI_IDC_NONE;

   if( !data->init ) {
      debug_printf( 1, "initializing retroani demo..." );

      debug_printf( 1, "creating animations..." );

      retroani_create(
         &(data->animations), RETROANI_TYPE_SNOW, RETROANI_FLAG_CLEANUP,
         0, 0, retroflat_screen_w(), retroflat_screen_h() - RETROFLAT_TILE_H );

      retroani_create(
         &(data->animations), RETROANI_TYPE_FIRE, RETROANI_FLAG_CLEANUP,
         0, retroflat_screen_h() - RETROFLAT_TILE_H, retroflat_screen_w(),
         RETROFLAT_TILE_H );

      debug_printf( 1, "initialization complete!" );
      data->init = 1;
   }

   input = retroflat_poll_input( &input_evt );

   retrocon_input( &(data->con), &input, &input_evt, &idc_con, NULL );

   switch( input ) {
   case RETROFLAT_KEY_RIGHT:
      break;

   case RETROFLAT_KEY_LEFT:
      break;

   case RETROFLAT_KEY_ESC:
      retroani_stop_all( &(data->animations) );
      retroflat_quit( 0 );
      break;
   }

   /* Drawing */

   retroflat_draw_lock( NULL );

   retroani_frame( &(data->animations), 0 );

   retrocon_display( &(data->con), NULL );

   demos_draw_timer( data );
   demos_draw_fps( data );

   retroflat_draw_release( NULL );
}

