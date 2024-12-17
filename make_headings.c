#include <ctype.h>
#include <gtk/gtk.h>
#include <headers.h>
/**
 * @file make_headings.c
 * @brief Defines functions for creating the MySQL field names using column
 * headings.
 *
 * If possible, the application creates field names from column headings in the
 * CSV file. If column headings are not in the CSV file, the application
 * creates them using strings starting with `column_` followed by a number.
 */

/**
 * Cleans a heading in place. The function does the following:
 * -# Removes trailing whitespace.
 * -# Transforms all alpha to lower case.
 * -# Removes Byte-Order-Mark (BOM) characters that may be in the first heading.
 * -# Replaces any character not in set [a-z-_] with underscore
 * @param heading Double pointer to the passed heading.
 */
void clean_column_heading(gchar **heading) {

	gchar *clean_string_step1 = g_strdup(*heading); /* Memory freed below */
	g_strchomp(clean_string_step1);
	gchar *clean_string_step2 = g_ascii_strdown(clean_string_step1, -1);

	/* Examine each character; if not in set [a-z-_] the replace with underscore
	 */

	char *ptr = clean_string_step2;

	/* Remove non-print characters in the string, such as BOM in the first 2-3 bytes of a text file.
	The "removal" actually shifts the characters so that they override the BOMs. */
	if (!g_unichar_isprint(*ptr)) {
		gchar *tracker = ptr + 1;
		while (*tracker != '\0') {
			if (g_unichar_isprint(*tracker)) {
				*ptr = *tracker;
				ptr++;
			}
			tracker++;
		}
		*ptr = '\0';
	}

	/* Replace non-alphanum/dash characters with underscore. */
	ptr = clean_string_step2;
	while (*ptr != '\0') {
		if (!g_ascii_isalnum(*ptr) && (*ptr != '_') && (*ptr != '-')) {
			*ptr = '_';
		}
		ptr++;
	}

	/* Copy the normalized string into the memory holding the original string. */
	g_strlcpy(*heading, clean_string_step2, sizeof(*heading));
	g_free(clean_string_step2);
	g_free(clean_string_step1);
}

/**
 * Makes individual headings from the first line in a CSV file, placing all of
 * them in a `GSList`. This function relies on `strsep` to tokenize between tab
 * characters.
 * @param csv_line First line from a CSV file.
 * @param field_quoting Type of quoting around the fields (never, always, optional).
 * @return A `GSList` of character arrays.
 */
GSList *make_headings(gchar *csv_line, enum field_quoting_options field_quoting) {
	/* Need to understand why need a copy of csv_line; required by strsep? */
	gchar *crawler = csv_line;
	gchar *token = NULL;
	GSList *local_list = NULL;

	while ((token = strsep(&crawler, "\t")) != NULL) {
		if (field_quoting != NEVER) {
			strip_quotes(&token);
		}
		clean_column_heading(&token);
		gchar *actual_entry = g_strdup(token); /* Call these entries are freed in cleanup. */
		local_list = g_slist_append(local_list, actual_entry);
	}
	return local_list;
}

/**
 * Makes artifical column headings `column_00`, `column_01`, etc., placing all of them in a `GSList`.
 * @param csv_line First line from a CSV file.
 * @return A `GSList` of character arrays.
 */
GSList *make_forced_headings(char *csv_line) {
	GSList *local_list = NULL;

	int number_columns = 0;
	while (strsep(&csv_line, "\t") != NULL) {
		number_columns++;
	}

	gchar *prefix = strdup("column_");
	gchar suffix[3];
	for (gdouble i = 0; i < number_columns; i++) {
		g_ascii_formatd(suffix, sizeof(suffix), "%02.0f", i);
		gchar *buffer = g_strconcat(prefix, suffix, NULL);
		local_list = g_slist_append(local_list, buffer);
	}
	g_free(prefix);
	return local_list;
}

/**
 * Removes quotes surrounding a value. The strip accounts for the possibility
 * of escaped quotes surrounding the value, such as `"""123 Main Street"""`,
 * which in output appears as `"123 Main Street".`
 * @param quoted_string_ptr Passed value from the CSV file.
 */
void strip_quotes(gchar **quoted_string_ptr) {
	/*
	If so, do the following:
	a) Find the quoted string's length.
	b) Duplicate the quoted string starting from the character AFTER the first quote.
	c) Copy the duplicated string back into csv_value EXCEPT for the last quote.
	d) Free the duplicate quoted string.
	*/

	gchar *quoted_string = *quoted_string_ptr;
	glong quoted_string_length = g_utf8_strlen(quoted_string, -1);
	gchar *unquoted;

	if ((*quoted_string == '"') &&
		(*(quoted_string + quoted_string_length - 1) == '"')) {
		gchar left_three_chars[4];
		gchar right_three_chars[4];

		g_utf8_strncpy(left_three_chars, quoted_string, 3);
		g_utf8_strncpy(right_three_chars, quoted_string + quoted_string_length - 3, 3);

		if ((g_strcmp0(left_three_chars, "\"\"\"") == 0) &&
			(g_strcmp0(right_three_chars, "\"\"\"") == 0)) {
			unquoted = g_strdup(quoted_string + 2); /* Memory freed below */
			g_utf8_strncpy(quoted_string, unquoted, quoted_string_length - 3);
		} else {
			unquoted = g_strdup(quoted_string + 1); /* Memory freed below */
			g_utf8_strncpy(quoted_string, unquoted, quoted_string_length - 2);
		}
		g_free(unquoted);
	}
}
