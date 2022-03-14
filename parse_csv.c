#include <glib-2.0/glib.h>
#include <gtk/gtk.h>
/**
 * @file parse_csv.c
 * @brief Provides functions for parsing and adjusting lines in a CSV file.
 */


/**
 * Inspects each character in the passed line, and replaces commas with tabs.
 * @param csv_line_ptr Double-pointer to a line from a CSV file.
 */
void change_commas_to_tabs_unquoted(gchar **csv_line_ptr) {

    gchar *csv_line = *csv_line_ptr;
    gchar *iterator = csv_line;

    while (*iterator != '\0') {
        if (*iterator == ',') {
            *iterator = '\t';
        }
        iterator++;
    }

    /* Use the following when get GLIB 2.68
    guint replacements = g_string_replace(newstring, ",", "\t", 0); */
}

void change_quoted_strings_to_tab_delimiter(gchar **csv_line_ptr, gchar *delimiter) {
    gchar delimiter_phrase[4] = {'"', '\t', '"', '\0'};
    ;

    if (g_strcmp0(delimiter, ",") == 0) {
        delimiter_phrase[1] = ',';
    }

    gchar *csv_line = *csv_line_ptr;

    // guint replacements = g_string_replace(csv_line, delimiter_phrase, "\t", 0);

    char **split = g_strsplit(csv_line, delimiter_phrase, -1);
    g_free(csv_line);
    csv_line = g_strjoinv("\t", split);
    g_strfreev(split);

    glong csv_line_length = g_utf8_strlen(csv_line, -1);

    *(csv_line + csv_line_length - 1) = '\0';
    csv_line++;
}
