
#include "demos.h"

void draw_hall_iter( struct HALL_DATA* data ) {
   struct RETROFLAT_INPUT input_evt;
   RETROFLAT_IN_KEY input = 0;
   retrogui_idc_t idc_con = RETROGUI_IDC_NONE;
   int x, y, c;

   input = retroflat_poll_input( &input_evt );

   if( !(data->init) ) {
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

   for( y = 1 ; retroflat_screen_w() > y ; y++ ) {
      for( x = 1 ; retroflat_screen_w() > x ; x++ ) {
         c = ~((-(data->x_offset + x | data->x_offset + x)) /
            ((~y) - (9 / y)));
         if( 0 > c ) {
            c *= -1;
         }
         retroflat_px( NULL, c % 16, 16 + x, 16 + y, 0 );
      }
   }

   retroflat_draw_release( NULL );

   data->x_offset += 1;
}

