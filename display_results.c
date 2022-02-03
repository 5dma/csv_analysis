#include <gtk/gtk.h>

#include "headers.h"

void display_single_result(gpointer key, gpointer value, gpointer data) {
    GHashTable *pointer_passer = (GHashTable *)data;

    gchar *key_char = (gchar *)key;
    Field_analysis *field_analysis = (Field_analysis *)value;
    enum data_types field_type = field_analysis->field_type;
    char **datatype_strings = (char **)g_hash_table_lookup(pointer_passer, &KEY_DATA_TYPE_STRINGS);

    gchar datatype_string[100];
    if (field_type == CHAR) {
        g_snprintf(datatype_string, 50, "%s(%d)", *(datatype_strings + field_type), field_analysis->char_width);
    } else {
        g_snprintf(datatype_string, 50, "%s", *(datatype_strings + field_type));
    }

    GtkListStore *list_store_results = (GtkListStore *)g_hash_table_lookup(pointer_passer, &KEY_LIST_STORE);
    GtkTreeIter iter;
    gtk_list_store_append(list_store_results, &iter);

    gtk_list_store_set(list_store_results, &iter,
                       COLUMN_NAME, key_char,
                       DATA_TYPE, datatype_string,
                       DETERMINING_LINE, field_analysis->last_line_change,

                       -1);
}

void display_results(GHashTable *pointer_passer) {
    GHashTable *field_analysis_hash = (GHashTable *)g_hash_table_lookup(pointer_passer, &KEY_FIELD_ANALYSIS_HASH);

    static char *datatype_strings[] = {
        "TINYINT_UNSIGNED",
        "SMALLINT_UNSIGNED",
        "MEDIUMINT_UNSIGNED",
        "INT_UNSIGNED",
        "BIGINT_UNSIGNED",
        "TINYINT_SIGNED",
        "SMALLINT_SIGNED",
        "MEDIUMINT_SIGNED",
        "INT_SIGNED",
        "BIGINT_SIGNED",
        "DECIMAL",
        "FLOAT",
        "TIMESTAMP",
        "CHAR"};

    g_hash_table_insert(pointer_passer, &KEY_DATA_TYPE_STRINGS, datatype_strings);

    g_hash_table_foreach(field_analysis_hash, display_single_result, pointer_passer);
}