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
/**   Win32 Display Management (Display)                                  */
/**                                                                       */
/**************************************************************************/
#ifdef WIN32
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "windows.h"
#include "gx_win32_display_driver.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    win32_24xrgb_bitmap_header_create                   PORTABLE C      */
/*                                                           6.1.4        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates bitmap header for 24xrgb driver.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Pointer to GX_DISPLAY         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    win32_graphics_driver_setup_24xrgb                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  02-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            Renamed function,           */
/*                                            resulting in version 6.1.4  */
/*                                                                        */
/**************************************************************************/
VOID win32_32bpp_bitmap_header_create(GX_DISPLAY *display)
{
GX_WIN32_DISPLAY_DRIVER_DATA *instance;
DWORD                        *putmask;

    instance = (GX_WIN32_DISPLAY_DRIVER_DATA *)display -> gx_display_driver_data;

    instance -> win32_driver_bmpinfo.gx_bmp_header.biSize = sizeof(BITMAPINFOHEADER);
    instance -> win32_driver_bmpinfo.gx_bmp_header.biWidth = display -> gx_display_width;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biHeight = display -> gx_display_height;

    instance -> win32_driver_bmpinfo.gx_bmp_header.biPlanes = 1;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biBitCount = 32;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biSizeImage = display -> gx_display_width * display -> gx_display_height * 4;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biClrUsed = 16777215;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biClrImportant = 16777215;
    instance -> win32_driver_bmpinfo.gx_bmp_header.biCompression = BI_BITFIELDS;

    putmask = (DWORD *)&(instance -> win32_driver_bmpinfo.gx_bmp_colors[0]);

    *putmask++ = 0x00ff0000;
    *putmask++ = 0x0000ff00;
    *putmask   = 0x000000ff;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    win32_graphics_driver_setup_24xrgb                  PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a Windows specific 24xrgb display driver.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    display                               Pointer to GX_DISPLAY         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_24xrgb_setup       guix display setup funciton.  */
/*    win32_24xrgb_bitmap_header_create     Create bitmap header info     */
/*    gx_win32_get_free_data_instance       Get display data instance     */
/*    GX_WIN32_EVENT_THREAD_CREATE          Create event thread           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Kenneth Maxwell          Initial Version 6.1.3         */
/*  02-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            Updated function name,      */
/*                                            resulting in version 6.1.4  */
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            improved logic,             */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display)
{
GX_WIN32_DISPLAY_DRIVER_DATA *data;

    /* Initialize the low-level drawing function pointers.
       For windows, these are always just the generic funcions,
       but for some hardware, these will be customized,
       optimized functions specific to that hardware. */
    data = gx_win32_get_free_data_instance();

    if (!data)
    {
        /* We don't have any free display data instance. */
        return(GX_FAILURE);
    }

    /* Save off the format of display driver. */
    data -> win32_driver_type = GX_COLOR_FORMAT_24XRGB;

    _gx_display_driver_24xrgb_setup(display, data, gx_win32_display_buffer_toggle);

    /* Create bitmap header for 24xrgb display driver. */
    win32_32bpp_bitmap_header_create(display);

    /* Create the GUIX / Windows event thread
       This thread is a substitute for a touch screen
       or keyboard driver thread that would be running
       on embedded hardware. */
    GX_WIN32_EVENT_THREAD_CREATE(data, "GUI-WIN32-24xrgb");

    return(GX_SUCCESS);
}
#endif /* WIN32 */

