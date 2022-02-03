#include <gtk/gtk.h>

#include "headers.h"


guint get_number_of_fields(GHashTable *field_analysis_hash) {
    GList *keys = g_hash_table_get_keys(field_analysis_hash);
    return g_list_length(keys);
}

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

    guint *current_column_number = (guint *)g_hash_table_lookup(pointer_passer, &KEY_CURRENT_COLUMN_NUMBER);
    gchar **column_strings = (gchar **)g_hash_table_lookup(pointer_passer, &KEY_COLUMN_STRINGS);

    *(column_strings + field_type) = g_strdup(datatype_string);

    (*current_column_number)++;
    
}

void display_results(GHashTable *pointer_passer) {
    GHashTable *field_analysis_hash = (GHashTable *)g_hash_table_lookup(pointer_passer, &KEY_FIELD_ANALYSIS_HASH);

    gchar *datatype_strings[14] = {
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

    guint number_of_columns = get_number_of_fields(field_analysis_hash);
    gchar *column_strings[number_of_columns];
    g_hash_table_insert(pointer_passer, &KEY_COLUMN_STRINGS, column_strings);

    guint current_column_number = 0;
    g_hash_table_insert(pointer_passer, &KEY_CURRENT_COLUMN_NUMBER, &current_column_number);

    g_hash_table_foreach(field_analysis_hash, display_single_result, pointer_passer);

    GtkEntryBuffer *buffer_table = (GtkEntryBuffer *)g_hash_table_lookup(pointer_passer, &KEY_BUFFER_TABLE);

    gchar *field_clause = g_strjoinv(", ",column_strings);

    gchar *complete_command = g_strconcat ("CREATE TABLE ", gtk_entry_buffer_get_text (buffer_table), " ", field_clause,");", NULL);

   GtkWidget *label_mysql_command = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_LABEL_MYSQL_COMMAND);

   gtk_label_set_text(GTK_LABEL(label_mysql_command), complete_command);

}