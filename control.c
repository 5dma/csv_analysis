#include <gtk/gtk.h>
#include "headers.h"
/**
 * @file control.c
 * @brief Defines callback functions that run after various signals emitted in the application window.
*/

/**
 * Callback that runs after clicking the Choose button. This function performs the following:
 * - Opens a file chooser dialog.
 * - Populates the entry field in the window with the selected file's path and name.
 * @param button Clicked button.
 * @param data Pointer to the pointer-passer hash.
*/
void button_choose_clicked(GtkButton *button, gpointer data) {
    GHashTable *pointer_passer = (GHashTable *)data;
    GtkWindow *window = (GtkWindow *)g_hash_table_lookup(pointer_passer, &KEY_WINDOW);
    GtkWidget *dialog;

    gchar *filename = NULL;
    dialog = gtk_file_chooser_dialog_new("Choose file", window,
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "Cancel", GTK_RESPONSE_CANCEL,
                                         "Open", GTK_RESPONSE_OK,
                                         NULL);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_OK) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        g_hash_table_insert(pointer_passer, &KEY_CSV_FILE, strdup(filename));
       
        GtkWidget *text_filename = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_TEXT_FILENAME);

        gtk_entry_set_text(GTK_ENTRY(text_filename), filename);
    }
    gtk_widget_destroy(dialog);
}


/**
 * Callback that runs when the entry for the CSV filename is changed. This function performs the following:
 * -# The the length of the string currently in the entry field.
 * -# If the length is zero, disable the Go button; otherwise, enable the Go button.
 * -# Set the message in the status bar to Ready.
 * @param text_filename GtkEntry holding the current filename.
 * @param data Pointer to the pointer-passer hash.
*/
void filename_changed(GtkEntry *text_filename, gpointer data) {
    GHashTable *pointer_passer = (GHashTable *)data;
    GtkWidget *button_go = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_BUTTON_GO);
    if (gtk_entry_get_text_length(text_filename) == 0) {
        gtk_widget_set_sensitive(button_go, FALSE);
    } else {
        gtk_widget_set_sensitive(button_go, TRUE);
    }

    GtkWidget *status_bar = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_STATUS_BAR);

    guint status_bar_context_info_message_id = *(guint *)g_hash_table_lookup(pointer_passer, &STATUS_BAR_CONTEXT_INFO_CURRENT_MESSAGE_ID);

    guint status_bar_context_info = gtk_statusbar_get_context_id(GTK_STATUSBAR(status_bar), STATUS_BAR_CONTEXT_INFO);

    gtk_statusbar_remove(GTK_STATUSBAR(status_bar), status_bar_context_info, status_bar_context_info_message_id);

    status_bar_context_info_message_id = gtk_statusbar_push(GTK_STATUSBAR(status_bar), status_bar_context_info, "Ready");
}