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
 * @param data Pointer to the pointer-passer hash.
*/
void concat_command(GtkEditable *self, gpointer data) {
    GHashTable *pointer_passer = (GHashTable *)data;
    GtkEntry *buffer_table = (GtkEntry *)g_hash_table_lookup(pointer_passer, &KEY_BUFFER_TABLE);
    gchar *field_clause = (gchar *)g_hash_table_lookup(pointer_passer, &KEY_FIELD_CLAUSE);

    GtkWidget *label_mysql_command = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_LABEL_MYSQL_COMMAND);

    /* Not sure if the following pointer needs to be freed, as it is set into the label widget. */
    gchar *complete_command = g_strconcat("CREATE TABLE ", gtk_entry_get_text(buffer_table), " ", field_clause, ");", NULL);

    gtk_label_set_text(GTK_LABEL(label_mysql_command), complete_command);
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
 * @param data Pointer to the pointer-passer hash.
*/
void display_single_result(gpointer heading, gpointer data) {
    gchar *key = (gchar *)heading;
    GHashTable *pointer_passer = (GHashTable *)data;

    GHashTable *field_analysis_hash = (GHashTable *)g_hash_table_lookup(pointer_passer, &KEY_FIELD_ANALYSIS_HASH);

    Field_analysis *field_analysis = (Field_analysis *)g_hash_table_lookup(field_analysis_hash, key);

    enum data_types field_type = field_analysis->field_type;
    char **datatype_strings = (char **)g_hash_table_lookup(pointer_passer, &KEY_DATA_TYPE_STRINGS);

    /* The following string holds a formatted string of the MySQL type, such as `TINYINT` or `CHAR(40)`. */
    gchar datatype_string[100];
    if (field_type == CHAR) {
        g_snprintf(datatype_string, 50, "%s(%d)", *(datatype_strings + field_type), field_analysis->char_width);
    } else {
        g_snprintf(datatype_string, 50, "%s", *(datatype_strings + field_type));
    }

    GtkListStore *list_store_results = (GtkListStore *)g_hash_table_lookup(pointer_passer, &KEY_LIST_STORE);
    GtkTreeIter iter;
    gtk_list_store_append(list_store_results, &iter);

    gtk_list_store_set(list_store_results, &iter,
                       COLUMN_NAME, key,
                       DATA_TYPE, datatype_string,
                       DETERMINING_LINE, field_analysis->last_line_change,
                       -1);

    /* Retrieve the current column number, which is an index into the array of character
       strings of the results. Copy the current formatted string into that index. */
    guint *current_column_number = (guint *)g_hash_table_lookup(pointer_passer, &KEY_CURRENT_COLUMN_NUMBER);

    gchar **column_strings = (gchar **)g_hash_table_lookup(pointer_passer, &KEY_COLUMN_STRINGS);

    char *intermediate = g_strconcat(key, " ", datatype_string, NULL);
    *(column_strings + *current_column_number) = g_strdup(intermediate);
    g_free(intermediate);

    (*current_column_number)++;
}

/**
 * Iterates over the accumulated results, displaying each one. The function does the following:
 * -# Declare an array of strings, each a MySQL type. Insert the array into pointer-passer.
 * -# Get the number of columns in the field analysis (which is the number of columns in the CSV file). Insert into the pointer-passer.
 * -# Declare an array of empty character strings. The length of the array is the number of columns from the previous step.
 * -# Retrieve the list of headings from pointer passer.
 * -# Iterate over the list of headings, formatting the results. Doing so ensures the results appear in the same order as they are in the original CSV file. See display_single_result().
 * @param pointer_passer Pointer to the pointer-passer hash.
*/
void display_results(GHashTable *pointer_passer) {

    gchar *datatype_strings[14] = {
        "TINYINT_UNSIGNED",
        "SMALLINT_UNSIGNED",
        "MEDIUMINT_UNSIGNED",
        "INT_UNSIGNED",
        "BIGINT_UNSIGNED",
        "TINYINT_SIGNED",
        "SMALLINT_SIGNED",
        "MEDIUMINT_SIGNED",
        "INT_SIGNED",
        "BIGINT_SIGNED",
        "DECIMAL",
        "FLOAT",
        "TIMESTAMP",
        "CHAR"};

    g_hash_table_insert(pointer_passer, &KEY_DATA_TYPE_STRINGS, datatype_strings);

    GHashTable *field_analysis_hash = (GHashTable *)g_hash_table_lookup(pointer_passer, &KEY_FIELD_ANALYSIS_HASH);

    guint number_of_columns = get_number_of_columns(field_analysis_hash);
    g_hash_table_insert(pointer_passer, &KEY_NUMBER_OF_COLUMNS, &number_of_columns);
    
    gchar *column_strings[number_of_columns + 1]; /* Memory freed in cleanup() */
    g_hash_table_insert(pointer_passer, &KEY_COLUMN_STRINGS, column_strings);

    guint current_column_number = 0;
    g_hash_table_insert(pointer_passer, &KEY_CURRENT_COLUMN_NUMBER, &current_column_number);

    GSList *headings = (GSList *)g_hash_table_lookup(pointer_passer, &KEY_HEADINGS);
    g_slist_foreach(headings, display_single_result, pointer_passer);

    column_strings[number_of_columns] = NULL;

    gchar *field_clause = g_strjoinv(", ", column_strings);
    g_hash_table_insert(pointer_passer, &KEY_FIELD_CLAUSE, field_clause);
    concat_command(NULL, (gpointer)pointer_passer);

    GtkWidget *button_copy = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_BUTTON_COPY);
    gtk_widget_set_sensitive(button_copy, TRUE);

    GtkWidget *entry_table_name = (GtkWidget *)g_hash_table_lookup(pointer_passer, &KEY_BUFFER_TABLE);
    gtk_widget_set_sensitive(entry_table_name, TRUE);
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
    GHashTable *pointer_passer = (GHashTable *)data;
    g_print("I got to the update\n");
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
        g_print("I pressed OK character\n");

        return FALSE;
    }
    return TRUE;
}
