#include <glib-2.0/glib.h>
#include <gtk/gtk.h>

#include <headers.h>
/**
 * @file cleanup.c
 * @brief Defines callback functions for freeing memory and closing down the application.
 */

/**
 * Callback that runs on each column heading in `data_passer->headings`. The function frees the memory associated with the passed value.
 * @param heading Passed heading.
 * @param data Pointer to user data (`NULL` in this case).
 */
void delete_heading(gpointer heading, gpointer data) {
	gchar *key = (gchar *)heading;
	g_free(key);
}

/**
 * Callback that runs on each element of the field analysis hash. The function frees the memory associated with the passed value, which is a `Field_analysis`.
 * @param key Passed key.
 * @param value Passed value.
 * @param data Pointer to user data (`NULL` in this case).
 * @return `TRUE` to remove the key and value from the hash table, as descrbed in [HRFunc](https://docs.gtk.org/glib/callback.HRFunc.html).
 */
gboolean free_field_analysis_hash(gpointer key, gpointer value, gpointer data) {
	Field_analysis *field_analysis = (Field_analysis *)value;
	g_free(field_analysis);
	return TRUE;
}

/**
 * Callback that runs after clicking <b>Close</b>. This function emits the \c destroy signal on the application window. The callback for that emitted signal is cleanup().
 * @param button_close Clicked button.
 * @param data Pointer to the data-passer structure.
 */
void closeup(GtkWidget *button_close, gpointer data) {
	Data_passer *data_passer = (Data_passer *)data;
	gtk_widget_destroy(data_passer->window);
}

/**
 * Callback that runs after the destory signal is emitted on the application window. This function frees all the memory allocated to objects in the data passer.
 * @param window Main application window.
 * @param data Pointer to the data-passer structure.
 */
void cleanup(GtkWidget *window, gpointer data) {
	Data_passer *data_passer = (Data_passer *)data;
	if (data_passer->filename != NULL) {
		g_free(data_passer->filename);
	}

	for (int i = 0; i < 15; i++) {
		g_free (data_passer -> datatype_strings[i]);
	}

	regfree(&(data_passer->decimal_regex));
	regfree(&(data_passer->timestamp_regex));

	if (data_passer->number_of_columns != 0) {
		for (gint i = 0; i < data_passer->number_of_columns; i++) {
			g_free(data_passer -> column_strings[i]);
		}
	}

	if (data_passer->field_clause != NULL) {
		g_free(data_passer->field_clause);
	}

	if (data_passer->headings != NULL) {
		g_slist_free_full(data_passer->headings, (GDestroyNotify)delete_heading);
	}

	if (data_passer->field_analysis_hash != NULL) {
		g_hash_table_foreach_remove(data_passer->field_analysis_hash, (GHRFunc)free_field_analysis_hash, NULL);
	}
	g_hash_table_destroy(data_passer->field_analysis_hash);
}
