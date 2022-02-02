#include <gtk/gtk.h>
#include <math.h>
#include <regex.h>
#include <stdio.h>

#include "headers.h"

gboolean omg(gboolean has_header_line) {
    FILE *fp = fopen("/home/abba/maryland-politics/checkbook_2020/checkbook.csv", "r");
    //FILE *fp = fopen("/tmp/county_government_checkbook_2020.csv", "r");
    if (fp == NULL) {
        g_print("Could not open the file\n");
        return FALSE;
    }
    char *csv_line;
    size_t max_number_characters = 1000;
    size_t len = 0;

    csv_line = (gchar *)g_malloc(max_number_characters * sizeof(char));

    if (csv_line == NULL) {
        g_print("We have a malloc problem\n");
        exit(-1);
    }

    GSList *headings = NULL;
    gboolean on_first_line = TRUE;

    char *token;
    char *delimiter = "\t";

    GHashTable *field_analysis_hash = g_hash_table_new(g_int_hash, g_str_equal);

    regex_t decimal_regex = make_decimal_regex();
    regex_t timestamp_regex = make_timestamp_regex();
    int line_number = 1;
    while (getline(&csv_line, &len, fp) != -1) {
        g_print("Processing line %d\n", line_number);
        if (on_first_line) {
            if (has_header_line) {
                headings = make_headings(csv_line);
            } else {
                headings = make_forced_headings(csv_line);
            }
            on_first_line = FALSE;
            g_slist_foreach(headings, initialize_field_analysis, &field_analysis_hash);
            continue;
        }
        int i = 0;
        gchar *key = NULL;
        gpointer value = NULL;
        while ((token = strsep(&csv_line, delimiter)) != NULL) {
            key = strdup((gchar *)g_slist_nth_data(headings, i));
            value = g_hash_table_lookup(field_analysis_hash, key);
            if (value == NULL) {
                g_print("There was a critical failure in looking up the key.\n");
                exit(-1);
            }
            Field_analysis *field_analysis = (Field_analysis *)value;

            enum data_types field_type = field_analysis->field_type;

            gchar *csv_value = g_strstrip(token);

            gboolean passes_test;
            switch (field_type) {
                case TINYINT_UNSIGNED:
                    passes_test = is_unsigned_int(csv_value, 0, 255);
                    if (passes_test) break;
                    passes_test = is_unsigned_int(csv_value, 0, 65535);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_unsigned_int(csv_value, 0, 16777215);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_unsigned_int(csv_value, 0, 4294967295);
                    if (passes_test) {
                        field_analysis->field_type = INT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -128, 127);
                    if (passes_test) {
                        field_analysis->field_type = TINYINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                case SMALLINT_UNSIGNED:
                    passes_test = is_unsigned_int(csv_value, 0, 65535);
                    if (passes_test) break;
                    passes_test = is_unsigned_int(csv_value, 0, 16777215);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_unsigned_int(csv_value, 0, 4294967295);
                    if (passes_test) {
                        field_analysis->field_type = INT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -128, 127);
                    if (passes_test) {
                        field_analysis->field_type = TINYINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                case MEDIUMINT_UNSIGNED:
                    passes_test = is_unsigned_int(csv_value, 0, 16777215);
                    if (passes_test) break;
                    passes_test = is_unsigned_int(csv_value, 0, 4294967295);
                    if (passes_test) {
                        field_analysis->field_type = INT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -128, 127);
                    if (passes_test) {
                        field_analysis->field_type = TINYINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                case INT_UNSIGNED:
                    passes_test = is_unsigned_int(csv_value, 0, 4294967295);
                    if (passes_test) break;
                    passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -128, 127);
                    if (passes_test) {
                        field_analysis->field_type = TINYINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                case BIGINT_UNSIGNED:
                    passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
                    if (passes_test) break;
                    passes_test = is_signed_int(csv_value, -128, 127);
                    if (passes_test) {
                        field_analysis->field_type = TINYINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                case TINYINT_SIGNED:
                    passes_test = is_signed_int(csv_value, -128, 127);
                    if (passes_test) break;
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }

                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }

                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                case SMALLINT_SIGNED:
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) break;
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        break;
                    }

                case MEDIUMINT_SIGNED:
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) break;
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }

                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                case INT_SIGNED:
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) break;
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        break;
                    }

                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        break;
                    }

                case BIGINT_SIGNED:
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) break;
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                case DECIMAL:
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) break;
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        break;
                    }
                case FLOAT:
                    passes_test = is_float(csv_value);
                    if (passes_test) break;
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        break;
                    }

                case TIMESTAMP:
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) break;
                default:
                    field_analysis->field_type = CHAR;
                    guint csv_value_length = strlen(csv_value);
                    if (field_analysis->char_width < csv_value_length) {
                        field_analysis->char_width = csv_value_length;
                        field_analysis->last_line_change = line_number;
                    }
            }
            i++;
   
        }
    
        line_number++;
    }
    fclose(fp);
    /* Free memory in the list of headings */
    //g_slist_free_full(headings, (GDestroyNotify)free_headings);

    display_results(field_analysis_hash);

    return TRUE;
}