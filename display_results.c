#include <gtk/gtk.h>

void display_single_result(gpointer key, gpointer value, gpointer user_data) {
    gchar *key_char = (gchar *)key;
    g_print("Results for %s\n", key_char);
}

void display_results(GHashTable *field_analysis_hash) {
    g_print("Ready to do the output\n");
    g_hash_table_foreach(field_analysis_hash, display_single_result, NULL);
}