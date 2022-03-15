#include <glib-2.0/glib.h>
#include <gtk/gtk.h>

#include "headers.h"
/**
 * @file cleanup.c
 * @brief Defines callback functions for closing down the application.
 */

/**
 * Callback that runs on each element of the field analysis hash. The function frees the memory associated with the passed value, which is a `Field_analysis`.
 */
gboolean free_field_analysis_hash(gpointer key, gpointer value, gpointer user_data) {
    Field_analysis *field_analysis = (Field_analysis *)value;
    g_free(field_analysis);
    return TRUE;
}

/**
 * Callback that runs after clicking on the Close button. This function emits the "destroy" signal on the application window. The callback for that emitted signal is cleanup().
 * @param button_close Clicked button.
 * @param data Pointer to the data-passer structure.
 */
void closeup(GtkWidget *button_close, gpointer data) {
    g_print("Closing\n");
    /* For some reason, this strategy emits two destroy signals. Need to resolve. */
    Data_passer *data_passer = (Data_passer *)data;

    //guint *number_of_columnsb = (guint *)g_hash_table_lookup(pointer_passer, &KEY_NUMBER_OF_COLUMNS);
    //   g_print("CLOSEUP Number: %u, pointed address: %p, variable address: %p\n  Key address: %p, key value: %d\n", *number_of_columnsb, number_of_columnsb, &number_of_columnsb, &KEY_NUMBER_OF_COLUMNS, KEY_NUMBER_OF_COLUMNS);

    gtk_widget_destroy(data_passer -> window );
}

/**
 * Callback that runs after the destory signal is emitted on the application window. This function frees all the memory allocated to objects in the pointer passer.
 * @param window Main application window.
 * @param data Pointer to the data-passer structure.
 */
void cleanup(GtkWidget *window, gpointer data) {

    g_print("Cleaning\n");
    Data_passer *data_passer = (Data_passer *)data;
g_print("Cleaning 1\n");
// guint *number_of_columnsb = (guint *)g_hash_table_lookup(pointer_passer, &KEY_NUMBER_OF_COLUMNS);
g_print("Cleaning 2\n");
       //g_print("CLEANUP Number: %u, pointed address: %p, variable address: %p\n  Key address: %p, key value: %d\n", //*number_of_columnsb, number_of_columnsb, &number_of_columnsb, &KEY_NUMBER_OF_COLUMNS, KEY_NUMBER_OF_COLUMNS);


    g_print("Cleaning A\n");
  //  gchar *filename = (gchar *)g_hash_table_lookup(pointer_passer, &KEY_CSV_FILE);
  //  g_print("Cleaning B\n");
    g_free(data_passer -> filename);
    g_print("Cleaning C\n");

//number_of_columnsb = (guint *)g_hash_table_lookup(pointer_passer, &KEY_NUMBER_OF_COLUMNS);
 //   g_print("The number of columns after C is %u\n", *number_of_columnsb);

   
    if (data_passer -> number_of_columns != 0) {
        
       // gchar **column_strings = (gchar **)g_hash_table_lookup(pointer_passer, &KEY_COLUMN_STRINGS);
        g_print("Cleaning E\n");
        for (gint i = 0; i < data_passer -> number_of_columns; i++) {
            g_print("Freeing %d\n", i);
            g_free((data_passer -> column_strings) + i);
        }
    }

    g_print("Cleaning F\n");
  //  null_hash_tester = g_hash_table_lookup(pointer_passer, &KEY_FIELD_CLAUSE);
    if (data_passer -> field_clause != NULL) {
        g_free(data_passer -> field_clause);
    }

    g_print("Cleaning G\n");
  //  null_hash_tester = g_hash_table_lookup(pointer_passer, &KEY_HEADINGS);
    if (data_passer -> headings != NULL) {
        g_slist_free_full(data_passer -> headings, g_free);
    }

    g_print("Cleaning H\n");
  //  null_hash_tester = g_hash_table_lookup(pointer_passer, &KEY_FIELD_ANALYSIS_HASH);
    if (data_passer -> field_analysis_hash != NULL) {
        g_hash_table_foreach_remove(data_passer -> field_analysis_hash, (GHRFunc)free_field_analysis_hash, NULL);
    }
    g_hash_table_destroy(data_passer -> field_analysis_hash);
    g_print("Cleaning Z\n");
}
