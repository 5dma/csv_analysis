#include <glib-2.0/glib.h>
#include <gtk/gtk.h>

#include "headers.h"
/**
 * @file display_results.c
 * @brief Defines functions for displaying the results of the CSV scan.
 */

/**
 * Concatenates the following strings into one long string:
 * - String literal `CREATE TABLE `.
 * - MySQL table name.
 * - List of columns and their definitions.
 * - String literal `);`.
 * @param self A `GtkEditable` containing the MySQL table name.
 * @param data Pointer to the data-passer hash.
 */
void concat_command(GtkEditable *self, gpointer data) {
    Data_passer *data_passer = (Data_passer *)data;
    //  GtkEntry *buffer_table = (GtkEntry *)g_hash_table_lookup(pointer_passer, &KEY_BUFFER_TABLE);
    // gchar *field_clause = (gchar *)g_hash_table_lookup(pointer_passer, &KEY_FIELD_CLAUSE);

    //   GtkWidget *label_mysql_command = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_LABEL_MYSQL_COMMAND);

    /* Not sure if the following pointer needs to be freed, as it is set into the label widget. */
    gchar *complete_command = g_strconcat("CREATE TABLE ", gtk_entry_get_text((GtkEntry *)(data_passer->entry_table_name)), " (", data_passer->field_clause, ");", NULL);

    gtk_label_set_text(GTK_LABEL(data_passer->label_mysql_command), complete_command);
}

/**
 * Returns the number of columns in a CSV file, by counting the number of keys in the field_analysis_hash.
 * @param field_analysis_hash Hash table desribing each column in the CSV file.
 */
guint get_number_of_columns(GHashTable *field_analysis_hash) {
    GList *keys = g_hash_table_get_keys(field_analysis_hash);
    return g_list_length(keys);
}

/**
 * Displays the result for a single column in the CSV file. The result is added to the list store holding the results, and also added to an allocated array of character strings. This function is a callback from an iterator over the list of headings.
 * @param heading Hash table desribing each column in the CSV file.
 * @param data Pointer to the data-passer structure.
 */
void display_single_result(gpointer heading, gpointer data) {
    gchar *key = (gchar *)heading;

    g_print("Processing the heading %s\n",key);

    Data_passer *data_passer = (Data_passer *)data;

    //    GHashTable *field_analysis_hash = (GHashTable *)g_hash_table_lookup(pointer_passer, &KEY_FIELD_ANALYSIS_HASH);

    Field_analysis *field_analysis = (Field_analysis *)g_hash_table_lookup(data_passer->field_analysis_hash, key);

    enum data_types field_type = field_analysis->field_type;
    //  char **datatype_strings = (char **)g_hash_table_lookup(pointer_passer, &KEY_DATA_TYPE_STRINGS);

    /* The following string holds a formatted string of the MySQL type, such as `TINYINT` or `CHAR(40)`. */
    gchar datatype_string[100];
    if (field_type == CHAR) {
        g_snprintf(datatype_string, 50, "%s(%d)", *(data_passer->datatype_strings + field_type), field_analysis->char_width);
    } else {
        g_snprintf(datatype_string, 50, "%s", *(data_passer->datatype_strings + field_type));
    }

    //   GtkListStore *list_store_results = (GtkListStore *)g_hash_table_lookup(pointer_passer, &KEY_LIST_STORE);
    GtkTreeIter iter;
    gtk_list_store_append(data_passer->list_store_results, &iter);

    gtk_list_store_set(data_passer->list_store_results, &iter,
                       COLUMN_NAME, key,
                       DATA_TYPE, datatype_string,
                       DETERMINING_LINE, field_analysis->last_line_change,
                       -1);

    /* Retrieve the current column number, which is an index into the array of character
       strings of the results. Copy the current formatted string into that index. */
    //  guint *current_column_number = (guint *)g_hash_table_lookup(pointer_passer, &KEY_CURRENT_COLUMN_NUMBER);

    //   g_print("The current column number is %u\n", *current_column_number);

    //  gchar **column_strings = (gchar **)g_hash_table_lookup(pointer_passer, &KEY_COLUMN_STRINGS);
    //   g_print("STEP 1\n");
    gchar *intermediate = g_strconcat(key, " ", datatype_string, NULL);
    g_print("STEP 2 intermediate %s\n", intermediate);
    *(data_passer->column_strings + data_passer->current_column_number) = g_strdup(intermediate);
   // g_print("STEP 3 inside column strings %s\n", *(data_passer->column_strings + data_passer->current_column_number));
    g_free(intermediate);
    //  g_print("STEP 4\n");

    (data_passer->current_column_number)++;
}

/**
 * Iterates over the accumulated results, displaying each one. The function does the following:
 * -# Declare an array of strings, each a MySQL type. Insert the array into pointer-passer.
 * -# Get the number of columns in the field analysis (which is the number of columns in the CSV file). Insert into the pointer-passer.
 * -# Declare an array of empty character strings. The length of the array is the number of columns from the previous step.
 * -# Retrieve the list of headings from pointer passer.
 * -# Iterate over the list of headings, formatting the results. Doing so ensures the results appear in the same order as they are in the original CSV file. See display_single_result().
 * @param pointer_passer Pointer to the data-passer structure.
 */
void display_results(Data_passer *data_passer) {

   

    data_passer->number_of_columns = get_number_of_columns(data_passer->field_analysis_hash);

    g_print("Number of columns is %u\n", data_passer->number_of_columns);

    /* column_strings holds the phrases for each column, such as id_number TINYINT. There are n columns,
        so we need to allocate n+1 pointers for these phrases. That's because further down we have a GLib
        function g_strjoinv() that joints an array of string pointers, and the last pointer in that array
        must be NULL.  */

    //gchar *column_strings[data_passer->number_of_columns + 1];

 g_print("Here we are A\n");


    gchar *trash;
    data_passer -> column_strings = (gchar **)malloc( (data_passer->number_of_columns + 1)* sizeof(trash));
    for (int i = 0; i <= data_passer->number_of_columns; i++) {
        data_passer -> column_strings[i] = NULL;
    }

 g_print("Here we are B\n");

    data_passer->current_column_number = 0;
    g_print("Current column number %u\n", data_passer->current_column_number);

   
    g_slist_foreach(data_passer->headings, display_single_result, data_passer);
  
    // gchar *field_clause = g_strjoinv(", ", column_strings);
//g_print("OUTSIDE column strings %s\n", *(data_passer->column_strings + 1));
    //   g_print("STEP 6\n");
    data_passer->field_clause = g_strjoinv(", ", data_passer -> column_strings);
g_print("HUGE FIELC CLAUSE %s\n", data_passer->field_clause);

    //  g_hash_table_insert(pointer_passer, &KEY_FIELD_CLAUSE, field_clause);
    concat_command(NULL, (gpointer)data_passer);

    //  GtkWidget *button_copy = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_BUTTON_COPY);
    gtk_widget_set_sensitive(data_passer->button_copy, TRUE);

    //  GtkWidget *entry_table_name = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_BUFFER_TABLE);
    gtk_widget_set_sensitive(data_passer->entry_table_name, TRUE);

    //  number_of_columnsb = (guint *)g_hash_table_lookup(pointer_passer, &KEY_NUMBER_OF_COLUMNS);
    // g_print("BARF TWO Number: %u, pointed address: %p, variable address: %p\n  Key address: %p, key value: %d\n", *number_of_columnsb, number_of_columnsb, &number_of_columnsb, &KEY_NUMBER_OF_COLUMNS, KEY_NUMBER_OF_COLUMNS);
}

/**
 * Callback that prevents the user from entering anything in a `GtkCellEditable` other than digits and a decimal point. The actual allowed keys are [0-9], decimal point, backspace, delete, cursor right, and cursor left.
 * @param widget Widget where the edit is occurring.
 * @param event Key that was pressed.
 * @param data `NULL` in this case.
 * @return  `FALSE` if an allowed key was pressed, `TRUE` otherwise.
 * \sa started_cell_editing()
 */
gboolean table_name_formatter(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    //    GHashTable *pointer_passer = (GHashTable *)data;
    Data_passer *data_passer = (Data_passer *)data;

    if (
        (event->keyval >= GDK_KEY_0) && (event->keyval <= GDK_KEY_9) ||
        (event->keyval >= GDK_KEY_KP_0) && (event->keyval <= GDK_KEY_KP_9) ||
        (event->keyval >= GDK_KEY_A) && (event->keyval <= GDK_KEY_Z) ||
        (event->keyval >= GDK_KEY_a) && (event->keyval <= GDK_KEY_z) ||
        (event->keyval == GDK_KEY_dollar) ||
        (event->keyval == GDK_KEY_underscore) ||
        (event->keyval == GDK_KEY_BackSpace) ||
        (event->keyval == GDK_KEY_Left) ||
        (event->keyval == GDK_KEY_Right) ||
        (event->keyval == GDK_KEY_KP_Left) ||
        (event->keyval == GDK_KEY_KP_Right) ||
        (event->keyval == GDK_KEY_Delete)) {
        return FALSE;
    }
    return TRUE;
}
