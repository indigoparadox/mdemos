
#include "demos.h"

void draw_palette_iter( struct PALETTE_DATA* data ) {
   struct RETROFLAT_INPUT input_evt;
   RETROFLAT_IN_KEY input = 0;
   retrogui_idc_t idc_con = RETROGUI_IDC_NONE;

   input = retroflat_poll_input( &input_evt );

   if( !(data->init) ) {
      retroflat_draw_lock( NULL );

      retroflat_blank( NULL, RETROFLAT_COLOR_BLUE );

      retroflat_draw_release( NULL );

      data->init = 1;

      data->timer = 30;
   }

   retrocon_input( &(data->con), &input, &input_evt, &idc_con, NULL );

   switch( input ) {
   case RETROFLAT_KEY_ESC:
      retroflat_quit( 0 );
      break;
   }

   /* Drawing */

   /* retrocon_display( &(data->con), NULL ); */

   retroflat_draw_lock( NULL );

   demos_draw_timer( data );
   demos_draw_fps( data );

   data->timer--;
   if( 0 == data->timer ) {
      retroflat_set_palette( RETROFLAT_COLOR_BLUE, data->color );

      data->color += 0x0a0a0a;
      if( 0xffffff <= data->color ) {
         data->color = 0;
      }
      data->timer = 30;
   }

   retroflat_draw_release( NULL );

}

