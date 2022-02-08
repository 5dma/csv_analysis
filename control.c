#include <gtk/gtk.h>

#include "headers.h"
/**
 * @file app_activate.c
 * @brief Builds the view.
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

void filename_changed(GtkEntry *text_filename, gpointer data) {
    GHashTable *pointer_passer = (GHashTable *)data;
    GtkWidget *button_go = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_BUTTON_GO);
    if (gtk_entry_get_text_length(text_filename) == 0) {
        gtk_widget_set_sensitive(button_go, FALSE);
    } else {
        gtk_widget_set_sensitive(button_go, TRUE);
    }
}