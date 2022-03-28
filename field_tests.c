#include <glib-2.0/glib.h>
#include <gtk/gtk.h>
#include <regex.h>
/**
 * @file field_tests.c
 * @brief Defines functions for analyzing the columns in a CSV file.
 *
 * The functions descrbed in this file are part of the analysis algorithm. For an overview of the algorithm, see \ref algorithm.
*/

/**
 * Checks if a passed value is an unsigned integer between a given min and max.
 * @param token A character string.
 * @param min Lower bound for the unsigned integer.
 * @param max Upper bound for the unsigned integer.
 * @return `TRUE` if the passed character string represents an unsigned integer, `FALSE` otherwise.
*/
gboolean is_unsigned_int(const gchar *token, gshort min, guint64 max) {
    GError *error = NULL;
    guint64 num = 0;
    gboolean success = g_ascii_string_to_unsigned(token, 10, min, max, &num, &error);
    if (success) {
        return TRUE;
    }
    return FALSE;
}

/**
 * Checks if a passed value is a signed integer between a given min and max.
 * @param token A character string.
 * @param min Lower bound for the signed integer.
 * @param max Upper bound for the signed integer.
 * @return `TRUE` if the passed character string represents a signed integer, `FALSE` otherwise.
*/
gboolean is_signed_int(const gchar *token, gint64 min, gint64 max) {
    GError *error = NULL;
    guint64 num = 0;
    gboolean success = g_ascii_string_to_signed(token, 10, min, max, &num, &error);
    if (success) {
        return TRUE;
    }
    return FALSE;
}

/**
 * Checks if a passed value is a decimal.
 * @param token A character string.
 * @param decimal_regex Compiled regex for a number with 0-2 numbers after a decimal point.
 * @return `TRUE` if the passed character string matches the pattern for a decimal, `FALSE` otherwise.
 * @see make_decimal_regex()
*/
gboolean is_decimal(const gchar *token, regex_t *decimal_regex) {
    regmatch_t pmatch[1];

    int success = regexec(decimal_regex, token, 1, pmatch, 0);
    g_print("For the value %s, the regecex result is %d\n", token, success);
    return (success == 0);
}

/**
 * Checks if a passed value is a float.
 * This function relies on [g_ascii_strtod](https://docs.gtk.org/glib/func.ascii_strtod.html), which returns `0.0` if a the token we are checking does not convert to a float. Therefore if this function returns `0.0` we might assume that the passed token is not a float. However, if we are examining a value whose column has already evaluated to a float, and we receive a token whose value is `0` or `0.0` or `0.00`, then we need to ensure the column continues to be considered a float. As a result, we call `g_ascii_strtod`, and then make the following checks if that function returns `0`:
 * - If so, is the token we received not one of the following: `0` or `0.0` or `0.00`
 * - Is the address of `token` plus the token's length != the `end_ptr` returned from `g_ascii_strtod`. 
 * 
 * If either of the two functions are true, then the zero returned from `g_ascii_strtod` is indeed a failed conversion, and the passed token is not a float.
 * @param token A character string.
 * @return `TRUE` if the passed character string can be converted to a float, `FALSE` otherwise.
*/
gboolean is_float(const gchar *token) {
    gchar *end_ptr;
    size_t token_length = strlen(token);
    gdouble result = g_ascii_strtod(token, &end_ptr);

    if (((result == 0) && 
    (g_ascii_strcasecmp(token, "0.00") != 0)) || 
    ((token + token_length) != end_ptr)) {
        return FALSE;
    } else {
        return TRUE;
    }
    
}

/**
 * Checks if a passed value is a timestamp.
 * @param token A character string.
 * @param timestamp_regex Compiled regex for a timestamp, such as a pattern YYYY-MM-DDThh:mm:ss:xxx.
 * @return `TRUE` if the passed character string matches the pattern for a timestamp, `FALSE` otherwise.
 * @see make_timestamp_regex()
*/
gboolean is_timestamp(const gchar *token, regex_t *timestamp_regex) {
    regmatch_t pmatch[2];

    int success = regexec(timestamp_regex, token, 1, pmatch, 0);
    return (success == 0) ? TRUE : FALSE;
}

