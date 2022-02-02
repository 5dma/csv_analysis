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

    GtkWidget *label_csv_file = gtk_label_new("CSV file:");
    GtkWidget *text_filename = gtk_entry_new();
    GtkWidget *button_choose = gtk_button_new_with_label("Choose...");

    g_signal_connect(G_OBJECT(button_choose), "clicked", G_CALLBACK(button_choose_clicked), pointer_passer);
    g_hash_table_insert(pointer_passer, &KEY_TEXT_FILENAME, text_filename);

    GtkWidget *hbox_file_choose = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(hbox_file_choose), label_csv_file, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(hbox_file_choose), text_filename, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(hbox_file_choose), button_choose, TRUE, TRUE, 10);

    GtkWidget *checkbox_has_headers = gtk_check_button_new();
    g_hash_table_insert(pointer_passer, &KEY_CHECKBOX_HEADER, checkbox_has_headers);

    GtkWidget *label_has_headers = gtk_label_new("File contains column headers");
    GtkWidget *button_go = gtk_button_new_with_label("Go");
    g_signal_connect(G_OBJECT(button_go), "clicked", G_CALLBACK(process_file), pointer_passer);

    GtkWidget *hbox_headers_go = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(hbox_headers_go), checkbox_has_headers, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(hbox_headers_go), label_has_headers, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(hbox_headers_go), button_go, TRUE, TRUE, 10);

    GtkListStore *list_store_results = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);

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

    GtkWidget *label_table_name = gtk_label_new("Table name:");
    GtkWidget *text_table_name = gtk_entry_new();
    GtkWidget *hbox_table_name = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(hbox_table_name), label_table_name, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(hbox_table_name), text_table_name, TRUE, TRUE, 10);

    GtkWidget *text_view_sql_command = gtk_text_view_new();

    GtkWidget *button_close = gtk_button_new_with_label("Close");
    GtkWidget *button_copy = gtk_button_new_with_label("Copy");
    GtkWidget *hbox_close_copy = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(hbox_close_copy), button_close, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(hbox_close_copy), button_copy, TRUE, TRUE, 10);

    GtkWidget *status_bar = gtk_statusbar_new();

    GtkWidget *vbox_ui = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox_ui), hbox_file_choose, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox_ui), hbox_headers_go, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox_ui), tree, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox_ui), hbox_table_name, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox_ui), text_view_sql_command, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox_ui), hbox_close_copy, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox_ui), status_bar, TRUE, TRUE, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox_ui);

    /* Upon destroying the application, free memory in data structures in pointer_passer. */
    //  g_signal_connect(window, "destroy", G_CALLBACK(free_memory), pointer_passer);
    return window;
}
