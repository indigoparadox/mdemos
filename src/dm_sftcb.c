
#include "demos.h"

void draw_softcube_iter( struct SOFTCUBE_DATA* data ) {
   struct RETROFLAT_INPUT input_evt;
   RETROFLAT_IN_KEY input = 0;
   retrogui_idc_t idc_con = RETROGUI_IDC_NONE;

   if( !data->init ) {
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
      retroflat_quit( 0 );
      break;
   }

   /* Drawing */

   retroflat_draw_lock( NULL );

   retrocon_display( &(data->con), NULL );

   demos_draw_timer( data );
   demos_draw_fps( data );

   retroflat_draw_release( NULL );
}

