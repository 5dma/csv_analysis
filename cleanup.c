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
    g_print("Closing\n");
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
    g_print("Cleaning\n");
    GHashTable *pointer_passer = (GHashTable *)data;
    g_print("Cleaning A\n");
    gchar *filename = (gchar *)g_hash_table_lookup(pointer_passer, &KEY_CSV_FILE);
    g_print("Cleaning B\n");
    g_free(filename);
    g_print("Cleaning C\n");

    gpointer null_hash_tester = g_hash_table_lookup(pointer_passer, &KEY_NUMBER_OF_COLUMNS);
    if (null_hash_tester != NULL) {
        guint *number_of_columns = (guint *)null_hash_tester;
        g_print("Cleaning D number of columns %u\n", *number_of_columns);

        gchar **column_strings = (gchar **)g_hash_table_lookup(pointer_passer, &KEY_COLUMN_STRINGS);
        g_print("Cleaning E\n");
        for (gint i = 0; i < *number_of_columns; i++) {
            g_print("Freeing %d\n", i);
            g_free(column_strings + i);
        }
    }

    g_print("Cleaning F\n");
    null_hash_tester = g_hash_table_lookup(pointer_passer, &KEY_FIELD_CLAUSE);
    if (null_hash_tester != NULL) {
        gchar *field_clause = (gchar *)null_hash_tester;
        g_free(field_clause);
    }

    g_print("Cleaning G\n");
    null_hash_tester = g_hash_table_lookup(pointer_passer, &KEY_HEADINGS);
    if (null_hash_tester != NULL) {
        GSList *headings = (GSList *)null_hash_tester;
        g_slist_free_full(headings, g_free);
    }

    g_print("Cleaning H\n");
    null_hash_tester = g_hash_table_lookup(pointer_passer, &KEY_FIELD_ANALYSIS_HASH);
    if (null_hash_tester != NULL) {
        GHashTable *field_analysis_hash = (GHashTable *)null_hash_tester;
        g_hash_table_foreach_remove(field_analysis_hash, (GHRFunc)free_field_analysis_hash, NULL);
    }
    g_hash_table_destroy(pointer_passer);
    g_print("Cleaning Z\n");
}
