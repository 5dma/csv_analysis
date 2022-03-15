#include <gtk/gtk.h>
#include "headers.h"
/**
 * @file initialize_field_analysis.c
 * @brief Initializes a Field_analysis struct.
*/

/**
 * Instantiates a `Field_analysis` struct and adds it to the field analysis hash. The key is the passed heading, and the value is the new struct.
 * @param heading Column for which we are instantiating a struct.
 * @param data Pointer to the data-passer structure.
*/
void initialize_field_analysis(gpointer heading, gpointer data) {
    gchar *key = (gchar *)heading;
    GHashTable *field_analysis_hash = (GHashTable *) data;

    Field_analysis *field_analysis_struct = g_new(Field_analysis, 1);

    field_analysis_struct->field_type = TINYINT_UNSIGNED;
    field_analysis_struct->sql_signed = FALSE;
    field_analysis_struct->char_width = 0;
    field_analysis_struct->precision = 0;
    field_analysis_struct->scale = 0;
    field_analysis_struct->last_line_change = 1;

    gboolean success = g_hash_table_insert(field_analysis_hash, key, field_analysis_struct);
    if (success == FALSE) {
        g_print("Warning! A key was replaced in the hash table!\n");
    }
}
