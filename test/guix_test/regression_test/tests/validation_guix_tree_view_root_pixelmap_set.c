/* This is a small demo of the high-performance GUIX graphics framework. */


#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_tree_view_root_pixelmap_set", /* Test name */
    4, 4,633, 473  /* Define the coordinates of the capture area.
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

#include "demo_guix_all_widgets.c"


char test_comment[256];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_TREE_VIEW  *tree;
    gx_widget_detach(pButtonScreen);
    gx_widget_attach(root, (GX_WIDGET *)&menu_screen);
    tree = &menu_screen.menu_screen_tree_view;

    gx_tree_view_root_pixelmap_set(tree, GX_PIXELMAP_ID_I_INDICATOR, GX_PIXELMAP_ID_I_INDICATOR_HORIZONTAL);
    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "set tree view expand_map_id: GX_PIXELMAP_ID_I_INDICATOR, collapse_map_id:GX_PIXELMAP_ID_I_INDICATOR_HORIZONTAL.");
    gx_validation_set_frame_comment(test_comment);

    gx_system_dirty_mark((GX_WIDGET *)&menu_screen);

    /* Force a screen refresh. */
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





