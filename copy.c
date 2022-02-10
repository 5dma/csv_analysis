#include <glib-2.0/glib.h>
#include <gtk/gtk.h>
#include "headers.h"
/**
 * @file copy.c
 * @brief Defines a function that copies the MySQL command into the clipboard.
*/

/**
 * Callback that runs after clicking the Copy button. This function copies the MySQL command into the clipboard.
 * @param button_copy Clicked button.
 * @param data Pointer to the pointer-passer hash.
*/
void copy_statement(GtkWidget *button_copy, gpointer data) {
    GHashTable *pointer_passer = (GHashTable *)data;

    GtkWidget *label_mysql_command = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_LABEL_MYSQL_COMMAND);

    /* The returned value is owned by the instance, which is clipboard. So no need to free. Is this really the case? */
    const gchar *mysql_statement = gtk_label_get_text(GTK_LABEL(label_mysql_command));

    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(clipboard, mysql_statement, -1);
}
