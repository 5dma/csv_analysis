#include <gtk/gtk.h>

#include "headers.h"
/**
 * @file app_activate.c
 * @brief Builds the view.
*/

/**
 * Function that creates the application's view.
 * @param app Pointer to the GTK application.
*/
GtkWidget *make_window(GHashTable *pointer_passer) {
    GApplication *app = (GApplication *)g_hash_table_lookup(pointer_passer, &KEY_APP);

    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    g_hash_table_insert(pointer_passer, &KEY_WINDOW, window);

    /* Set maximal width, prevent users from resizing. */
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, -1);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    /* Controls in first row of grid: label, entry field for file name, button for choosing a file. */
    GtkWidget *label_csv_file = gtk_label_new("CSV file:");
    GtkWidget *text_filename = gtk_entry_new();
    g_signal_connect(G_OBJECT(text_filename), "changed", G_CALLBACK(filename_changed), pointer_passer);
    g_hash_table_insert(pointer_passer, &KEY_TEXT_FILENAME, text_filename);

    GtkWidget *button_choose = gtk_button_new_with_label("Choose...");
    gtk_widget_set_valign(button_choose, GTK_ALIGN_START);
    gtk_widget_set_halign(button_choose, GTK_ALIGN_CENTER);
    g_signal_connect(G_OBJECT(button_choose), "clicked", G_CALLBACK(button_choose_clicked), pointer_passer);

    /* Pack the first row into an hbox. */
    GtkWidget *hbox_file_choose = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(hbox_file_choose), label_csv_file, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_file_choose), text_filename, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_file_choose), button_choose, FALSE, FALSE, 10);

    /* Checkbox for indicating if the input CSV file has column headers. */
    GtkWidget *checkbox_has_headers = gtk_check_button_new_with_label("File contains column headers");
    g_hash_table_insert(pointer_passer, &KEY_CHECKBOX_HEADER, checkbox_has_headers);

    /* Button for starting the processing. */
    GtkWidget *button_go = gtk_button_new_with_label("Go");
    gtk_widget_set_valign(button_go, GTK_ALIGN_START);
    gtk_widget_set_halign(button_go, GTK_ALIGN_START);
    gtk_widget_set_sensitive(button_go, FALSE);
    g_hash_table_insert(pointer_passer, &KEY_BUTTON_GO, button_go);
    g_signal_connect(G_OBJECT(button_go), "clicked", G_CALLBACK(process_file), pointer_passer);

    /* List store that contains the results of the analysis. The columns in the store are column heading, MySQL data type, and the line in the CSV file that determined the MySQL data type. */
    GtkListStore *list_store_results = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);
    g_hash_table_insert(pointer_passer, &KEY_LIST_STORE, list_store_results);

    GtkTreeIter iter;
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
    columnDeterminingLine = gtk_tree_view_column_new_with_attributes("Determining line", rendererDeterminingLine,
                                                                     "text", DETERMINING_LINE,
                                                                     NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), columnName);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), columnType);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), columnDeterminingLine);

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
    g_hash_table_insert(pointer_passer, &KEY_BUFFER_TABLE, entry_table_name);
    gtk_widget_set_sensitive(entry_table_name, FALSE);
    
    /* Two signals attached to this object. The first is key-press-event, which checks that the user types a valid character for MySQL tables. If so, the changed signal updates the displayed command with the new table name. */
    g_signal_connect(G_OBJECT(entry_table_name), "key-press-event", G_CALLBACK(table_name_formatter), pointer_passer);
    g_signal_connect(G_OBJECT(entry_table_name), "changed", G_CALLBACK(concat_command), pointer_passer);

    /* Place the label and entry field into an hbox. */
    GtkWidget *hbox_table_name = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(hbox_table_name), label_table_name, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(hbox_table_name), entry_table_name, FALSE, FALSE, 10);

    /* Label for displaying the MySQL CREATE TABLE command. */
    GtkWidget *label_mysql_command = gtk_label_new("CREATE TABLE ");
    gtk_widget_set_halign(label_mysql_command, GTK_ALIGN_START);
    gtk_label_set_line_wrap(GTK_LABEL(label_mysql_command), TRUE);
    gtk_label_set_selectable(GTK_LABEL(label_mysql_command), TRUE);
    gtk_label_set_yalign (GTK_LABEL(label_mysql_command), 0);

    g_hash_table_insert(pointer_passer, &KEY_LABEL_MYSQL_COMMAND, label_mysql_command);

    /* Scrolled window that contains the generated MySQL command. */
    GtkWidget *scrolled_window_command = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_command), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window_command), label_mysql_command);
    gtk_widget_set_size_request(scrolled_window_command, WINDOW_WIDTH, 150);
    
    /* Button that terminates the application. */
    GtkWidget *button_close = gtk_button_new_with_label("Close");
    gtk_widget_set_valign(button_close, GTK_ALIGN_START);
    gtk_widget_set_halign(button_close, GTK_ALIGN_END);
    g_signal_connect(G_OBJECT(button_close), "clicked", G_CALLBACK(closeup), pointer_passer);

    /* Button that copies the generated MySQL command into the clipboard. */
    GtkWidget *button_copy = gtk_button_new_with_label("Copy");
    gtk_widget_set_valign(button_copy, GTK_ALIGN_START);
    gtk_widget_set_halign(button_copy, GTK_ALIGN_CENTER);
    g_signal_connect(G_OBJECT(button_copy), "clicked", G_CALLBACK(copy_statement), pointer_passer);
    gtk_widget_set_sensitive(button_copy, FALSE);
    g_hash_table_insert(pointer_passer, &KEY_BUTTON_COPY, button_copy);

    /* Status bar showing various messages. */
    GtkWidget *status_bar = gtk_statusbar_new();
    g_hash_table_insert(pointer_passer, &KEY_STATUS_BAR, status_bar);
    guint status_bar_context_info = gtk_statusbar_get_context_id(GTK_STATUSBAR(status_bar), STATUS_BAR_CONTEXT_INFO);
    guint status_bar_context_info_message_id = gtk_statusbar_push(GTK_STATUSBAR(status_bar), status_bar_context_info, "Ready");
    g_hash_table_insert(pointer_passer, &STATUS_BAR_CONTEXT_INFO_CURRENT_MESSAGE_ID, &status_bar_context_info_message_id);

    /* Grid for displaying all of the controls. */
    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_margin_top(grid, 20);
    gtk_grid_attach(GTK_GRID(grid), hbox_file_choose, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), checkbox_has_headers, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_go, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), hbox_table_name, 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window_command, 0, 4, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), button_close, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_copy, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), status_bar, 0, 6, 2, 1);

    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);

    gtk_container_add(GTK_CONTAINER(window), grid);

    /* Upon destroying the application, free memory in data structures in pointer_passer. */
    g_signal_connect(window, "destroy", G_CALLBACK(cleanup), pointer_passer);
    return window;
}
