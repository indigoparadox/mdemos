
#include <stdlib.h>

#define MAUG_C

#include "demos.h"

static int g_loop_idx = -1;
static int g_config = 0;

struct DEMO_CTL_DATA {
   struct RETROGUI gui;
};

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

void demo_ctl_loop( struct DEMO_CTL_DATA* data ) {
   RETROFLAT_IN_KEY input = 0;
   struct RETROFLAT_INPUT input_evt;
   /* char* test_arr[] = { "Test 1", "Test 2", "Test 3" }; */
   static int init = 0;
   union RETROGUI_CTL ctl;
   MERROR_RETVAL retval = MERROR_OK;
   RETROGUI_IDC idc = RETROGUI_IDC_NONE;

   retrogui_lock( &(data->gui) );

   if( !init ) {

      retrogui_init_ctl( &ctl, RETROGUI_CTL_TYPE_LISTBOX, 101 ); 
      
      ctl.base.x = 10;
      ctl.base.y = 10;
      ctl.base.w = 70;
      ctl.base.h = 60;

      retrogui_push_ctl( &(data->gui), &ctl );

      retrogui_push_listbox_item( &(data->gui), 101, "Test 1", 6 );
      retrogui_push_listbox_item( &(data->gui), 101, "Test 2", 6 );
      retrogui_push_listbox_item( &(data->gui), 101, "Test 3", 6 );

      retrogui_init_ctl( &ctl, RETROGUI_CTL_TYPE_BUTTON, 100 );

      ctl.base.x = 90;
      ctl.base.y = 10;
      ctl.base.w = 50;
      ctl.base.h = 20;
      strncpy( ctl.BUTTON.label, "Test", 5 );

      retrogui_push_ctl( &(data->gui), &ctl );

      init = 1;
   }

   /* Input */

   input = retroflat_poll_input( &input_evt );

   switch( input ) {
   case RETROFLAT_KEY_ESC:
      retroflat_quit( 0 );
      break;
   }

   idc = retrogui_poll_ctls( &(data->gui), &input, &input_evt );
   switch( idc ) {
   case 100:
      printf( SIZE_T_FMT "\n", retrogui_get_ctl_sel_idx( &(data->gui), 101 ) );
      break;
   }

   /* Drawing */

   retroflat_draw_lock( NULL );

   retroflat_rect(
      NULL, RETROFLAT_COLOR_BLACK, 0, 0,
      retroflat_screen_w(), retroflat_screen_h(),
      RETROFLAT_FLAGS_FILL );

   retrogui_redraw_ctls( &(data->gui) );

   retroflat_draw_release( NULL );

   retrogui_unlock( &(data->gui) );

cleanup:

   if( MERROR_OK != retval ) {
      retroflat_quit( retval );
   }

   return;
}

int main( int argc, char** argv ) {
   int retval = 0;
   struct RETROFLAT_ARGS args;
   int i = 0;
   void* data = NULL;
   struct DEMO_CTL_DATA data_ctl;
   const int negative_one = -1;
   RETROFLAT_CONFIG config;

   /* === Setup === */

   logging_init();

   maug_mzero( &args, sizeof( struct RETROFLAT_ARGS ) );

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

   retval = retroflat_config_open( &config, 0 );
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
      if( NULL == data ) {
         error_printf( "could not allocate data!" );
         retval = MERROR_ALLOC;
         goto cleanup;
      }

      debug_printf( 1, "setting up console..." );
      ((struct MDEMOS_DATA*)data)->con.lbuffer_color = RETROFLAT_COLOR_WHITE;
      ((struct MDEMOS_DATA*)data)->con.sbuffer_color = RETROFLAT_COLOR_GRAY;
      ((struct MDEMOS_DATA*)data)->con.bg_color = RETROFLAT_COLOR_DARKBLUE;
   }

   /* === Main Loop === */

   if( g_config ) {
      debug_printf( 1, "setting up GUI..." );
      retval = retrogui_init( &(data_ctl.gui) );
      maug_cleanup_if_not_ok();
      debug_printf( 1, "jumping to demo loop..." );
      retroflat_loop( (retroflat_loop_iter)demo_ctl_loop, NULL, &data_ctl );
   } else {
      debug_printf( 1, "jumping to demo loop..." );
      retroflat_loop( gc_demo_loops[g_loop_idx], NULL, data );
   }

cleanup:

#ifndef RETROFLAT_OS_WASM

   if( g_config ) {
      retrogui_free( &(data_ctl.gui) );
   }

   retroflat_shutdown( retval );

   if( NULL != data ) {
      free( data );
   }

   logging_shutdown();

#endif /* !RETROFLAT_OS_WASM */

   return retval;
}
END_OF_MAIN()

