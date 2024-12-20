#include <gtk/gtk.h>
#include <stdio.h>
#include <glib-2.0/glib.h>
#include <headers.h>
/**
 * @file open_close_file.c
 * @brief Functions for processing the CSV file.
 */

/**
 * Passes the current line number in the CSV file to the status bar in the application window. This gives the user an indication that the application is actively reading the file.
 * @param data Pointer to the data-passer structure.
 * @return `FALSE` as required for threads accessing the main loop.
 */
gboolean line_number_in_status_bar(gpointer data) {
	Data_passer *data_passer = (Data_passer *)data;

	guint status_bar_context_info = gtk_statusbar_get_context_id(GTK_STATUSBAR(data_passer->status_bar), STATUS_BAR_CONTEXT_INFO);

	gtk_statusbar_remove(GTK_STATUSBAR(data_passer->status_bar), status_bar_context_info, data_passer->status_bar_context_info_message_id);

	gchar progress_message[100];
	g_snprintf(progress_message, 100, "Reading line %d...", data_passer->current_line_number);
	data_passer->status_bar_context_info_message_id = gtk_statusbar_push(GTK_STATUSBAR(data_passer->status_bar), status_bar_context_info, progress_message);
	return FALSE;
}

/**
 * Worker thread that analyzes the CSV file. The function does the following:
 * -# Compile regular expressions.
 * -# Create headers, either from the first line of the file or artificially.
 * -# Initialize a `Field_analysis` struct for each column.
 * -# For each line, for each column in the line, determine the minimal MySQL data type. If the data type is larger than the current data type for that column, set it as the new minimum.
 *
 *  For an overview of the algorithm implemented in this function, see \ref algorithm.
 * @param data Pointer to the data-passer structure.
 * @return `FALSE` as required for threads accessing the main loop.
 */
gboolean process_thread(gpointer data) {
	Data_passer *data_passer = (Data_passer *)data;

	gchar *csv_line;
	size_t max_number_characters = 20000;
	size_t len = 0;

	csv_line = (gchar *)g_malloc(max_number_characters * sizeof(char));

	if (csv_line == NULL) {
		g_print("We have a malloc problem\n");
		exit(-1);
	}

	gboolean on_first_line = TRUE;

	char *token;

	/* Retrieve the type of SQL */
	const char *selected_sql_type = gtk_combo_box_get_active_id((GtkComboBox *) data_passer->combo_sql_type);
	switch (*selected_sql_type) {
		case '0':
			data_passer->sql_type = SQLITE;
			break;
		case '1':
			data_passer->sql_type = MYSQL;
			break;
		default:
			data_passer->sql_type = NONE;
			g_print("No SQL type detected, results may be unreliable.\n");
	}

	/* Retrieve the field delimiter */
	char *delimiter = (g_strcmp0(gtk_combo_box_get_active_id((GtkComboBox *)(data_passer->combo_field_delimeter)), "0") == 0) ? "\t" : ",";

	data_passer->current_line_number = 0;

	gboolean has_header_line = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data_passer->checkbox_has_headers));

	enum field_quoting_options field_quoting;

	/* If the delimiter is a comma, then replace commas with tabs. */
	const gchar *active_quoted_fields = gtk_combo_box_get_active_id((GtkComboBox *)data_passer->combo_fields_enclosed);

	switch (*active_quoted_fields) {
		case '0':
			field_quoting = NEVER;
			break;
		case '1':
			field_quoting = ALWAYS;
			break;
		case '2':
			field_quoting = OPTIONAL;
			break;
	}

	while (getline(&csv_line, &len, data_passer->fp) != -1) {
		data_passer->current_line_number++;

		/* Send line number to main loop. */
		gdk_threads_add_idle((GSourceFunc)line_number_in_status_bar, data);

		/* If the delimiter is a comma, then replace commas with tabs. */
		if ((g_strcmp0(delimiter, ",") == 0)) {
			switch (field_quoting) {
				case NEVER:
					change_commas_to_tabs(&csv_line);
					break;
				case OPTIONAL:
					change_commas_to_tabs_with_optional_quotes(&csv_line);
					break;
				case ALWAYS:
					change_commas_to_tabs_with_quotes(&csv_line);
			}
		}

		if (on_first_line) {
			on_first_line = FALSE;
			if (has_header_line) {
				data_passer->headings = make_headings(csv_line, field_quoting);
			} else {
				data_passer->headings = make_forced_headings(csv_line);
			}

			switch (data_passer->sql_type) {
				case SQLITE:
					g_slist_foreach(data_passer->headings, initialize_sqlite_field_analysis, data_passer);
					break;
				case MYSQL:
					g_slist_foreach(data_passer->headings, initialize_mysql_field_analysis, data_passer);
					break;
				default:
					g_print("No initialization found for requested SQL type. Exiting.\n");
					exit(-2);
			}
			if (has_header_line) {
				continue;
			}
		}

		int column_number = 0;

		gchar *crawler = csv_line;
		gchar key[LONGEST_STRING];
		while ((token = strsep(&crawler, "\t")) != NULL) {
			/* Strip whitespace from the current token. */
			gchar *csv_value = g_strstrip(token);

			if (field_quoting != NEVER) {
				strip_quotes(&csv_value);
			}

			/* Memory freed after this while loop. */
			g_strlcpy (key, (gchar *)g_slist_nth_data(data_passer->headings, column_number), LONGEST_STRING);
		
			void *field_analysis = NULL;
			switch (data_passer->sql_type) {
				case MYSQL:
					/* Get the current field's current field analysis, which includes its MySQL data type. */
					field_analysis = (Field_analysis_mysql *)g_hash_table_lookup(data_passer->field_analysis_hash, key);
					break;
				case SQLITE:
					/* Get the current field's current field analysis, which includes its MySQL data type. */
					field_analysis = (Field_analysis_mysql *)g_hash_table_lookup(data_passer->field_analysis_hash, key);
					break;
				default:
					g_print("No SQL type was selected, results may be unreliable.\n");
				}
				if (field_analysis == NULL) {
					g_print("There was a critical failure in looking up the key.\n");
				}
		
				switch (data_passer->sql_type) {
					case SQLITE:
						do_sqlite_tests(csv_value, field_analysis, data_passer);
						break;
					case MYSQL:
						do_mysql_tests(csv_value, field_analysis, data_passer);
						break;
					default:
						g_print("You need to specify a type of SQL database. Exiting.\n");
						exit(1); 
				}
				column_number++;
			}
	}
	g_free(csv_line);

	g_main_loop_quit(data_passer->gloop);
}

/**
 * Callback for processing the CSV file. Runs after clicking Go in the main window. This function does the following:
 * -# Open the CSV file.
 * -# Launch the worker thread that processes the file.
 * -# Close the file.
 * -# Display the results.
 *
 * @param button Clicked button.
 * @param data Pointer to the data-passer structure.
 */
gboolean process_file(GtkButton *button, gpointer data) {
	Data_passer *data_passer = (Data_passer *)data;
	data_passer->fp = fopen(data_passer->filename, "r");

	if (data_passer->fp == NULL) {
		guint status_bar_context_info = gtk_statusbar_get_context_id(GTK_STATUSBAR(data_passer->status_bar), STATUS_BAR_CONTEXT_INFO);

		data_passer->status_bar_context_info_message_id = gtk_statusbar_push(GTK_STATUSBAR(data_passer->status_bar), status_bar_context_info, "Could not open the file.");
		return FALSE;
	}

	/* The setup for the threading was taken from
	https://github.com/libnice/libnice/blob/master/examples/threaded-example.c
	*/

	data_passer->gloop = g_main_loop_new(NULL, FALSE);

	GThread *process_g_thread = g_thread_new("process_thread", (GThreadFunc)process_thread, data);
	g_main_loop_run(data_passer->gloop);

	g_thread_join(process_g_thread);
	g_main_loop_unref(data_passer->gloop);
	fclose(data_passer->fp);

	display_results(data_passer);

	return TRUE;
}