#include <glib-2.0/glib.h>
#include <gtk/gtk.h>
#include "headers.h"


void closeup(GtkWidget *button_close, gpointer data) {
    GHashTable *pointer_passer = (GHashTable *)data;
    g_print("Got OMG\n");
    GtkWidget *window = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_WINDOW);
    g_signal_emit_by_name(G_OBJECT(window), "destroy", NULL);
}

void cleanup (GtkWidget* window, gpointer data) {
    g_print("Got here\n");
}
