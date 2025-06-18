
#include "demos.h"

void create_starlines( struct STARLINE_DATA* starlines ) {
   int i = 0;

   for( i = 0 ; STARLINES_SZ > i ; i++ ) {
      starlines->ang_inc[i] = STARLINE_ANG_INC;
      starlines->radius[i] = 
         STARLINE_RADIUS_MIN + (rand() % STARLINE_RADIUS_MAX);
      starlines->ang_min[i] = fmod( (rand() * 0.1), DEMOS_PI ) 
         - DEMOS_PI; /* Make angle negative since we're facing up. */
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
   RETROFLAT_IN_KEY input = 0;
   double angle = 0;
   retrogui_idc_t idc_con = RETROGUI_IDC_NONE;

   if( !(data->init) ) {
      /* Do initial setup. */

      data->start_at = retroflat_get_ms() + STARLINE_START_AT;

      create_starlines( data );

      data->init = 1;
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
         retroflat_px( NULL, data->color[i], x, y, 0 );
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
         data->ang_min[i] + (2 * DEMOS_PI) > data->ang_max[i]
      ) {
         data->ang_max[i] += data->ang_inc[i];
      }
   }

   retrocon_display( &(data->con), NULL );

   demos_draw_timer( data );
   demos_draw_fps( data );

   retroflat_draw_release( NULL );
}

