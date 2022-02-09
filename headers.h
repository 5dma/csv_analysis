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

typedef struct {
    enum data_types field_type;
    gboolean sql_signed;
    int char_width;
    int precision;
    int scale;
    int last_line_change;
} Field_analysis;

typedef struct decimal_sizes {
  int precision;
  int scale;
} decimal_size;

/**
 * Enum for declaring a list store of results.
*/
enum {
    COLUMN_NAME, /**< 0 */
    DATA_TYPE,  /**< 1 */
    DETERMINING_LINE,  /**< 2 */
    N_COLUMNS  /**< 5 */
};


void on_app_activate(GApplication *app, gpointer data);
GtkWidget *make_window(GHashTable *pointer_passer);
void button_choose_clicked (GtkButton *button, gpointer data);

gboolean process_file(GtkButton *button, gpointer data);
GSList *make_headings(char *csv_line);
GSList *make_forced_headings(char *csv_line);
void initialize_field_analysis(gpointer heading, gpointer data);
GDestroyNotify free_headings(gpointer data);
void display_results(GHashTable *field_analysis_hash);

gboolean is_unsigned_int(const gchar *token, gshort min, guint64 max);
gboolean is_signed_int(const gchar *token, gint64 min, gint64 max);
gboolean is_decimal(const gchar *token, regex_t *decimal_regex);
gboolean is_float(const gchar *token);
gboolean is_timestamp(const gchar *token, regex_t *timestamp_regex);
void cleanup (GtkWidget* window, gpointer data);
void closeup(GtkWidget *button_close, gpointer data);
void copy_statement(GtkWidget *button_copy, gpointer data);
gboolean table_name_formatter(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void concat_command(GtkEditable* self,  gpointer data);
void filename_changed(GtkEntry *text_filename, gpointer data);

regex_t make_decimal_regex();
regex_t make_timestamp_regex();


/** Hash key for the pointer to the application window. */
static gint KEY_WINDOW = 0;
static gint KEY_CSV_FILE = 1;
static gint KEY_APP = 2;
static gint KEY_TEXT_FILENAME = 3;
static gint KEY_CHECKBOX_HEADER = 4;
static gint KEY_FIELD_ANALYSIS_HASH = 5;
static gint KEY_DATA_TYPE_STRINGS = 6;
static gint KEY_LIST_STORE = 7;
static gint KEY_BUFFER_TABLE = 8;
static gint KEY_COLUMN_STRINGS = 9;
static gint KEY_CURRENT_COLUMN_NUMBER = 10;
static gint KEY_LABEL_MYSQL_COMMAND = 11;
static gint KEY_STATUS_BAR = 12;
static gint STATUS_BAR_CONTEXT_INFO_CURRENT_MESSAGE_ID = 13;
static gint KEY_CURRENT_LINE_NUMBER = 14;
static gint KEY_FIELD_CLAUSE = 15;
static gint KEY_BUTTON_GO = 16;
static gint KEY_BUTTON_COPY = 17;
static gint KEY_HEADINGS = 18;

static gchar *STATUS_BAR_CONTEXT_INFO = "STATUS_BAR_CONTEXT_INFO";


#endif
