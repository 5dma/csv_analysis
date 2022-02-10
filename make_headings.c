#include <ctype.h>
#include <gtk/gtk.h>
/**
 * @file make_headings.c
 * @brief Defines functions for creating the MySQL column headings.
*/

/**
 * Callback for iterating over the raw headings read from the CSV file. The function does the following:
 * -# Removes trailing whitespace.
 * -# Transforms all alpha to lower case.
 * -# Replaces any character not in set [a-z-_] with underscore
 * @param original_heading_ptr Raw string.
 * @param data Pointer to the pointer-passer hash.
*/
void clean_column_headings(gpointer original_heading_ptr, gpointer data) {
    char *heading = (gchar *)original_heading_ptr;
    char *clean_string;

    clean_string = strdup(heading); /* Memory freed below */
    clean_string = g_strchomp(clean_string);
    clean_string = g_ascii_strdown(clean_string, -1);

    /* Examine each character; if not in set [a-z-_] the replace with underscore */
    char *ptr = clean_string;

    while (*ptr) {
        if (!isalnum(*ptr) &&
            (*ptr != '_') &&
            (*ptr != '-')) {
            *ptr = '_';
        }
        ptr++;
    }

    /* Copy the normalized string into the memory holding the original string. */
    strcpy(heading, clean_string);
    g_free(clean_string);
}

/**
 * Makes individual headings from the first line in a CSV file, placing all of them in a `GSList`. This function relies on `strsep` to tokenize between tab characters.
 * @param csv_line First line from a CSV file.
*/
GSList *make_headings(char *csv_line) {
    char *token = NULL;
    char *delimiter = "\t";
    GSList *local_list = NULL;
    gchar *heading = NULL;
    while ((token = strsep(&csv_line, delimiter)) != NULL) {
        heading = strdup(token); /* Why is the heading variable necessary? */
        local_list = g_slist_append(local_list, heading);
    }

    g_slist_foreach(local_list, clean_column_headings, NULL);
    return local_list;
}

/**
 * Makes artifical column headings `column_00`, `column_01`, etc., placing all of them in a `GSList`.
 * @param csv_line First line from a CSV file.
*/
GSList *make_forced_headings(char *csv_line) {
    char *token;
    char *delimiter = "\t";
    GSList *local_list = NULL;

    int number_columns = 0;
    while (strsep(&csv_line, delimiter) != NULL) {
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
