/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_scroll_wheel.h"

TEST_PARAM test_parameter = {
    "guix_prompt_text_set_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

static VOID      control_thread_entry(ULONG);

VOID tx_application_define(void *first_unused_memory)
{
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
}


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

static VOID control_thread_entry(ULONG input)
{
int                       failed_tests = 0;
UINT                      status;
GX_RECTANGLE              size;
GX_STRING                 string;
GX_CONST GX_CHAR          test_string[] = "test";
GX_PROMPT                 prompt;
#ifdef GX_ENABLE_DEPRECATED_STRING_API
GX_CHAR invalid_string[GX_MAX_STRING_LENGTH + 1];
#endif
    status = gx_prompt_text_set_ext(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&prompt, 0, sizeof(GX_PROMPT));
    gx_utility_rectangle_define(&size, 0, 0, 100, 100);
    gx_prompt_create(&prompt, "", GX_NULL, 0, 0, 1024, &size);

#ifdef GX_ENABLE_DEPRECATED_STRING_API
    memset(invalid_string, 'x', sizeof(invalid_string));
    status = gx_prompt_text_set(&prompt, invalid_string);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);
#endif
    string.gx_string_ptr = test_string;
    string.gx_string_length = sizeof(test_string) - 2;
    status = gx_prompt_text_set_ext(&prompt, &string);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    string.gx_string_ptr = GX_NULL;
    string.gx_string_length = 2;
    status = gx_prompt_text_set_ext(&prompt, &string);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    string.gx_string_ptr = test_string;
    string.gx_string_length = sizeof(test_string) - 1;
    status = gx_prompt_text_set_ext(&prompt, &string);
    EXPECT_EQ(GX_SUCCESS, status);

    /* Test if text been set successfully. */
    gx_prompt_text_get_ext(&prompt, &string);
    if(strcmp(string.gx_string_ptr, test_string) != 0)
    {
        failed_tests++;
    }

    status = gx_prompt_text_set_ext(&prompt, GX_NULL);
    EXPECT_EQ(GX_SUCCESS, status);

    /* Test if text been set successfully. */
    gx_prompt_text_get_ext(&prompt, &string);
    if(string.gx_string_ptr)
    {
        failed_tests++;
    }
    
    if(failed_tests == 0) 
    {
        gx_validation_print_test_result(TEST_SUCCESS);
        exit(0);
    }
    else 
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
}
