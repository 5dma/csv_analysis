#include <gtk/gtk.h>

#include "headers.h"
/**
 * @file app_activate.c
 * @brief Builds the view.
*/

/**
 * Function that starts the GTK loop.
 * @param app Pointer to the GTK application
*/
GtkWidget *make_window(GHashTable *pointer_passer) {
    GApplication *app = (GApplication *)g_hash_table_lookup(pointer_passer, &KEY_APP);

    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));

    g_hash_table_insert(pointer_passer, &KEY_WINDOW, window);

    // gtk_window_set_default_size(GTK_WINDOW(window), 400, 800);

    GtkWidget *label_csv_file = gtk_label_new("CSV file:");
    GtkWidget *text_filename = gtk_entry_new();
    GtkWidget *button_choose = gtk_button_new_with_label("Choose...");
        gtk_widget_set_valign(button_choose, GTK_ALIGN_START);
    gtk_widget_set_halign(button_choose, GTK_ALIGN_CENTER);

    GtkWidget *hbox_file_choose = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(hbox_file_choose), label_csv_file, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_file_choose), text_filename, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox_file_choose), button_choose, FALSE, FALSE, 10);

    g_signal_connect(G_OBJECT(button_choose), "clicked", G_CALLBACK(button_choose_clicked), pointer_passer);
    g_hash_table_insert(pointer_passer, &KEY_TEXT_FILENAME, text_filename);

    GtkWidget *checkbox_has_headers = gtk_check_button_new_with_label("File contains column headers");
    g_hash_table_insert(pointer_passer, &KEY_CHECKBOX_HEADER, checkbox_has_headers);

    GtkWidget *button_go = gtk_button_new_with_label("Go");
    gtk_widget_set_valign(button_go, GTK_ALIGN_START);
    gtk_widget_set_halign(button_go, GTK_ALIGN_START);

    g_signal_connect(G_OBJECT(button_go), "clicked", G_CALLBACK(process_file), pointer_passer);

    GtkListStore *list_store_results = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);

    g_hash_table_insert(pointer_passer, &KEY_LIST_STORE, list_store_results);

    GtkTreeIter iter;
    GtkWidget *tree;

    tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store_results));
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

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window), tree);
       gtk_widget_set_size_request (scrolled_window, -1, 250);

    GtkWidget *label_table_name = gtk_label_new("Table name:");

    GtkWidget *entry_table_name = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry_table_name), "gagme");
    g_hash_table_insert(pointer_passer, &KEY_BUFFER_TABLE, entry_table_name);

    g_signal_connect(G_OBJECT(entry_table_name), "key-press-event", G_CALLBACK(table_name_formatter), pointer_passer);
    g_signal_connect(G_OBJECT(entry_table_name), "changed", G_CALLBACK(concat_command), pointer_passer);

    GtkWidget *hbox_table_name = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(hbox_table_name), label_table_name, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(hbox_table_name), entry_table_name, FALSE, FALSE, 10);

    GtkWidget *label_mysql_command = gtk_label_new("CREATE TABLE ");
    gtk_widget_set_halign(label_mysql_command,GTK_ALIGN_START);
    gtk_label_set_line_wrap(GTK_LABEL(label_mysql_command), TRUE);
    gtk_label_set_selectable (GTK_LABEL(label_mysql_command), TRUE);
    g_hash_table_insert(pointer_passer, &KEY_LABEL_MYSQL_COMMAND, label_mysql_command);

    GtkWidget *scrolled_window_command = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_command), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window_command), label_mysql_command);
       gtk_widget_set_size_request (scrolled_window_command, -1, 150);

    GtkWidget *button_close = gtk_button_new_with_label("Close");
            gtk_widget_set_valign(button_close, GTK_ALIGN_START);
    gtk_widget_set_halign(button_close, GTK_ALIGN_END);

    g_signal_connect(G_OBJECT(button_close), "clicked", G_CALLBACK(closeup), pointer_passer);

    GtkWidget *button_copy = gtk_button_new_with_label("Copy");
           gtk_widget_set_valign(button_copy, GTK_ALIGN_START);
    gtk_widget_set_halign(button_copy, GTK_ALIGN_CENTER);
    g_signal_connect(G_OBJECT(button_copy), "clicked", G_CALLBACK(copy_statement), pointer_passer);


    GtkWidget *status_bar = gtk_statusbar_new();
    g_hash_table_insert(pointer_passer, &KEY_STATUS_BAR, status_bar);

    guint status_bar_context_info = gtk_statusbar_get_context_id(GTK_STATUSBAR(status_bar), STATUS_BAR_CONTEXT_INFO);

    guint status_bar_context_info_message_id = gtk_statusbar_push(GTK_STATUSBAR(status_bar), status_bar_context_info, "Ready");
    g_hash_table_insert(pointer_passer, &STATUS_BAR_CONTEXT_INFO_CURRENT_MESSAGE_ID, &status_bar_context_info_message_id);

    GtkWidget *grid = gtk_grid_new();
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
