#include <gtk/gtk.h>
#include "headers.h"
/**
 * @file initialize_field_analysis.c
 * @brief Initializes a Field_analysis struct.
 * 
 * Each column (field) in a CSV file has its own analysis. When initialized, the algorithm assumes the column is the smallest possible MySQL data type, `TINYINT`, so the instantiation of each `Field_analysis` structure includes the `TINYINT` data type.
*/

/**
 * Instantiates a `Field_analysis` struct and adds it to the field analysis hash. The key is the passed heading, and the value is the new struct.
 * @param heading Column for which we are instantiating a struct.
 * @param data Pointer to the data-passer structure.
*/
void initialize_field_analysis(gpointer heading, gpointer data) {
    gchar *key = (gchar *)heading;

    Data_passer *data_passer = (Data_passer *)data;

    GHashTable *field_analysis_hash = data_passer -> field_analysis_hash;

    Field_analysis *field_analysis_struct = g_new(Field_analysis, 1);

    field_analysis_struct->field_type = TINYINT_UNSIGNED;
    field_analysis_struct->sql_signed = FALSE;
    field_analysis_struct->char_width = 0;
    field_analysis_struct->precision = 0;
    field_analysis_struct->scale = 0;
    field_analysis_struct->last_line_change = 1;
    /*field_analysis_struct->determining_value implicitly assigned NULL in all positions. */

    gboolean success = g_hash_table_insert(field_analysis_hash, key, field_analysis_struct);
    if (success == FALSE) {
        g_print("Critical! The key %s is a duplicate. Ensure all your column headings are unique.\n", key);
    }

}
