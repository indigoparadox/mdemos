
#include "demos.h"

const uint8_t gc_raymap[RAYMAP_H][RAYMAP_W] = {
   { 5, 5, 5, 5, 5, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 5, 0, 0, 0, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 0, 0, 0, 0, 5 },
   { 5, 5, 5, 5, 5, 5 }
};

int cast_ray( int sx, int sy, float ray, int depth ) {
   int x = 0,
      y = 0;
   /* Which tenth of the wall are we hitting? */
   float frac_depth = depth * (1.0f / RAYCAST_WALL_H);
   
   do {
      y = sy + (sin( ray ) * frac_depth);
      x = sx + (cos( ray ) * frac_depth);
      depth += 1;
      frac_depth = depth * (1.0f / RAYCAST_WALL_H);
   } while(
      (y < RAYMAP_H || y >= 0) &&
      (x < RAYMAP_W || x >= 0) &&
      0 == gc_raymap[y][x]
   );

   return depth;
}

void draw_raycast_iter( struct RAYCAST_DATA* data ) {
   int x = 0;
   struct RETROFLAT_INPUT input_evt;
   RETROFLAT_IN_KEY input = 0;
   float ray = 0;
   float ray_next = 0;
   float wall_dist[320];
   int wall_line = 0;
   RETROFLAT_COLOR wall_color;
   retrogui_idc_t idc_con = RETROGUI_IDC_NONE;

   if( !(data->init) ) {
      /* Do initial setup. */

      /* Find projection plane dist using triangle between POV and plane. */
      data->plane_dist = (retroflat_screen_w() / 2) / tan( RAYCAST_FOV / 2 );

      /* Each ray is this many radians off to the right of the last. */
      data->ray_inc = (2 * DEMOS_PI_4) / retroflat_screen_w();

      /* Start off at this position on the tilemap. */
      data->pos_x = 3;
      data->pos_y = 2;

      data->init = 1;
   }

   input = retroflat_poll_input( &input_evt );

   retrocon_input( &(data->con), &input, &input_evt, &idc_con, NULL );

   switch( input ) {
   case RETROFLAT_KEY_UP:
      /* Move forward in facing dir. */
      if( DEMOS_PI_4 <= data->facing && 3 * DEMOS_PI_4 > data->facing ) {
         /* Down */
         if( data->pos_y + 2 < RAYMAP_H ) {
            data->pos_y += 1;
         }

      } else if(
         3 * DEMOS_PI_4 <= data->facing && 5 * DEMOS_PI_4 > data->facing
      ) {
         /* Left */
         if( data->pos_x - 2 >= 0 ) {
            data->pos_x -= 1;
         }

      } else if(
         5 * DEMOS_PI_4 <= data->facing && 7 * DEMOS_PI_4 > data->facing
      ) {
         /* Up */
         if( data->pos_y - 2 >= 0 ) {
            data->pos_y -= 1;
         }

      } else {
         /* Right */
         if( data->pos_x + 2 < RAYMAP_W ) {
            data->pos_x += 1;
         }
      }
      break;

   case RETROFLAT_KEY_RIGHT:
      data->facing += 0.1f;
      if( (2 * RETROFLAT_PI) <= data->facing ) {
         data->facing = 0;
      }
      break;

   case RETROFLAT_KEY_LEFT:
      data->facing -= 0.1f;
      if( data->facing < 0 ) {
         data->facing = (2 * RETROFLAT_PI) - 0.1;
      }
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

   for( x = 0 ; retroflat_screen_w() > x ; x += 2 ) {
      /* Ray is a fraction of Pi, for 180 FOV. */
      /* Ray angle is pixel X over screen W cross-multiplied by ? over Pi. */
      ray = data->facing + ((x - (retroflat_screen_w() / 2)) * data->ray_inc);
      ray_next = ray + data->ray_inc;

      wall_dist[x] = cast_ray( data->pos_x, data->pos_y, ray, 0 );
      wall_dist[x + 1] = cast_ray( data->pos_x, data->pos_y, ray_next, 0 );

      if( 0 >= wall_dist[x] ) {
         /* Something weird happened. */
         continue;
      }

      /* Compare ray distances, shade accordingly. */
      if( fabs( wall_dist[x] ) > fabs( wall_dist[x + 1] ) ) {
         wall_color = RETROFLAT_COLOR_WHITE;
      } else {
         wall_color = RETROFLAT_COLOR_GRAY;
      }

      /* Draw the wall. */
      wall_line = data->plane_dist * RAYCAST_WALL_H / 
         /* Multiply by cos( angle between x beam and center ) to correct
          * fisheye effect.
          */
         (wall_dist[x] * cos( ray - data->facing ));
      retroflat_line( NULL, wall_color,
         x,
         (retroflat_screen_h() / 2) - (wall_line / 2),
         x,
         (retroflat_screen_h() / 2) + (wall_line / 2),
         0 );

      wall_line = data->plane_dist * RAYCAST_WALL_H /
         /* Multiply by cos( angle between x beam and center ) to correct
          * fisheye effect.
          */
         (wall_dist[x + 1] * cos( ray - data->facing ));
      retroflat_line( NULL, wall_color,
         x + 1,
         (retroflat_screen_h() / 2) - (wall_line / 2),
         x + 1,
         (retroflat_screen_h() / 2) + (wall_line / 2),
         0 );
   }

   /* Draw minimap. */
   for( x = 0 ; retroflat_screen_w() > x ; x++ ) {
      ray = data->facing + ((x - retroflat_screen_w() / 2) * data->ray_inc);
      if( 0 == x % 2 ) {
         wall_color = RETROFLAT_COLOR_BLUE;
      } else {
         wall_color = RETROFLAT_COLOR_RED;
      }
      retroflat_line( NULL, wall_color,
         RAYCAST_MINI_CX, RAYCAST_MINI_CY,
         /* Convert angles to coords and multiply by stored depths. */
         RAYCAST_MINI_CX + cos( ray ) * (wall_dist[x] / RAYCAST_MINIMAP_SCALE),
         RAYCAST_MINI_CY + sin( ray ) * (wall_dist[x] / RAYCAST_MINIMAP_SCALE),
         0 );
   }

   retrocon_display( &(data->con), NULL );

   demos_draw_timer( data );
   demos_draw_fps( data );

   retroflat_draw_release( NULL );
}

