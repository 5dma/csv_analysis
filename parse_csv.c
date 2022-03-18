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
void change_commas_to_tabs_with_quotes(gchar **csv_line_ptr, gchar *delimiter) {
    gchar *csv_line = *csv_line_ptr;
    gchar *iterator = csv_line;
    gboolean inside_value = FALSE;
    /* Do not change the order of the following tests. */
    do {
        if (*iterator == '"') {
            if (iterator == csv_line) {
                inside_value = TRUE;
                iterator++;
                continue;
            }
            if (inside_value) {
                if (*(iterator + 1) == *delimiter) {
                    inside_value = FALSE;
                    iterator++;
                    continue;
                }
                if (((*(iterator - 1) == '"') && (*(iterator + 1) == '"')) ||
                    ((*(iterator - 1) == '"') && (*(iterator - 2) == '"')) ||
                    ((*(iterator + 1) == '"') && (*(iterator + 2) == '"'))) {
                    iterator++;
                    continue;
                }
                iterator++;
                continue;
            } else {
                inside_value = TRUE;
            }
        } else {
            if (!inside_value) {
                if (*iterator == '"') {
                    inside_value = TRUE;
                }
                if (*iterator == ',') {
                    *iterator = '\t';
                }
                iterator++;
                continue;
            }
        }
        iterator++;
    } while (*iterator != '\0');
}
/**
 * Inspects each character in the passed line, and replaces commas with tabs.
 * @param csv_line_ptr Double-pointer to a line from a CSV file.
 */
void change_commas_to_tabs(gchar **csv_line_ptr) {
    gchar *csv_line = *csv_line_ptr;
    gchar *iterator = csv_line;

    while (*iterator != '\0') {
        if ((*iterator == ',') && (*(iterator - 1) == '"') && (*(iterator + 1) == '"')) {
            *iterator = '\t';
        }
        iterator++;
    }

    /* Use the following when get GLIB 2.68
    guint replacements = g_string_replace(newstring, ",", "\t", 0); */
}


