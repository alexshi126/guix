/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_display_driver_generic_glyph_4bit_draw", /* Test name */
    0, 12, 216, 291  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_pixelmaps_32bpp.c"

char test_comment[256];

static INT brush_alpha_value[] = {0, 100, 200, 255};
 
/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{

int brush_alpha_index;
int frame_id = 1;
GX_RECTANGLE size;

    /* shift pic window so that we cant test the skip code in pixelmap draw function */
    size = pMainWin -> gx_widget_size;
    size.gx_rectangle_left = -56;
    size.gx_rectangle_right = 423;
    gx_widget_resize((GX_RECTANGLE *)pMainWin, &size);

    for(brush_alpha_index = 0; brush_alpha_index < (INT)(sizeof(brush_alpha_value)/sizeof(INT)); brush_alpha_index++)
    {
        brush_alpha = brush_alpha_value[brush_alpha_index];

        /* Inform the validation system 
        (1) Frame ID, which identifies a specific test configuration;
        (2) Start recording frame on the next toggle operation.
        */
        gx_validation_set_frame_id(frame_id);

        sprintf(test_comment, "brush alpha: %d ", brush_alpha);

        gx_validation_set_frame_comment(test_comment);

        /* Force a screen refresh. */
        gx_validation_screen_refresh();

        /* Increment frame ID */
        frame_id ++;
    
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





