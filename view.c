#include <gtk/gtk.h>
#include <headers.h>
/**
 * @file view.c
 * @brief Builds the view.
 */

/**
 * Function that creates the application's view.
 * @param data_passer Pointer to the data-passer structure.
 * @return Pointer to the application window.
*/
GtkWidget *make_window(Data_passer *data_passer) {
	GApplication *app = data_passer->app;

	GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
	data_passer->window = window;

	/* Set application icon. */
	GdkPixbuf *app_icon = gdk_pixbuf_new_from_file("/home/abba/.csv_analysis/csv_analysis_icon.png", NULL);
	gtk_window_set_icon(GTK_WINDOW(window), app_icon);

	/* Set maximal width, prevent users from resizing. */
	gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, -1);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	/* Controls in first row of grid: label, entry field for file name, button for choosing a file. */
	GtkWidget *label_csv_file = gtk_label_new("CSV file:");
	GtkWidget *text_filename = gtk_entry_new();
	g_signal_connect(G_OBJECT(text_filename), "changed", G_CALLBACK(filename_changed), data_passer);
	data_passer->text_filename = text_filename;

	GtkWidget *button_choose = gtk_button_new_with_label("Choose...");
	gtk_widget_set_valign(button_choose, GTK_ALIGN_START);
	gtk_widget_set_halign(button_choose, GTK_ALIGN_CENTER);
	g_signal_connect(G_OBJECT(button_choose), "clicked", G_CALLBACK(button_choose_clicked), data_passer);

	/* Pack the first row into an hbox. */
	GtkWidget *hbox_file_choose = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_start(GTK_BOX(hbox_file_choose), label_csv_file, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox_file_choose), text_filename, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox_file_choose), button_choose, FALSE, FALSE, 10);



	/* Control for type of SQL */
	GtkWidget *label_sql_type = gtk_label_new("SQL type:");
	GtkWidget *combo_sql_type = gtk_combo_box_text_new();
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_sql_type), "0", "SQLite");
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_sql_type), "1", "MySQL");
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(combo_sql_type), "0");
	data_passer->combo_sql_type = combo_sql_type;

	/* Control for field delimiter */
	GtkWidget *label_field_delimiter = gtk_label_new("Field delimiter:");
	GtkWidget *combo_field_delimeter = gtk_combo_box_text_new();
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_field_delimeter), "0", "Tabs");
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_field_delimeter), "1", "Commas");
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(combo_field_delimeter), "0");
	data_passer->combo_field_delimeter = combo_field_delimeter;


	/* Hbox for type of SQL and label. */
	GtkWidget *hbox_sql_type = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_start(GTK_BOX(hbox_sql_type), label_sql_type, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox_sql_type), combo_sql_type, FALSE, FALSE, 5);

	/* Hbox for field delimiter and label. */
	GtkWidget *hbox_field_delimiter = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_start(GTK_BOX(hbox_field_delimiter), label_field_delimiter, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox_field_delimiter), combo_field_delimeter, FALSE, FALSE, 5);

	/* Control for fields enclosed by */
	GtkWidget *label_fields_enclosed = gtk_label_new("Fields enclosed by:");
	GtkWidget *combo_fields_enclosed = gtk_combo_box_text_new();
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_fields_enclosed), "0", "(none)");
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_fields_enclosed), "1", "Double quotes (always)");
	gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_fields_enclosed), "2", "Double quotes (optional)");
	gtk_combo_box_set_active_id(GTK_COMBO_BOX(combo_fields_enclosed), "0");
	data_passer->combo_fields_enclosed = combo_fields_enclosed;

	/* Hbox for fields enclosed by and label. */
	GtkWidget *hbox_fields_enclosed = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_start(GTK_BOX(hbox_fields_enclosed), label_fields_enclosed, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox_fields_enclosed), combo_fields_enclosed, FALSE, FALSE, 5);

	/* Checkbox for indicating if the input CSV file has column headers. */
	GtkWidget *checkbox_has_headers = gtk_check_button_new_with_label("File contains column headers");
	data_passer->checkbox_has_headers = checkbox_has_headers;

	/* Button for starting the processing. */
	GtkWidget *button_go = gtk_button_new_with_label("Go");
	gtk_widget_set_valign(button_go, GTK_ALIGN_START);
	gtk_widget_set_halign(button_go, GTK_ALIGN_START);
	gtk_widget_set_sensitive(button_go, FALSE);
	data_passer->button_go = button_go;
	g_signal_connect(G_OBJECT(button_go), "clicked", G_CALLBACK(process_file), data_passer);

	/* List store that contains the results of the analysis. The columns in the store are column heading, MySQL data type, and the line in the CSV file that determined the MySQL data type. */
	GtkListStore *list_store_results = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING); /* Reference count decremented below, after being assigned to the tree. */
	data_passer->list_store_results = list_store_results;

	GtkWidget *tree;

	/* Tree view for displaying the results. */
	tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store_results));
	g_object_unref(list_store_results);
	g_object_set(tree, "enable-grid-lines", GTK_TREE_VIEW_GRID_LINES_BOTH, NULL);

	GtkCellRenderer *rendererColumnName;
	GtkTreeViewColumn *columnName;

	rendererColumnName = gtk_cell_renderer_text_new();
	columnName = gtk_tree_view_column_new_with_attributes("Column name", rendererColumnName,
														  "text", COLUMN_NAME,
														  NULL);
	GtkCellRenderer *rendererType;
	GtkTreeViewColumn *columnType;

	rendererType = gtk_cell_renderer_text_new();
	columnType = gtk_tree_view_column_new_with_attributes("Data type", rendererType,
														  "text", DATA_TYPE,
														  NULL);

	GtkCellRenderer *rendererDeterminingLine;
	GtkTreeViewColumn *columnDeterminingLine;

	rendererDeterminingLine = gtk_cell_renderer_text_new();
	gtk_cell_renderer_set_alignment(GTK_CELL_RENDERER(rendererDeterminingLine), 1.0, 0.0);
	columnDeterminingLine = gtk_tree_view_column_new_with_attributes("Determining line", rendererDeterminingLine,
																	 "text", DETERMINING_LINE,
																	 NULL);
	gtk_tree_view_column_set_alignment(GTK_TREE_VIEW_COLUMN(columnDeterminingLine), 1.0);

	GtkCellRenderer *rendererDeterminingValue;
	GtkTreeViewColumn *columnDeterminingValue;

	rendererDeterminingValue = gtk_cell_renderer_text_new();
	columnDeterminingValue = gtk_tree_view_column_new_with_attributes("Determining value", rendererDeterminingValue,
																	  "text", DETERMINING_VALUE,
																	  NULL);

	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), columnName);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), columnType);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), columnDeterminingLine);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), columnDeterminingValue);

	/* A scrolled window that contains the tree view. */
	GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scrolled_window), tree);
	gtk_widget_set_size_request(scrolled_window, WINDOW_WIDTH, 250);

	/* Label for the entry field. */
	GtkWidget *label_table_name = gtk_label_new("Table name:");

	/* Entry field where user types the new table's name. */
	GtkWidget *entry_table_name = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry_table_name), "mytable");
	data_passer->entry_table_name = entry_table_name;
	gtk_widget_set_sensitive(entry_table_name, FALSE);

	/* Two signals attached to this object. The first is key-press-event, which checks that the user types a valid character for MySQL tables. If so, the changed signal updates the displayed command with the new table name. */
	g_signal_connect(G_OBJECT(entry_table_name), "key-press-event", G_CALLBACK(table_name_formatter), data_passer);
	g_signal_connect(G_OBJECT(entry_table_name), "changed", G_CALLBACK(concat_command), data_passer);

	/* Place the label and entry field into an hbox. */
	GtkWidget *hbox_table_name = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_pack_start(GTK_BOX(hbox_table_name), label_table_name, FALSE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(hbox_table_name), entry_table_name, FALSE, FALSE, 10);

	/* Label for displaying the MySQL CREATE TABLE command. */
	GtkWidget *label_mysql_command = gtk_label_new("CREATE TABLE ");
	gtk_widget_set_halign(label_mysql_command, GTK_ALIGN_START);
	gtk_label_set_line_wrap(GTK_LABEL(label_mysql_command), TRUE);
	gtk_label_set_selectable(GTK_LABEL(label_mysql_command), TRUE);
	gtk_label_set_yalign(GTK_LABEL(label_mysql_command), 0);

	data_passer->label_mysql_command = label_mysql_command;

	/* Scrolled window that contains the generated MySQL command. */
	GtkWidget *scrolled_window_command = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_command), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scrolled_window_command), label_mysql_command);
	gtk_widget_set_size_request(scrolled_window_command, WINDOW_WIDTH, 150);

	/* Button that terminates the application. */
	GtkWidget *button_close = gtk_button_new_with_label("Close");
	gtk_widget_set_valign(button_close, GTK_ALIGN_START);
	gtk_widget_set_halign(button_close, GTK_ALIGN_END);
	g_signal_connect(G_OBJECT(button_close), "clicked", G_CALLBACK(closeup), data_passer);

	/* Button that copies the generated MySQL command into the clipboard. */
	GtkWidget *button_copy = gtk_button_new_with_label("Copy");
	gtk_widget_set_valign(button_copy, GTK_ALIGN_START);
	gtk_widget_set_halign(button_copy, GTK_ALIGN_CENTER);
	g_signal_connect(G_OBJECT(button_copy), "clicked", G_CALLBACK(copy_statement), data_passer);
	gtk_widget_set_sensitive(button_copy, FALSE);
	data_passer->button_copy = button_copy;

	/* Status bar showing various messages. */
	GtkWidget *status_bar = gtk_statusbar_new();
	data_passer->status_bar = status_bar;
	guint status_bar_context_info = gtk_statusbar_get_context_id(GTK_STATUSBAR(status_bar), STATUS_BAR_CONTEXT_INFO);
	guint status_bar_context_info_message_id = gtk_statusbar_push(GTK_STATUSBAR(status_bar), status_bar_context_info, "Ready");
	data_passer->status_bar_context_info_message_id = status_bar_context_info_message_id;

	/* Grid for displaying all of the controls. */
	GtkWidget *grid = gtk_grid_new();
	gtk_widget_set_margin_top(grid, 20);
	gtk_grid_attach(GTK_GRID(grid), hbox_file_choose, 0, 0, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), hbox_sql_type, 0, 1, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), hbox_field_delimiter, 0, 2, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), hbox_fields_enclosed, 0, 3, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), checkbox_has_headers, 0, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), button_go, 1, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 5, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), hbox_table_name, 0, 6, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), scrolled_window_command, 0, 7, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), button_close, 0, 8, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), button_copy, 1, 8, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), status_bar, 0, 9, 2, 1);

	gtk_grid_set_row_spacing(GTK_GRID(grid), 20);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 20);

	gtk_container_add(GTK_CONTAINER(window), grid);

	/* Upon destroying the application, free memory in data passer. */
	g_signal_connect(window, "destroy", G_CALLBACK(cleanup), data_passer);
	return window;
}
