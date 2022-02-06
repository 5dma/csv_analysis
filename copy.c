#include <glib-2.0/glib.h>
#include <gtk/gtk.h>
#include "headers.h"

void copy_statement(GtkWidget *button_copy, gpointer data) {
    GHashTable *pointer_passer = (GHashTable *)data;

    GtkWidget *label_mysql_command = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_LABEL_MYSQL_COMMAND);

    const gchar *mysql_statement = gtk_label_get_text(GTK_LABEL(label_mysql_command));

    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(clipboard, mysql_statement, -1);
}
