/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_graphics_ellipse_32bpp", /* Test name */
    24, 43, 370, 402  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the graphics
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

#include "demo_guix_graphics_32bpp.c"

typedef struct ELLIPSE_PROPS_STRUCT{
INT brush_width;
INT solid_fill;
INT pixelmap_fill;
INT alpha_mode;
INT compress_mode;
INT anti_aliased_mode;
}ELLIPSE_PROPS;

ELLIPSE_PROPS ellipse_props[]={
    {1, 0, 0, 0, 0, 1},
    {2, 1, 0, 0, 0, 1},
    {3, 0, 1, 0, 0, 1},
    {5, 0, 1, 0, 1, 1},
    {7, 0, 1, 2, 0, 1},
    {9, 0, 1, 2, 1, 1},
    {18, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0},
    {2, 1, 0, 0, 0, 0},
    {1, 0, 1, 0, 0, 0},
    {19, 0, 1, 2, 0, 0},
};

char test_comment[256];

static INT brush_alpha_value[] = {0, 155, 255};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT index, brush_alpha_index;
INT frame_id = 1;

    ToggleScreen((GX_WINDOW *)pEllipseWin, (GX_WINDOW *)pMainWin);
    pGraphicsWin = &pEllipseWin -> ellipse_window_graphics_window;
    pre_shape = draw_shape;
    draw_shape = ELLIPSE;

    /* Loop radius from 5 to 200. */
    for(a = 5; a < 200; a += 30)
    {
        for(b = 5; b < 200; b += 30)
        {
            for(brush_alpha_index = 0; brush_alpha_index < (INT)(sizeof(brush_alpha_value)/sizeof(INT)); brush_alpha_index++)
            {
                brush_alpha = brush_alpha_value[brush_alpha_index];

                for(index = 0; index < (INT)(sizeof(ellipse_props)/sizeof(ELLIPSE_PROPS)); index++)
                {
                    if((a < 30) || (b < 30))
                    {
                        brush_width = 1;
                    }
                    else
                    {
                        brush_width = ellipse_props[index].brush_width;
                    }

                    solid_fill = ellipse_props[index].solid_fill;
                    pixelmap_fill = ellipse_props[index].pixelmap_fill;
                    alpha = ellipse_props[index].alpha_mode;
                    compress = ellipse_props[index].compress_mode;
                    pixelmap_index = alpha + compress;
                    anti_aliased = ellipse_props[index].anti_aliased_mode;
                    /* Set frame id. */
                    gx_validation_set_frame_id(frame_id);

                    frame_id++;

                    /* Set test comments. */
                    sprintf(test_comment, "a:%d, b:%d, brush_width %d, solid_fill:%d, pixelmap_fill:%d, alpha:%d, compress: %d, brush_alpha: %d, anti_aiased: %d ", a, b, brush_width, solid_fill, pixelmap_fill, alpha, compress, brush_alpha, anti_aliased);

                    gx_validation_set_frame_comment(test_comment);

                    /* Mark graphics window dirty. */
                    gx_system_dirty_mark(pGraphicsWin);

                    /* Force a screen refresh.  */
                    gx_validation_screen_refresh();
                }
            }
        }
    }

    anti_aliased = GX_TRUE;
    brush_alpha = 255;
    brush_width = 1;

    for(index = 0; index < 4; index++)
    {
        switch(index)
        {
        case 0:
            a = 0;
            b = 100;
            sprintf(test_comment, "a = 0, b = 100");
            break;

        case 1:
            a = 100;
            b = 0;
            sprintf(test_comment, "a = 100, b = 0");
            break;

        case 2:
            a = 200;
            b = 100;
            root->gx_window_root_canvas->gx_canvas_display->gx_display_driver_pixel_blend = GX_NULL;
            sprintf(test_comment, "set pixel blend null, brush_width = 1");
            break;

        case 3:
            brush_width = 2;
            sprintf(test_comment, "set pixel blend null, brush_width = 2");
            break;
        }
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
    
   
    /* Signal the end of the test case, Verify the outout.  */
    gx_validation_end();

    exit(0);
}
