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


gboolean omg();
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



regex_t make_decimal_regex();
regex_t make_timestamp_regex();

#endif
