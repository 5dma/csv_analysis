#include <glib-2.0/glib.h>
#include <gtk/gtk.h>
#include <headers.h>
/**
 * @file copy.c
 * @brief Defines a function that copies the MySQL command into the clipboard.
*/

/**
 * Callback that runs after clicking the Copy button. This function copies the MySQL command into the clipboard.
 * @param button_copy Clicked button.
 * @param data Pointer to the data-passer structure.
*/
void copy_statement(GtkWidget *button_copy, gpointer data) {
    Data_passer *data_passer = (Data_passer *)data;

    /* The returned value is owned by the instance, which is clipboard. So no need to free. Is this really the case? */
    const gchar *mysql_statement = gtk_label_get_text(GTK_LABEL(data_passer -> label_mysql_command));

    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(clipboard, mysql_statement, -1);
}
