/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_single_line_text_input_fill_color_set", /* Test name */
    56, 196, 365, 242  /* Define the coordinates of the capture area.
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
       on the line drawing example. These operations simulate 
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

char test_comment[255];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                        frame_id = 1;
GX_SINGLE_LINE_TEXT_INPUT *sl_text_input = GX_NULL;
static char text[] = "This is text buffer of sl line text input.";
GX_EVENT my_event;
    
    memset(&my_event, 0, sizeof(GX_EVENT));
    gx_widget_detach(pButtonScreen);
    gx_widget_attach(root, pTextScreen);
    sl_text_input = &text_screen.text_screen_text_input_1;
    gx_single_line_text_input_style_remove(sl_text_input, GX_STYLE_CURSOR_BLINK);
    gx_single_line_text_input_text_set(sl_text_input, text);

    gx_system_dirty_mark(sl_text_input);
    sprintf(test_comment, "Set sl text: This is text buffer of sl line text input.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();    
    
    gx_single_line_text_input_fill_color_set(sl_text_input, GX_COLOR_ID_ORANGE, GX_COLOR_ID_BLUE, GX_COLOR_ID_WHITE, GX_COLOR_ID_SHADOW);
    
    gx_system_dirty_mark(sl_text_input);
    sprintf(test_comment, "Set fille color to sl: normal_fill: GX_COLOR_ID_ORANGE, selected_fill: GX_COLOR_ID_BLUE, disabled_fill: GX_COLOR_ID_WHITE, readonly_fill: GX_COLOR_ID_SHADOW");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();   
    
    /* Simulate a click event. */
    my_event.gx_event_display_handle = 1;
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 150;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 200;
    gx_system_event_send(&my_event);

    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);
    
    gx_system_dirty_mark(sl_text_input);
    sprintf(test_comment, "Click on input widget (150, 250).");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();   
    
    gx_single_line_text_input_style_add(sl_text_input, GX_STYLE_TEXT_INPUT_READONLY);
    
    gx_system_dirty_mark(sl_text_input);
    sprintf(test_comment, "Add readonly style to widget.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();  
    
    gx_single_line_text_input_style_remove(sl_text_input, GX_STYLE_ENABLED);
    
    gx_system_dirty_mark(sl_text_input);
    sprintf(test_comment, "Remove GX_STYLE_ENABLED from widget.");
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment(test_comment);
    gx_validation_screen_refresh();  
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
