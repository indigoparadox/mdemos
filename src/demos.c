
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define DEMOS_C
#include "demos.h"

int32_t g_frames_per_sec = 0;
int32_t g_frames_since_last = 0;
uint32_t g_last_frame_ms = 0;

void demos_draw_fps( void* v_data ) {
   char status[STATUS_SZ] = { 0 };
   uint32_t now_ms = 0;
   union MDEMOS_DATA* data = (union MDEMOS_DATA*)v_data;
   
   now_ms = retroflat_get_ms();

   g_frames_since_last++;
   if( 0 == g_last_frame_ms || g_last_frame_ms + 1000 <= now_ms ) {
      g_last_frame_ms = now_ms;
      g_frames_per_sec = g_frames_since_last;
      g_frames_since_last = 0;
   }

   maug_snprintf( status, STATUS_SZ, "%lu", g_frames_per_sec );
   retrofont_string(
      NULL, RETROFLAT_COLOR_RED, status, 0,
      data->base.font_h, 10, 10, 0, 0, RETROFONT_FLAG_OUTLINE );
}

void demos_draw_timer( void* v_data ) {
   char status[STATUS_SZ] = { 0 };
   union MDEMOS_DATA* data = (union MDEMOS_DATA*)v_data;

   if( g_timer ) {
      maug_snprintf( status, STATUS_SZ, "%lu", retroflat_get_ms() );
      retrofont_string(
         NULL, RETROFLAT_COLOR_RED, status, 0,
         data->base.font_h, 10, 20, 0, 0, RETROFONT_FLAG_OUTLINE );
   }
}

