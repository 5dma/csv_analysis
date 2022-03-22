#include <gtk/gtk.h>
#include <regex.h>
#ifndef __HEADER
#define __HEADER
/**
 * @file headers.h
 * @brief C headers.
 */

/**
 * Enum for declaring a list store of accounts.
 */
enum data_types {
    TINYINT_UNSIGNED,
    SMALLINT_UNSIGNED,
    MEDIUMINT_UNSIGNED,
    INT_UNSIGNED,
    BIGINT_UNSIGNED,
    TINYINT_SIGNED,
    SMALLINT_SIGNED,
    MEDIUMINT_SIGNED,
    INT_SIGNED,
    BIGINT_SIGNED,
    DECIMAL,
    FLOAT,
    TIMESTAMP,
    CHAR
};


/**
 * Enum for describing fields surrounded by double quotes.
 */
enum field_quoting_options {
    NEVER,
    ALWAYS,
    OPTIONAL
};


typedef struct {
    enum data_types field_type;
    gboolean sql_signed;
    int char_width;
    int precision;
    int scale;
    int last_line_change;
    gchar determining_value[4096];
} Field_analysis;

typedef struct decimal_sizes {
    int precision;
    int scale;
} decimal_size;

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
    GtkWidget *window;
    gchar *csv_file;
    gchar *filename;
    GApplication *app;
    GtkWidget *text_filename;
    GtkWidget *checkbox_has_headers;
    GHashTable *field_analysis_hash;
    const gchar *datatype_strings[14];
    GtkListStore *list_store_results;
    GtkWidget *entry_table_name;
    gchar **column_strings;
    guint current_column_number;
    GtkWidget *label_mysql_command;
    GtkWidget *status_bar;
    guint status_bar_context_info_message_id;
    gchar *field_clause;
    GtkWidget *button_go;
    GtkWidget *button_copy;
    GSList *headings;
    guint number_of_columns;
    GtkWidget *combo_field_delimeter;
    GtkWidget *combo_fields_enclosed;
    guint current_line_number;
    gboolean finished_processing_file;
    FILE *fp;
} Data_passer;

void on_app_activate(GApplication *app, gpointer data);
GtkWidget *make_window(Data_passer *data_passer);
void button_choose_clicked(GtkButton *button, gpointer data);

gboolean process_file(GtkButton *button, gpointer data);
GSList *make_headings(gchar *csv_line, enum field_quoting_options fields_quoting);
GSList *make_forced_headings(char *csv_line);
void strip_quotes(gchar **quoted_string_ptr);
void initialize_field_analysis(gpointer heading, gpointer data);
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

static gchar *STATUS_BAR_CONTEXT_INFO = "STATUS_BAR_CONTEXT_INFO";
static guint WINDOW_WIDTH = 400;

#endif
