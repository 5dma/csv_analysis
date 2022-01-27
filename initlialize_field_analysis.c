#include <gtk/gtk.h>

#include "headers.h"

void initialize_field_analysis(gpointer heading, gpointer data) {
    char *key = (char *)heading;
    GHashTable *field_analysis_hash = (GHashTable *)data;

    Field_analysis *field_analysis_struct = (Field_analysis *)malloc(sizeof(Field_analysis));

    field_analysis_struct->field_type = TINYINT_UNSIGNED;
    field_analysis_struct->sql_signed = FALSE;
    field_analysis_struct->char_width = 0;
    field_analysis_struct->precision = 0;
    field_analysis_struct->scale = 0;

    g_hash_table_insert(field_analysis_hash, key, field_analysis_struct);
}
