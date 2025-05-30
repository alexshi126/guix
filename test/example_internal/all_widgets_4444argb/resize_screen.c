/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "gx_canvas.h"

#include "all_widgets_4444argb_resources.h"
#include "all_widgets_4444argb_specifications.h"

extern VOID memory_free(VOID *mem);

float            x_ratio = 1;
float            y_ratio = 1;
GX_RESOURCE_ID   resize_pixelmap_id = GX_PIXELMAP_ID_ICON_FOOT_ALPHA;

VOID update_prompt_text(GX_RESOURCE_ID id, float value)
{
    GX_PROMPT *pp;
    static GX_CHAR text_buffer[10];
    GX_STRING text;
    INT index = 0;

    gx_widget_find((GX_WIDGET*)& resize_screen, (USHORT)id, 0, &pp);

    if (pp)
    {
        /* Transfer float value to string. */
        text_buffer[index++] = '0' + (int)(value);
        text_buffer[index++] = '.';
        text_buffer[index++] = '0' + (int)(value * 10) % 10;
        text_buffer[index++] = '0' + (int)(value * 100) % 10;
        text_buffer[index] = '\0';

        /* Reset prompt text. */
        text.gx_string_ptr = text_buffer;
        text.gx_string_length = strnlen(text_buffer, sizeof(text_buffer));
        gx_prompt_text_set_ext(pp, &text);
    }
}

UINT resize_screen_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
    UINT status = 0;
    INT  value;

    switch (myevent->gx_event_type)
    {
    case GX_SIGNAL(ID_SLIDER_WIDTH, GX_EVENT_SLIDER_VALUE):
        value = myevent->gx_event_payload.gx_event_longdata;
        value -= 50;
        x_ratio = 1 + (float)value / 100;
        update_prompt_text(ID_WIDTH_DISPLAY, x_ratio);
        gx_system_dirty_mark((GX_WIDGET *)&resize_screen.resize_screen_resize_window);
        break;

    case GX_SIGNAL(ID_SLIDER_HEIGHT, GX_EVENT_SLIDER_VALUE):
        value = myevent->gx_event_payload.gx_event_longdata;
        value -= 50;
        y_ratio = 1 + (float)value / 100;
        update_prompt_text(ID_HEIGHT_DISPLAY, y_ratio);
        gx_system_dirty_mark((GX_WIDGET *)&resize_screen.resize_screen_resize_window);
        break;

    case GX_SIGNAL(ID_RADIO_FOOT, GX_EVENT_RADIO_SELECT):
        resize_pixelmap_id = GX_PIXELMAP_ID_ICON_FOOT_ALPHA;
        gx_system_dirty_mark((GX_WIDGET *)&resize_screen.resize_screen_resize_window);
        break;

    case GX_SIGNAL(ID_RADIO_FOOT_ALPHA, GX_EVENT_RADIO_SELECT):
        resize_pixelmap_id = GX_PIXELMAP_ID_RED_APPLE;
        gx_system_dirty_mark((GX_WIDGET *)&resize_screen.resize_screen_resize_window);
        break;

    case GX_SIGNAL(ID_RADIO_FISH, GX_EVENT_RADIO_SELECT):
        resize_pixelmap_id = GX_PIXELMAP_ID_ROTATE_FISH;
        gx_system_dirty_mark((GX_WIDGET *)&resize_screen.resize_screen_resize_window);
        break;

    default:
        status = next_button_handler(window, myevent);
        break;
    }
    return status;
}

VOID resize_window_draw(GX_WINDOW *window)
{
    INT  xpos;
    INT  ypos;
    GX_PIXELMAP *pixelmap;
    GX_RECTANGLE win_size;
    GX_PIXELMAP destination;
    INT width, height;

    gx_window_draw((GX_WINDOW*)window);

    gx_context_pixelmap_get(resize_pixelmap_id, &pixelmap);
    win_size = resize_screen.resize_screen_resize_window.gx_widget_size;

    xpos = (win_size.gx_rectangle_right + win_size.gx_rectangle_left -pixelmap->gx_pixelmap_width) >> 1;
    ypos = (win_size.gx_rectangle_top + win_size.gx_rectangle_bottom - pixelmap->gx_pixelmap_height) >> 1;

    width = (int)(pixelmap->gx_pixelmap_width * x_ratio);
    height = (int)(pixelmap->gx_pixelmap_height * y_ratio);

    if (x_ratio == 1 && y_ratio == 1)
    {
        /* Draw source pixelmap.  */
        gx_canvas_pixelmap_draw(xpos, ypos, pixelmap);
    }
    else if (gx_utility_pixelmap_resize(pixelmap, &destination, width, height) == GX_SUCCESS)
    {
        xpos += (pixelmap->gx_pixelmap_width - destination.gx_pixelmap_width) >> 1;
        ypos += (pixelmap->gx_pixelmap_height - destination.gx_pixelmap_height) >> 1;

        /* Draw resized pixelmap.  */
        gx_canvas_pixelmap_draw(xpos, ypos, &destination);

        if (destination.gx_pixelmap_data)
        {
            memory_free((VOID *)destination.gx_pixelmap_data);
        }

        if (destination.gx_pixelmap_aux_data)
        {
            memory_free((VOID *)destination.gx_pixelmap_aux_data);
        }
    }
}
