/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_drop_list", /* Test name */
    165, 138, 460, 400  /* Define the coordinates of the capture area.
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

#include "demo_guix_drop_list.c"

static int test_scroll_values[]={
-1, -2, -3, -4, -5, -6, -7, -8,
-10, -15, -20, -25, -27, -30, -36, -48,
-59, -62, -73, -74, -80, -81, -89, -90,
-200, -3000, 50, 40, 30, 20, 1,
2, 2000
};

static char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT frame_id = 1;
UINT index;
GX_DROP_LIST *drop_list;
GX_VERTICAL_LIST *list;
GX_WIDGET *child;
GX_EVENT myevent;

    drop_list = &main_screen.main_screen_droplist;

    /* Get vertical list of drop list. */
    gx_drop_list_popup_get(drop_list, &list);

    /* Open drop list.  */
    gx_drop_list_open(drop_list);

    gx_validation_set_frame_id(frame_id++);
    sprintf(test_comment, "Open drop list.");
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();

    memset(&myevent, 0, sizeof(GX_EVENT));
    myevent.gx_event_display_handle = -1;
    myevent.gx_event_target = (GX_WIDGET *)list;
    myevent.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
    myevent.gx_event_payload.gx_event_intdata[0] = 0;

    for(index = 0; index < sizeof(test_scroll_values)/sizeof(int); index++)
    {
        myevent.gx_event_payload.gx_event_intdata[1] = test_scroll_values[index];

        gx_validation_set_frame_id(frame_id++);
        sprintf(test_comment, "scroll value: %d", test_scroll_values[index]);
        gx_validation_set_frame_comment(test_comment);
        gx_system_event_send(&myevent);
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





