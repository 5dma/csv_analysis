#include <glib-2.0/glib.h>
#include <gtk/gtk.h>

#include "headers.h"
/**
 * @file cleanup.c
 * @brief Defines callback functions for closing down the application.
*/

/**
 * Callback that runs on each element of the field analysis hash. The function frees the memory associated with the passed value, which is a `Field_analysis`. 
*/
gboolean free_field_analysis_hash(gpointer key, gpointer value, gpointer user_data) {
    Field_analysis *field_analysis = (Field_analysis *)value;
    g_free(field_analysis);
    return TRUE;
}

/**
 * Callback that runs after clicking on the Close button. This function emits the "destroy" signal on the application window. The callback for that emitted signal is cleanup().
 * @param button_close Clicked button.
 * @param data Pointer to the pointer-passer hash.
*/
void closeup(GtkWidget *button_close, gpointer data) {
    /* For some reason, this strategy emits two destroy signals. Need to resolve. */
    GHashTable *pointer_passer = (GHashTable *)data;
    GtkWidget *window = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_WINDOW);
    gtk_widget_destroy(window);
}

/**
 * Callback that runs after the destory signal is emitted on the application window. This function frees all the memory allocated to objects in the pointer passer.
 * @param window Main application window.
 * @param data Pointer to the pointer-passer hash.
*/
void cleanup(GtkWidget *window, gpointer data) {
    GHashTable *pointer_passer = (GHashTable *)data;
    gchar *filename = (gchar *)g_hash_table_lookup(pointer_passer, &KEY_CSV_FILE);
    g_free(filename);

    guint *number_of_columns = (guint *)g_hash_table_lookup(pointer_passer, &KEY_NUMBER_OF_COLUMNS);

    gchar **column_strings = (gchar **)g_hash_table_lookup(pointer_passer, &KEY_COLUMN_STRINGS);
    for (gint i = 0; i < *number_of_columns; i++) {
        g_free(column_strings + i);
    }

    gchar *field_clause = g_hash_table_lookup(pointer_passer, &KEY_FIELD_CLAUSE);
    g_free(field_clause);

    GSList *headings = (GSList *)g_hash_table_lookup(pointer_passer, &KEY_HEADINGS);
    g_slist_free_full(headings, g_free);

    GHashTable *field_analysis_hash = (GHashTable *)g_hash_table_lookup(pointer_passer, &KEY_FIELD_ANALYSIS_HASH);
    g_hash_table_foreach_remove(field_analysis_hash, (GHRFunc)free_field_analysis_hash, NULL);

    g_hash_table_destroy (pointer_passer);

}
