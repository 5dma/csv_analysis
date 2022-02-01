#include <gtk/gtk.h>

#include "headers.h"

void initialize_field_analysis(gpointer heading, gpointer data) {
    gchar *key = (gchar *)heading;
    GHashTable **field_analysis_hash_ptr = (GHashTable **)data;
    GHashTable *field_analysis_hash = *field_analysis_hash_ptr;

    Field_analysis *field_analysis_struct = (Field_analysis *)malloc(sizeof(Field_analysis));

    field_analysis_struct->field_type = TINYINT_UNSIGNED;
    field_analysis_struct->sql_signed = FALSE;
    field_analysis_struct->char_width = 0;
    field_analysis_struct->precision = 0;
    field_analysis_struct->scale = 0;

    gboolean success = g_hash_table_insert(field_analysis_hash, key, field_analysis_struct);
    if (success == FALSE) {
        g_print("Warning! A key was replaced in the hash table!\n");
    }
}
