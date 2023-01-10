
#define RETROFLT_C
#include <retroflt.h>

#include "demos.h"

int main( int argc, char** argv ) {
   int retval = 0;
   struct RETROFLAT_ARGS args;

   /* === Setup === */

   args.screen_w = 320;
   args.screen_h = 200;
   args.title = "mdemo";
   args.assets_path = "";

   retval = retroflat_init( argc, argv, &args );
   if( RETROFLAT_OK != retval ) {
      goto cleanup;
   }

   /* === Main Loop === */

   retroflat_loop( (retroflat_loop_iter)draw_sine_iter, NULL );

cleanup:

   retroflat_shutdown( retval );

   return retval;
}
END_OF_MAIN()

