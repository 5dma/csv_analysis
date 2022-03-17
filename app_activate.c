#include <gtk/gtk.h>
#include "headers.h"
/**
 * @file app_activate.c
 * @brief Builds the view.
*/

/**
 * Function that starts the GTK loop.
 * @param app Pointer to the GTK application
 * @param data Pointer to passed data, `NULL` in this case.
*/
void on_app_activate(GApplication *app, gpointer data) {

    /* Structure for passing data to functions and callbacks */
    Data_passer *data_passer = g_new(Data_passer, 1);
    data_passer -> app = app;
    data_passer -> field_analysis_hash = g_hash_table_new(g_str_hash, g_str_equal);
    data_passer -> field_clause = NULL;
    data_passer -> filename = NULL;
    data_passer -> headings = NULL;
    data_passer -> number_of_columns = 0;

    data_passer -> datatype_strings[0] = strdup("TINYINT_UNSIGNED");
    data_passer -> datatype_strings[1] = strdup("SMALLINT_UNSIGNED");
    data_passer -> datatype_strings[2] = strdup("MEDIUMINT_UNSIGNED");
    data_passer -> datatype_strings[3] = strdup("INT_UNSIGNED");
    data_passer -> datatype_strings[4] = strdup("BIGINT_UNSIGNED");
    data_passer -> datatype_strings[5] = strdup("TINYINT_SIGNED");
    data_passer -> datatype_strings[6] = strdup("SMALLINT_SIGNED");
    data_passer -> datatype_strings[7] = strdup("MEDIUMINT_SIGNED");
    data_passer -> datatype_strings[8] = strdup("INT_SIGNED");
    data_passer -> datatype_strings[9] = strdup("BIGINT_SIGNED");
    data_passer -> datatype_strings[10] = strdup("DECIMAL");
    data_passer -> datatype_strings[11] = strdup("FLOAT");
    data_passer -> datatype_strings[12] = strdup("TIMESTAMP");
    data_passer -> datatype_strings[13] = strdup("CHAR");
    data_passer -> datatype_strings[14] = strdup("TINYINT_UNSIGNED");
       
    GtkWidget *window = make_window(data_passer);

    gtk_widget_show_all(GTK_WIDGET(window));
    
}
