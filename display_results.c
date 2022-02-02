#include <gtk/gtk.h>
#include "headers.h"


void display_single_result(gpointer key, gpointer value, gpointer user_data) {
    gchar *key_char = (gchar *)key;
    Field_analysis *field_analysis = (Field_analysis *) value;
    enum data_types field_type = field_analysis->field_type;
   char **datatype_strings = (char **)user_data;
    g_print("Results for %s:\n", key_char);
    if (field_type == CHAR) {
        g_print("  %s(%d)\n",*(datatype_strings + field_type),field_analysis->char_width);
    } else {
        g_print("  %s\n",*(datatype_strings + field_type));
    }
    g_print("  Latest change was on line %d\n", field_analysis->last_line_change);

} 

void display_results(GHashTable *field_analysis_hash) {

    static char* datatype_strings[] = {
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

    g_print("Ready to do the output\n");
    g_hash_table_foreach(field_analysis_hash, display_single_result, &datatype_strings);
}