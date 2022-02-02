#include <gtk/gtk.h>
#include "headers.h"
/**
 * @file app_activate.c
 * @brief Builds the view.
*/

/**
 * Function that starts the GTK loop.
 * @param app Pointer to the GTK application
 * @param data Pointer to passed data, `NULL` in this case.
*/
void on_app_activate(GApplication *app, gpointer data) {

    /* A hash table for passing pointers to callbacks */
    GHashTable *pointer_passer = g_hash_table_new(g_int_hash, g_int_equal);

    g_hash_table_insert(pointer_passer, &KEY_APP, app);

    GtkWidget *window = make_window(pointer_passer);

    gtk_widget_show_all(GTK_WIDGET(window));
    
}
