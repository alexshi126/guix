
/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Dispaly Management (Dispaly)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_16bpp_mouse_restore              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service restores captured memory under the mouse area.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Display control block         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
#if defined(GX_MOUSE_SUPPORT)
#if !defined(GX_HARDWARE_MOUSE_SUPPORT)
VOID _gx_display_driver_16bpp_mouse_restore(GX_DISPLAY *display)
{
INT          row;
INT          column;
INT          height;
INT          width;
USHORT      *putrow;
USHORT      *put;
USHORT      *get;
GX_CANVAS   *canvas;

    if(display -> gx_display_mouse.gx_mouse_cursor_info)
    {
        if (display -> gx_display_mouse.gx_mouse_capture_memory &&
            (display -> gx_display_mouse.gx_mouse_status & GX_MOUSE_VISIBLE))
        {
            canvas = display -> gx_display_mouse.gx_mouse_canvas;
            height = display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_bottom - display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_top + 1;
            width = display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_right - display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_left + 1;

            if (width > 0 && height > 0)
            {
                get = (USHORT *)display -> gx_display_mouse.gx_mouse_capture_memory;
                putrow = (USHORT *)canvas -> gx_canvas_memory;
                putrow += canvas -> gx_canvas_x_resolution * display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_top;
                putrow += display -> gx_display_mouse.gx_mouse_rect.gx_rectangle_left;

                for (row = 0; row < height; row++)
                {
                    put = putrow;
                    for (column = 0; column < width; column++)
                    {
                        *put++ = *get++;
                    }
                    putrow += display -> gx_display_width;
                }
            }
        }
        display -> gx_display_mouse.gx_mouse_status &= (GX_UBYTE)(~GX_MOUSE_VISIBLE);
    }
}
#endif
#endif

