#include <gtk/gtk.h>

#include "headers.h"
/**
 * @file initialize_field_analysis.c
 * @brief Initializes a Field_analysis struct.
 *
 * Each column (field) in a CSV file has its own analysis. When initialized, the algorithm assumes the column is the smallest possible MySQL data type, `TINYINT` with a value of `0`, so the instantiation of each `Field_analysis` structure includes the `TINYINT` data type.
 */

/**
 * Instantiates a `Field_analysis` struct and adds it to the field analysis hash. The following table describes how a field analysis is initialized.
 * <table>
 * <tr><th>Member</th><th>Value</th></tr>
 * <tr><td>field_type</td><td>`TINYINT_UNSIGNED`, which is the smallest MySQL data type.</td></tr>
 * <tr><td>sql_signed</td><td>`FALSE`, which indicates the value is unsigned. While this is implied in the previous member, we need a field to track if a column contains signed or unsigned values.</td></tr>
 * <tr><td>char_width</td><td>`0`, the smallest possible length of a `CHAR` column.</td></tr>
 * <tr><td>precision</td><td>`0`, the smallest possible precision of a `DECIMAL` column.</td></tr>
 * <tr><td>scale</td><td>`0`, the smallest possible scale of a `DECIMAL` column.</td></tr>
 * <tr><td>last_line_change</td><td>If CSV file does not have a heading, set to 1. If CSV file has a heading, set to 2.</td></tr>
 * <tr><td>determining_value</td><td>`0`, an initial number.</td></tr>
 * </table>
 * @param heading Column for which we are instantiating a struct.
 * @param data Pointer to the data-passer structure.
 */
void initialize_field_analysis(gpointer heading, gpointer data) {
    gchar *key = (gchar *)heading;

    Data_passer *data_passer = (Data_passer *)data;

    GHashTable *field_analysis_hash = data_passer->field_analysis_hash;

    gboolean has_header_line = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data_passer->checkbox_has_headers));

    Field_analysis *field_analysis_struct = g_new(Field_analysis, 1);

    field_analysis_struct->field_type = TINYINT_UNSIGNED;
    field_analysis_struct->sql_signed = FALSE;
    field_analysis_struct->char_width = 0;
    field_analysis_struct->precision = 0;
    field_analysis_struct->scale = 0;
    field_analysis_struct->last_line_change = (has_header_line)  ? 2 : 1;
    g_strlcpy( field_analysis_struct->determining_value,"0", 4096);
    /*field_analysis_struct->determining_value implicitly assigned NULL in all positions. */

    gboolean success = g_hash_table_insert(field_analysis_hash, key, field_analysis_struct);
    if (success == FALSE) {
        g_print("Critical! The key %s is a duplicate. Ensure all your column headings are unique.\n", key);
    }
}

void assign_char_field_type(const char *csv_value, const int current_line_number, Field_analysis *field_analysis) {
    field_analysis->field_type = CHAR;
    guint csv_value_length = strlen(csv_value);
    if (field_analysis->char_width < csv_value_length) {
        field_analysis->char_width = csv_value_length;
        field_analysis->last_line_change = current_line_number;
        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
    }
}