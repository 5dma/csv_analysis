#include <gtk/gtk.h>
#include <headers.h>
/**
 * @file control.c
 * @brief Defines callback functions that run after various signals emitted in the application window.
*/

/**
 * Callback that runs after clicking the \b Choose button. This function performs the following:
 * - Opens a file chooser dialog.
 * - Populates the entry field in the window with the selected file's path and name.
 *
 * @param button Clicked button.
 * @param data Pointer to the data-passer structure.
*/
void button_choose_clicked(GtkButton *button, gpointer data) {
	Data_passer *data_passer = (Data_passer *)data;
	GtkWindow *window = (GtkWindow *)data_passer -> window;
	GtkWidget *dialog;


	dialog = gtk_file_chooser_dialog_new("Choose file", window,
										 GTK_FILE_CHOOSER_ACTION_OPEN,
										 "Cancel", GTK_RESPONSE_CANCEL,
										 "Open", GTK_RESPONSE_OK,
										 NULL);

	gint result = gtk_dialog_run(GTK_DIALOG(dialog));

	if (result == GTK_RESPONSE_OK) {
		gchar *filename = NULL;
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		data_passer -> filename = strdup(filename);

		gtk_entry_set_text(GTK_ENTRY(data_passer -> text_filename), filename);
		g_free(filename);
	}
	gtk_widget_destroy(dialog);
}


/**
 * Callback that runs when the entry for the CSV filename is changed. This function performs the following:
 * -# Determines the length of the string currently in the entry field.
 * -# If the length is zero, disables the \b Go button; otherwise, enable the \b Go button.
 * -# Sets the message in the status bar to \c Ready.
 *
 * @param text_filename GtkEntry holding the current filename.
 * @param data Pointer to the data-passer structure.
*/
void filename_changed(GtkEntry *text_filename, gpointer data) {
   Data_passer *data_passer = (Data_passer *)data;
	if (gtk_entry_get_text_length(text_filename) == 0) {
		gtk_widget_set_sensitive(data_passer -> button_go , FALSE);
	} else {
		gtk_widget_set_sensitive(data_passer -> button_go, TRUE);
	}

	guint status_bar_context_info = gtk_statusbar_get_context_id(GTK_STATUSBAR(data_passer -> status_bar), STATUS_BAR_CONTEXT_INFO);

	gtk_statusbar_remove(GTK_STATUSBAR(data_passer -> status_bar), status_bar_context_info, data_passer -> status_bar_context_info_message_id);

	data_passer -> status_bar_context_info_message_id = gtk_statusbar_push(GTK_STATUSBAR(data_passer -> status_bar), status_bar_context_info, "Ready");
}