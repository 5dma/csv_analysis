#include <ctype.h>
#include <gtk/gtk.h>

#include <headers.h>
/**
 * @file make_headings.c
 * @brief Defines functions for creating the MySQL field names using column headings.
 * 
 * If possible, the application creates field names from column headings in the CSV file. If column headings are not in the CSV file, the application creates them using strings starting with `column_` followed by a number.
 */

/**
 * Callback for iterating over the raw headings read from the CSV file. The function does the following:
 * -# Removes trailing whitespace.
 * -# Transforms all alpha to lower case.
 * -# Replaces any character not in set [a-z-_] with underscore
 * @param original_heading_ptr Raw string.
 * @param data Pointer to the data-passer structure.
 */
void clean_column_headings(gpointer original_heading_ptr, gpointer data) {
	gchar *heading = (gchar *)original_heading_ptr;
	
	gchar *clean_string_step1 = g_strdup(heading); /* Memory freed below */
	g_strchomp(clean_string_step1);
	gchar *clean_string_step2 = g_ascii_strdown(clean_string_step1, -1);

	/* Examine each character; if not in set [a-z-_] the replace with underscore */
	char *ptr = clean_string_step2;

	while (*ptr) {
		/* Ignore non-print characters in the string, such as BOM in the first 2-3 bytes of a text file.
		We may need to also check g_unichar_iszerowidth */
		if (g_unichar_isprint(*ptr)) {
			if (!g_unichar_isalnum(*ptr) &&
				(*ptr != '_') &&
				(*ptr != '-')) {
				*ptr = '_';
			}
		}

		ptr++;
	}
	/* Copy the normalized string into the memory holding the original string. */
	strcpy(heading, clean_string_step2);
	g_free(clean_string_step2);
	g_free(clean_string_step1);
}
void crap(gpointer original_heading_ptr, gpointer data) {
	gchar *heading = (gchar *)original_heading_ptr;
	g_print("%p %s\n", &heading, heading);
}

/**
 * Makes individual headings from the first line in a CSV file, placing all of them in a `GSList`. This function relies on `strsep` to tokenize between tab characters.
 * @param csv_line First line from a CSV file.
 * @param field_quoting Type of quoting around the fields (never, always, optional).
 * @param data_passer Pointer to the data-passer structure.
 */
void make_headings(gchar *csv_line, enum field_quoting_options field_quoting, Data_passer *data_passer) {
	/* Need to understand why need a copy of csv_line; required by strsep? */
	gchar *local_csv_line = g_strdup(csv_line); /* Memory freed below */
	gchar *crawler = local_csv_line;
	char *token = NULL;

	/* Also need to understand why we need temporary_token. 
	Currently we copy the token into temporary_token, and then add temporary_token to
	the headings. Adding just the token to the list of headings generates a memory error,
	maybe a dangling pointer?
	 */
	while ((token = strsep(&crawler, "\t")) != NULL) {
		if (field_quoting != NEVER) {
			strip_quotes(&token);
		}
		data_passer -> headings = g_slist_append(data_passer -> headings, token);
		g_slist_foreach(data_passer -> headings, crap, NULL);
	}
	g_slist_foreach(data_passer -> headings, clean_column_headings, NULL);
	g_print("After cleaning column headings\n");
	g_slist_foreach(data_passer -> headings, crap, NULL);
	g_print("The heading length is %d\n", g_slist_length (data_passer->headings));

	g_free(crawler); /* Not sure this frees local_csv_line, because it moved during strsep. */
}

/**
 * Makes artifical column headings `column_00`, `column_01`, etc., placing all of them in a `GSList`.
 * @param csv_line First line from a CSV file.
 */
GSList *make_forced_headings(char *csv_line) {
	gchar *local_csv_line = strdup(csv_line);
	GSList *local_list = NULL;

	int number_columns = 0;
	while (strsep(&local_csv_line, "\t") != NULL) {
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
 * Removes quotes surrounding a value. The strip accounts for the possibility of escaped quotes surrounding the value, such as `"""123 Main Street"""`, which in output appears as `"123 Main Street".`
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

	if ((*quoted_string == '"') && (*(quoted_string + quoted_string_length - 1) == '"')) {
		gchar left_three_chars[4];
		gchar right_three_chars[4];

		g_utf8_strncpy(left_three_chars, quoted_string, 3);
		g_utf8_strncpy(right_three_chars, quoted_string + quoted_string_length - 3, 3);

		if ((g_strcmp0(left_three_chars, "\"\"\"") == 0) && (g_strcmp0(right_three_chars, "\"\"\"") == 0)) {
			unquoted = g_strdup(quoted_string + 2); /* Memory freed below */
			g_utf8_strncpy(quoted_string, unquoted, quoted_string_length - 3);
		} else {
			unquoted = g_strdup(quoted_string + 1);  /* Memory freed below */
			g_utf8_strncpy(quoted_string, unquoted, quoted_string_length - 2);
		}
		g_free(unquoted);
	}
}
