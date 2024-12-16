#include <gtk/gtk.h>
#include <regex.h>
#ifndef __HEADER
#define __HEADER
/**
 * @file headers.h
 * @brief C headers.
 */


/**
 * Enum for declaring a list store of accounts. These correspond to the MySQL data types (see [Data Types](https://dev.mysql.com/doc/refman/5.7/en/data-types.html)). This enumeration is used in Field_analysis.
 */
enum data_types {
    GARBAGE, /**< Artificial type for initialization */
    TINYINT_UNSIGNED, /**< 0 - 255 */
    SMALLINT_UNSIGNED, /**< 0 - 65535 */
    MEDIUMINT_UNSIGNED, /**< 0 - 16777215 */
    INT_UNSIGNED, /**< 0 - 4294967295 */
    BIGINT_UNSIGNED, /**< 0 - 2<sup>64</sup> &minus; 1 */
    TINYINT_SIGNED, /**<  &minus;128 - 127 */
    SMALLINT_SIGNED, /**<  &minus;32768 - 32767 */
    MEDIUMINT_SIGNED, /**<  &minus;8388608 - 8388607 */
    INT_SIGNED, /**<  &minus;2147483648 - 2147483647 */
    BIGINT_SIGNED, /**< &minus;2<sup>63</sup> - 2<sup>63</sup> &minus; 1 */
    DECIMAL, /**< Fixed number with an optional decimal point */
    FLOAT, /**<  Floating number */
    TIMESTAMP, /**< A timestamp */
    CHAR /**< Any character string */
};


/**
 * Enum for describing how fields are surrounded by double quotes.
 */
enum field_quoting_options {
    NEVER, /**< Fields never surrounded by double quotes. */
    ALWAYS, /**< Fields always surrounded by double quotes. */
    OPTIONAL /**< Fields may be surrounded by double quotes. */
};

/**
 * Structure containing the results of a column.
 */
typedef struct {
    enum data_types field_type;  /**< One of the possible MySQL field types enumerated in [Data Types](https://dev.mysql.com/doc/refman/5.7/en/data-types.html). */
    gboolean sql_signed; /**< Indicates if the data type is signed. */
    int char_width; /**< Width of a `CHAR` field. */
    int precision;  /**< Precision (number of digits) of a `DECIMAL`. See [Fixed-Point Types (Exact Value) - DECIMAL, NUMERIC](https://dev.mysql.com/doc/refman/5.7/en/fixed-point-types.html). */
    int scale;  /**< Scale (number of digits after decimal point) of a `DECIMAL`. See [Fixed-Point Types (Exact Value) - DECIMAL, NUMERIC](https://dev.mysql.com/doc/refman/5.7/en/fixed-point-types.html). */
    int last_line_change; /**< Most recent line in the CSV file that determined the column's type. */
    gchar determining_value[4096]; /**< Most recent value in the column that determined the column's type. */
} Field_analysis;

/**
 * Enum for declaring a list store of results.
 */
enum {
    COLUMN_NAME,      /**< 0 */
    DATA_TYPE,        /**< 1 */
    DETERMINING_LINE, /**< 2 */
    DETERMINING_VALUE, /**< 3 */
    N_COLUMNS         /**< 4 */
};

/**
 * Structure for passing data between functions.
*/ 
typedef struct {
    GtkWidget *window; /**< Application's window */
    gchar *filename; /**< Path and filename of the CSV file. */
    GApplication *app;  /**< Application */
    GtkWidget *text_filename;  /**< Path and filename of the CSV file (in the UI) */
    GtkWidget *checkbox_has_headers; /**< Path and filename of the CSV file (in the UI) */
    GHashTable *field_analysis_hash;  /**< Hash keyed by column name; values are the analysis of the values in the column. See Field_analysis. */
    gchar *datatype_strings[15]; /**< List of MySQL data types. See [Data Types](https://dev.mysql.com/doc/refman/5.7/en/data-types.html). */
    GtkListStore *list_store_results; /**< List store containing the analysis results. There is one element in the store for each column in the CSV file. */
    GtkWidget *entry_table_name; /**< MySQL table name in the UI. */
    gchar **column_strings; /**< List of strings used for column names in a MySQL table. */
    guint current_column_number; /**< Current column number as we iterate over all columns in a single CSV row. */
    GtkWidget *label_mysql_command; /**< Final MySQL command displayed in the UI. */
    GtkWidget *status_bar; /**< Status bar in the I. */
    guint status_bar_context_info_message_id;  /**< Status bar's context ID.  */
    gchar *field_clause;  /**< Clause in a MySQL field definition, such as `CHAR(30)`.  */
    GtkWidget *button_go; /**< \b Go button in the UI. */
    GtkWidget *button_copy;  /**< \b Copy button in the UI. */
    GSList *headings; /**< GSList containing column headings. */
    guint number_of_columns; /**< Number of columns in the CSV file. */
    GtkWidget *combo_field_delimeter; /**< UI control specifying the field delimiter. */
    GtkWidget *combo_fields_enclosed; /**< UI control specifying if fields are always, never, or optionally surrounded by double quotes. */
    guint current_line_number; /**< Current line number we are reading from the CSV file. */
    FILE *fp; /**< Handle for the CSV file. */
    GMainLoop *gloop; /**< The main loop, required to set up the threading. */
} Data_passer;

void on_app_activate(GApplication *app, gpointer data);
GtkWidget *make_window(Data_passer *data_passer);
void button_choose_clicked(GtkButton *button, gpointer data);

gboolean process_file(GtkButton *button, gpointer data);
void make_headings(gchar *csv_line, enum field_quoting_options field_quoting, Data_passer *data_passer);
GSList *make_forced_headings(char *csv_line);
void strip_quotes(gchar **quoted_string_ptr);
void initialize_field_analysis(gpointer heading, gpointer data);
void assign_char_field_type(const char *csv_value, const int current_line_number, Field_analysis *field_analysis);
GDestroyNotify free_headings(gpointer data);
void display_results(Data_passer *data_passer);

gboolean is_unsigned_int(const gchar *token, gshort min, guint64 max);
gboolean is_signed_int(const gchar *token, gint64 min, gint64 max);
gboolean is_decimal(const gchar *token, regex_t *decimal_regex);
gboolean is_float(const gchar *token);
gboolean is_timestamp(const gchar *token, regex_t *timestamp_regex);
void cleanup(GtkWidget *window, gpointer data);
void closeup(GtkWidget *button_close, gpointer data);
void copy_statement(GtkWidget *button_copy, gpointer data);
gboolean table_name_formatter(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void concat_command(GtkEditable *self, gpointer data);
void filename_changed(GtkEntry *text_filename, gpointer data);

void change_commas_to_tabs(gchar **csv_line_ptr);
void change_commas_to_tabs_with_quotes(gchar **csv_line_ptr);
void change_commas_to_tabs_with_optional_quotes(gchar **csv_line_ptr);

regex_t make_decimal_regex();
regex_t make_timestamp_regex();


static gchar *STATUS_BAR_CONTEXT_INFO = "STATUS_BAR_CONTEXT_INFO"; /**< Context description for the status bar. See [get_context_id](https://docs.gtk.org/gtk3/method.Statusbar.get_context_id.html). */
static guint WINDOW_WIDTH = 400; /**< Width of the application window. */

#endif
