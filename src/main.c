
#include <time.h>
#include <stdlib.h>

#define MAIN_C
#define RETROFLT_C
#include <retroflt.h>

#include "demos.h"

retroflat_loop_iter g_loop = NULL;

static int demo_cli_cb( const char* arg, struct RETROFLAT_ARGS* args ) {
   int i = 0;

   for( i = 0 ; '\0' != gc_demo_names[i][0] ; i++ ) {
      if( 0 == strncmp( gc_demo_names[i], arg, strlen( gc_demo_names[i] ) ) ) {
         g_loop = gc_demo_loops[i];
         printf( "demo loop manually selected: %s\n", gc_demo_names[i] );
         break;
      }
   }

   return RETROFLAT_OK;
}

int main( int argc, char** argv ) {
   int retval = 0;
   struct RETROFLAT_ARGS args;
   int i = 0,
      j = 0;

   /* === Setup === */

   srand( time( NULL ) );

   args.screen_w = 320;
   args.screen_h = 200;
   args.title = "mdemo";
   args.assets_path = "";

   for( i = 0 ; '\0' != gc_demo_names[i][0] ; i++ ) {
      retroflat_add_arg(
         gc_demo_names[i], 0, "display this demo", 0, demo_cli_cb );
   }

   retval = retroflat_init( argc, argv, &args );
   if( RETROFLAT_OK != retval ) {
      goto cleanup;
   }

   /* See if a loop was specified. */
#if 0
   i = 0;
   while( '\0' != gc_demo_names[i][0] ) {
      for( j = 1 ; argc > j ; j++ ) {
         if( 0 == strncmp(
            gc_demo_names[i], argv[j], strlen( gc_demo_names[i] )
         ) ) {
            printf( "demo loop manually selected: %s\n", gc_demo_names[i] );
            loop = gc_demo_loops[i];
            break;
         }
      }
      if( NULL != loop ) {
         break;
      }
      i++;
   }
#endif

   if( NULL == g_loop ) {
      j = rand() % i;
      g_loop = gc_demo_loops[j];
      printf( "auto-selecting demo loop (%d of %d): %s\n",
         j, i, gc_demo_names[j] );
   }

   /* === Main Loop === */

   retroflat_loop( (retroflat_loop_iter)g_loop, NULL );

cleanup:

   retroflat_shutdown( retval );

   return retval;
}
END_OF_MAIN()

