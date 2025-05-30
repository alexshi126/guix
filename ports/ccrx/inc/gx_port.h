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
/**   Port Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/*                                                                        */
/*  APPLICATION INTERFACE DEFINITION                       RELEASE        */
/*                                                                        */
/*    gx_port.h                                            RX/CCRX        */
/*                                                           6.1.9        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file contains data type definitions and constants that work    */
/*    with the Renesas CCRX compiler.                                     */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  10-15-2021     Kenneth Maxwell          Initial Version 6.1.9         */
/*                                                                        */
/**************************************************************************/

#ifndef GX_PORT_H
#define GX_PORT_H

/* Determine if the optional GUIX user define file should be used.  */

#ifdef GX_INCLUDE_USER_DEFINE_FILE

/* Include the user defines in gx_user.h. The defines in this file may
   alternately be defined on the command line.  */

#include "gx_user.h"
#endif

typedef INT    GX_BOOL;
typedef SHORT  GX_VALUE;
typedef unsigned char GX_UCHAR;

#define NULL   ((void *)(0))
#include "string.h"

#define GX_VALUE_MAX                        0x7FFF

#define GX_DISABLE_DEPRECATED_STRING_API

/* Define the basic system parameters.  */

#ifndef GX_THREAD_STACK_SIZE
#define GX_THREAD_STACK_SIZE                4096
#endif

#define GX_CONST                            const

#define GX_INCLUDE_DEFAULT_COLORS

#define GX_PALETTE_MODE_AA_TEXT_COLORS      16

#define GX_MAX_ACTIVE_TIMERS                32

#define GX_MAX_VIEWS                        32

#define GX_MAX_DISPLAY_HEIGHT               800


/* Define several macros for the error checking shell in GUIX.  */

#ifdef GX_DISABLE_THREADX_BINDING

/* Here if not running with ThreadX RTOS */
#define GX_CALLER_CHECKING_EXTERNS
#define GX_THREADS_ONLY_CALLER_CHECKING
#define GX_INIT_AND_THREADS_CALLER_CHECKING
#define GX_NOT_ISR_CALLER_CHECKING          
#define GX_THREAD_WAIT_CALLER_CHECKING      

#else

#ifdef TX_TIMER_PROCESS_IN_ISR
#define GX_CALLER_CHECKING_EXTERNS          extern  TX_THREAD           *_tx_thread_current_ptr; \
                                            extern  volatile ULONG      _tx_thread_system_state;

#define GX_THREADS_ONLY_CALLER_CHECKING     if ((TX_THREAD_GET_SYSTEM_STATE()) || \
                                                (_tx_thread_current_ptr == TX_NULL)) \
                                                return(GX_CALLER_ERROR);

#define GX_INIT_AND_THREADS_CALLER_CHECKING if (((TX_THREAD_GET_SYSTEM_STATE()) && ((TX_THREAD_GET_SYSTEM_STATE()) < ((ULONG) 0xF0F0F0F0)))) \
                                                return(GX_CALLER_ERROR);

#define GX_NOT_ISR_CALLER_CHECKING          if ((TX_THREAD_GET_SYSTEM_STATE()) && ((TX_THREAD_GET_SYSTEM_STATE()) < ((ULONG) 0xF0F0F0F0))) \
                                                return(GX_CALLER_ERROR);

#define GX_THREAD_WAIT_CALLER_CHECKING      if ((wait_option) && \
                                               ((_tx_thread_current_ptr == NX_NULL) || (TX_THREAD_GET_SYSTEM_STATE()))) \
                                            return(GX_CALLER_ERROR);

#else

#define GX_CALLER_CHECKING_EXTERNS          extern  TX_THREAD           *_tx_thread_current_ptr; \
                                            extern  TX_THREAD           _tx_timer_thread; \
                                            extern  volatile ULONG      _tx_thread_system_state;

#define GX_THREADS_ONLY_CALLER_CHECKING     if ((TX_THREAD_GET_SYSTEM_STATE()) || \
                                                (_tx_thread_current_ptr == TX_NULL) || \
                                                (_tx_thread_current_ptr == &_tx_timer_thread)) \
                                                return(GX_CALLER_ERROR);

#define GX_INIT_AND_THREADS_CALLER_CHECKING if (((TX_THREAD_GET_SYSTEM_STATE()) && ((TX_THREAD_GET_SYSTEM_STATE()) < ((ULONG) 0xF0F0F0F0))) || \
                                                (_tx_thread_current_ptr == &_tx_timer_thread)) \
                                                return(GX_CALLER_ERROR);


#define GX_NOT_ISR_CALLER_CHECKING          if ((TX_THREAD_GET_SYSTEM_STATE()) && ((TX_THREAD_GET_SYSTEM_STATE()) < ((ULONG) 0xF0F0F0F0))) \
                                                return(GX_CALLER_ERROR);

#define GX_THREAD_WAIT_CALLER_CHECKING      if ((wait_option) && \
                                               ((_tx_thread_current_ptr == NX_NULL) || (TX_THREAD_GET_SYSTEM_STATE()) || (_tx_thread_current_ptr == &_tx_timer_thread))) \
                                            return(GX_CALLER_ERROR);

#endif
#endif

/* Define the version ID of GUIX.  This may be utilized by the application.  */

#ifdef GX_SYSTEM_INIT
CHAR _gx_version_id[] =
    "Copyright (c) 2024 Microsoft Corporation.  *  GUIX Version 6.4.1 *";
#else
extern  CHAR _gx_version_id[];
#endif

#endif

