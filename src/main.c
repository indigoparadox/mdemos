
#include <time.h>
#include <stdlib.h>

#define MAUG_C
#include <maug.h>

#define RETROFLT_C
#include <retroflt.h>

#include "demos.h"

static int g_loop_idx = -1;

static int demo_cli_cb( const char* arg, struct RETROFLAT_ARGS* args ) {
   int i = 0;

   for( i = 0 ; '\0' != gc_demo_names[i][0] ; i++ ) {
      if( 0 == strncmp( gc_demo_names[i], arg, strlen( gc_demo_names[i] ) ) ) {
         g_loop_idx = i;
         printf( "demo loop manually selected: %s\n", gc_demo_names[i] );
         break;
      }
   }

   return RETROFLAT_OK;
}

static int demo_timer_cli_cb( const char* arg, struct RETROFLAT_ARGS* args ) {
   g_timer = 1;
   printf( "timer enabled\n" );
   return RETROFLAT_OK;
}

int main( int argc, char** argv ) {
   int retval = 0;
   struct RETROFLAT_ARGS args;
   int i = 0;
   void* data = NULL;

   /* === Setup === */

   srand( time( NULL ) );

   args.screen_w = 320;
   args.screen_h = 200;
   args.title = "mdemo";
   args.assets_path = "";

   maug_add_arg(
      MAUG_CLI_SIGIL "t", MAUG_CLI_SIGIL_SZ + 1, "show the on-screen timer", 0,
      (maug_cli_cb)demo_timer_cli_cb, NULL, &args );

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

   if( 0 > g_loop_idx ) {
      g_loop_idx = rand() % i;
      printf( "auto-selecting demo loop (%d of %d): %s\n",
         g_loop_idx, i, gc_demo_names[g_loop_idx] );
   }

   if( 0 < gc_demo_data_sz[g_loop_idx] ) {
      data = calloc( 1, gc_demo_data_sz[g_loop_idx] );
   }

   /* === Main Loop === */

   retroflat_loop( gc_demo_loops[g_loop_idx], data );

cleanup:

   retroflat_shutdown( retval );

   return retval;
}
END_OF_MAIN()

