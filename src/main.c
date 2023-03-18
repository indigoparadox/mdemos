
#include <stdlib.h>

#define MAUG_C
#include <maug.h>

#define RETROFLT_C
#include <retroflt.h>

#define RETROGUI_C
#include <retrogui.h>

#define RETROCON_C
#include <retrocon.h>

#include "demos.h"

static int g_loop_idx = -1;
static int g_config = 0;

struct RETROGUI_CTL g_ctls[2];

static int demo_cli_c( const char* arg, struct RETROFLAT_ARGS* args ) {
   g_config = 1;
   return RETROFLAT_OK;
}

static int demo_cli_cb( const char* arg, struct RETROFLAT_ARGS* args ) {
   int i = 0;

   for( i = 0 ; '\0' != gc_demo_names[i][0] ; i++ ) {
      if( 0 == strncmp( gc_demo_names[i], arg, strlen( gc_demo_names[i] ) ) ) {
         g_loop_idx = i;
         debug_printf( 3, "demo loop manually selected: %s",
            gc_demo_names[i] );
         break;
      }
   }

   return RETROFLAT_OK;
}

static int demo_timer_cli_cb( const char* arg, struct RETROFLAT_ARGS* args ) {
   g_timer = 1;
   debug_printf( 3, "timer enabled" );
   return RETROFLAT_OK;
}

void demo_ctl_loop( void* data ) {
   int input = 0;
   struct RETROFLAT_INPUT input_evt;
   char* test_arr[] = { "Test 1", "Test 2", "Test 3" };
   static int init = 0;

   if( !init ) {
      retrogui_add_listbox(
         test_arr, 3, 0, 10, 10, 70, 60,
         RETROFLAT_COLOR_WHITE, RETROFLAT_COLOR_BLACK, 101, 0, &(g_ctls[0]) );

      retrogui_add_button( 
         "Test", 90, 10, 50, 20,
         RETROFLAT_COLOR_WHITE, RETROFLAT_COLOR_BLACK, 100, 0, &(g_ctls[1]) );
   }

   /* Input */

   input = retroflat_poll_input( &input_evt );

   switch( input ) {
   case RETROFLAT_KEY_ESC:
      retroflat_quit( 0 );
      break;
   }

   input = retrogui_poll_ctls( input, &input_evt, g_ctls, 2 );
   switch( input ) {
   case 100:
      printf( "%d\n", retrogui_get_ctl_sel_idx( &(g_ctls[0]) ) );
      break;
   }

   /* Drawing */

   retroflat_draw_lock( NULL );

   retroflat_rect(
      NULL, RETROFLAT_COLOR_BLACK, 0, 0,
      retroflat_screen_w(), retroflat_screen_h(),
      RETROFLAT_FLAGS_FILL );

   retrogui_redraw_ctls( g_ctls, 2 );

   retroflat_draw_release( NULL );
}

int main( int argc, char** argv ) {
   int retval = 0;
   struct RETROFLAT_ARGS args;
   int i = 0;
   void* data = NULL;
   const int negative_one = -1;
   RETROFLAT_CONFIG config;

   /* === Setup === */

   maug_mzero( &args, sizeof( struct RETROFLAT_ARGS ) );

   args.screen_w = 320;
   args.screen_h = 200;
   args.title = "mdemo";
   args.assets_path = "";

   maug_add_arg(
      MAUG_CLI_SIGIL "t", MAUG_CLI_SIGIL_SZ + 1, "show the on-screen timer", 0,
      (maug_cli_cb)demo_timer_cli_cb, NULL, &args );
   maug_add_arg(
      MAUG_CLI_SIGIL "c", MAUG_CLI_SIGIL_SZ + 1, "show config dialog", 0,
      (maug_cli_cb)demo_cli_c, NULL, &args );

   /* Add demos to CLI parser. */
   for( i = 0 ; '\0' != gc_demo_names[i][0] ; i++ ) {
      maug_add_arg(
         gc_demo_names[i], 0, "display this demo", 0,
         (maug_cli_cb)demo_cli_cb, NULL, &args );
   }

   retval = retroflat_init( argc, argv, &args );
   if( RETROFLAT_OK != retval ) {
      goto cleanup;
   }

   retval = retroflat_config_open( &config );
   if( MERROR_OK == retval ) {
      retroflat_config_read(
         &config, "default", "demo", RETROFLAT_BUFFER_INT,
         &g_loop_idx, sizeof( int ),
         &negative_one, sizeof( int ) );

      retroflat_config_close( &config );
   }
   retval = MERROR_OK;

   if( 0 > g_loop_idx ) {
      g_loop_idx = rand() % i;
      debug_printf( 3, "auto-selecting demo loop (%d of %d): %s",
         g_loop_idx, i, gc_demo_names[g_loop_idx] );
   } else {
      debug_printf( 3,
         "demo loop specified in command line or config: %s (%d)",
         gc_demo_names[g_loop_idx], g_loop_idx );
   }

   if( 0 < gc_demo_data_sz[g_loop_idx] ) {
      debug_printf(
         2, "allocating data (" SIZE_T_FMT " bytes)...",
         gc_demo_data_sz[g_loop_idx] );
      data = calloc( 1, gc_demo_data_sz[g_loop_idx] );

      ((struct MDEMOS_DATA*)data)->con.lbuffer_color = RETROFLAT_COLOR_WHITE;
      ((struct MDEMOS_DATA*)data)->con.sbuffer_color = RETROFLAT_COLOR_GRAY;
      ((struct MDEMOS_DATA*)data)->con.bg_color = RETROFLAT_COLOR_DARKBLUE;
   }

   /* === Main Loop === */

   if( g_config ) {
      retroflat_loop( demo_ctl_loop, NULL );
   } else {
      retroflat_loop( gc_demo_loops[g_loop_idx], data );
   }

cleanup:

#ifndef RETROFLAT_OS_WASM

   retroflat_shutdown( retval );

   if( NULL != data ) {
      free( data );
   }

#endif /* !RETROFLAT_OS_WASM */

   return retval;
}
END_OF_MAIN()

