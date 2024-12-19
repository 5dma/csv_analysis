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
	} else {
		g_error_free (error);
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
	gint64 num = 0;
	gboolean success = g_ascii_string_to_signed(token, 10, min, max, &num, &error);
	if (success) {
		return TRUE;
	} else {
		g_error_free (error);
	}
	return FALSE;
}

/**
 * Checks if a passed value is a decimal.
 * A number with more than two digits after the decimal point is considered a `FLOAT`.
 * 
 * @param token A character string.
 * @param decimal_regex Compiled regex for a number with 0-2 numbers after a decimal point.
 * @return `TRUE` if the passed character string matches the pattern for a decimal, `FALSE` otherwise.
 * @see make_decimal_regex()
*/
gboolean is_decimal(const gchar *token, regex_t *decimal_regex) {
	regmatch_t pmatch[2];
	int success = regexec(decimal_regex, token, 1, pmatch, 0);
	return (success == 0);
}

/**
 * Checks if a passed value is an SQLite `NULL`.
 * @param token A character string.
 * @return `TRUE` if the passed character string's first character is `\0`, `FALSE` otherwise.
*/
gboolean is_null(const gchar *token) {
	return (*token == '\0');
}


/**
 * Checks if a passed value is a float.
 * This function relies on [g_ascii_strtod](https://docs.gtk.org/glib/func.ascii_strtod.html). This function scans the passed token, and returns the float (or double) corresponding to all the characters that can comprise a float as well as the pointer to the last such character. If the function cannot do any such conversation (such as if the first character is alphabetic), the function returns `0.0`. 
 * 
 * As some tokens passed to this function may be alphanumeric (such as `6c4759f60000`), we have to ensure that the function does not return a false result. For example, if we pass `6c4759f60000`, the return value of `g_ascii_strtod` is `6` and the returned end pointer also points to `6`. We therefore need to ensure that the end pointer points to the actual last character in the token. If it doesn't we know that the conversion failed. Furthermore, `g_ascii_strtod` returns `0` if the conversion fails, so we need to check the first character of the token to verify it isn't zero. To summarize:
 * - If `g_ascii_strtod` returned a non-zero number and the end pointer matches the end of the token, return `TRUE`.
 * - If `g_ascii_strtod` returned a non-zero number and the end pointer does not match the end of the token, return `FALSE`.
 * - If `g_ascii_strtod` returned a zero and the first character is a zero, return `TRUE`.
 * - If `g_ascii_strtod` returned a zero and the first character is not a zero, return `FALSE`.
 *
 * @param token A character string.
 * @return `TRUE` if the passed character string can be converted to a float, `FALSE` otherwise.
*/
gboolean is_float(const gchar *token) {
	gchar *end_ptr;
	size_t token_length = strlen(token);
	gdouble result = g_ascii_strtod(token, &end_ptr);

 if (((result == 0) && (*token != '0')) || (token + token_length != end_ptr)) {
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

