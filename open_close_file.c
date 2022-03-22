#include <gtk/gtk.h>
#include <math.h>
#include <regex.h>
#include <stdio.h>
#define G_LOG_USE_STRUCTURED
#include <glib-2.0/glib.h>

#include "headers.h"
/**
 * @file open_close_file.c
 * @brief Functions for processing the CSV file.
 */

gboolean line_number_in_status_bar(gpointer data) {
    Data_passer *data_passer = (Data_passer *)data;
    data_passer->current_line_number;
    // g_print("Here I am in line number %d\n", data_passer->current_line_number);

    guint status_bar_context_info = gtk_statusbar_get_context_id(GTK_STATUSBAR(data_passer->status_bar), STATUS_BAR_CONTEXT_INFO);

    gtk_statusbar_remove(GTK_STATUSBAR(data_passer->status_bar), status_bar_context_info, data_passer->status_bar_context_info_message_id);

    gchar progress_message[100];
    g_snprintf(progress_message, 100, "Reading line %d...", data_passer->current_line_number);
    data_passer->status_bar_context_info_message_id = gtk_statusbar_push(GTK_STATUSBAR(data_passer->status_bar), status_bar_context_info, progress_message);
    /* if (data_passer->finished_processing_file) {
        return FALSE;
    } else {
        return TRUE;
    } */
    return FALSE;
}

gboolean process_thread(gpointer data) {
    Data_passer *data_passer = (Data_passer *)data;

    gchar *csv_line;
    size_t max_number_characters = 1000;
    size_t len = 0;

    csv_line = (gchar *)g_malloc(max_number_characters * sizeof(char));

    if (csv_line == NULL) {
        g_print("We have a malloc problem\n");
        exit(-1);
    }

    gboolean on_first_line = TRUE;

    char *token;

    /* Retrieve the field delimiter */
    char *delimiter = (g_strcmp0(gtk_combo_box_get_active_id((GtkComboBox *)(data_passer->combo_field_delimeter)), "0") == 0) ? "\t" : ",";
    g_print("Here B\n");

    regex_t decimal_regex = make_decimal_regex();
    regex_t timestamp_regex = make_timestamp_regex();
    gint line_number = 0;
    data_passer->current_line_number = line_number;

    gboolean has_header_line = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data_passer->checkbox_has_headers));

    enum field_quoting_options field_quoting;

    /* If the delimiter is a comma, then replace commas with tabs. */
    const gchar *active_quoted_fields = gtk_combo_box_get_active_id((GtkComboBox *)data_passer->combo_fields_enclosed);

    switch (*active_quoted_fields) {
        case '0':
            field_quoting = NEVER;
            break;
        case '1':
            field_quoting = ALWAYS;
            break;
        case '2':
            field_quoting = OPTIONAL;
            break;
    }

    while (getline(&csv_line, &len, data_passer->fp) != -1) {
        line_number++;
        data_passer->current_line_number = line_number;

        gdk_threads_add_idle((GSourceFunc)line_number_in_status_bar, data);
        //   g_idle_add((GSourceFunc)line_number_in_status_bar, data);
        /* If the delimiter is a comma, then replace commas with tabs. */
        if ((g_strcmp0(delimiter, ",") == 0)) {
            switch (field_quoting) {
                case NEVER:
                    change_commas_to_tabs(&csv_line);
                    break;
                case OPTIONAL:
                    change_commas_to_tabs_with_optional_quotes(&csv_line);
                    break;
                case ALWAYS:
                    change_commas_to_tabs_with_quotes(&csv_line);
            }
        }

        if (on_first_line) {
            on_first_line = FALSE;
            if (has_header_line) {
                data_passer->headings = make_headings(csv_line, field_quoting);
            } else {
                data_passer->headings = make_forced_headings(csv_line);
            }

            g_slist_foreach(data_passer->headings, initialize_field_analysis, data_passer);

            if (has_header_line) {
                continue;
            }
        }

        int column_number = 0;
        gchar *key = NULL;
        gpointer value = NULL;

        while ((token = strsep(&csv_line, "\t")) != NULL) {
            /* Skip a value that is empty. */
            if (g_utf8_strlen(token, -1) == 0) {
                column_number++;
                continue;
            }

            key = strdup((gchar *)g_slist_nth_data(data_passer->headings, column_number));

            Field_analysis *field_analysis = (Field_analysis *)g_hash_table_lookup(data_passer->field_analysis_hash, key);

            if (field_analysis == NULL) {
                g_print("There was a critical failure in looking up the key.\n");
                exit(-1);
            }

            enum data_types field_type = field_analysis->field_type;

            /* Strip whitespace from the current token. */
            gchar *csv_value = g_strstrip(token);

            if (field_quoting != NEVER) {
                strip_quotes(&csv_value);
            }

            gboolean passes_test;
            switch (field_type) {
                case TINYINT_UNSIGNED:
                    passes_test = is_unsigned_int(csv_value, 0, 255);
                    if (passes_test) {
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_unsigned_int(csv_value, 0, 65535);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }

                    passes_test = is_unsigned_int(csv_value, 0, 16777215);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_unsigned_int(csv_value, 0, 4294967295);
                    if (passes_test) {
                        field_analysis->field_type = INT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -128, 127);
                    if (passes_test) {
                        field_analysis->field_type = TINYINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                case SMALLINT_UNSIGNED:
                    passes_test = is_unsigned_int(csv_value, 0, 65535);
                    if (passes_test) break;
                    passes_test = is_unsigned_int(csv_value, 0, 16777215);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_unsigned_int(csv_value, 0, 4294967295);
                    if (passes_test) {
                        field_analysis->field_type = INT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -128, 127);
                    if (passes_test) {
                        field_analysis->field_type = TINYINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                case MEDIUMINT_UNSIGNED:
                    passes_test = is_unsigned_int(csv_value, 0, 16777215);
                    if (passes_test) break;
                    passes_test = is_unsigned_int(csv_value, 0, 4294967295);
                    if (passes_test) {
                        field_analysis->field_type = INT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -128, 127);
                    if (passes_test) {
                        field_analysis->field_type = TINYINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                case INT_UNSIGNED:
                    passes_test = is_unsigned_int(csv_value, 0, 4294967295);
                    if (passes_test) break;
                    passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_UNSIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -128, 127);
                    if (passes_test) {
                        field_analysis->field_type = TINYINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                case BIGINT_UNSIGNED:
                    passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
                    if (passes_test) break;
                    passes_test = is_signed_int(csv_value, -128, 127);
                    if (passes_test) {
                        field_analysis->field_type = TINYINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                case TINYINT_SIGNED:
                    passes_test = is_signed_int(csv_value, -128, 127);
                    if (passes_test) break;
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }

                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }

                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                case SMALLINT_SIGNED:
                    passes_test = is_signed_int(csv_value, -32768, 32767);
                    if (passes_test) break;
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }

                case MEDIUMINT_SIGNED:
                    passes_test = is_signed_int(csv_value, -8388608, 8388607);
                    if (passes_test) break;
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }

                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                case INT_SIGNED:
                    passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
                    if (passes_test) break;
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }

                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }

                case BIGINT_SIGNED:
                    passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
                    if (passes_test) break;
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) {
                        field_analysis->field_type = DECIMAL;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                case DECIMAL:
                    passes_test = is_decimal(csv_value, &decimal_regex);
                    if (passes_test) break;
                    passes_test = is_float(csv_value);
                    if (passes_test) {
                        field_analysis->field_type = FLOAT;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                        break;
                    }
                case FLOAT:
                    passes_test = is_float(csv_value);
                    if (passes_test) break;
                    passes_test = is_timestamp(csv_value, &timestamp_regex);
                    if (passes_test) {
                        field_analysis->field_type = TIMESTAMP;
                        field_analysis->last_line_change = line_number;
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
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
                        g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
                    }
            }
            column_number++;
        }
    }
    data_passer->finished_processing_file = TRUE;
    g_print("Finished the thread\n");
        g_main_loop_quit (data_passer -> gloop);
    g_print("Quit the main loop\n");

}

/**
 * Callback for processing the CSV file. Runs after clicking Go in the main window. The function does the following:
 * -# Checks if the passed file exists.
 * -# Compile regular expressions.
 * -# Create headers, either from the first line of the file or artificially.
 * -# Initilize a `Field_analysis` struct for each column.
 * -# For each line, for each column in the line, determine the minimal MySQL data type. If the data type is larger than the current data type for that column, set it as the new minimum.
 * -# Close the file.
 * -# Print the results.
 * @param button Clicked button.
 * @param data Pointer to the data-passer structure.
 */
gboolean process_file(GtkButton *button, gpointer data) {
    Data_passer *data_passer = (Data_passer *)data;
    FILE *fp = fopen(data_passer->filename, "r");

    if (fp == NULL) {
        //  data_passer->status_bar_context_info_message_id = gtk_statusbar_push(GTK_STATUSBAR(data_passer->status_bar), status_bar_context_info, "Could not open the file.");
        return FALSE;
    }

    data_passer->fp = fp;


    data_passer -> gloop = g_main_loop_new(NULL, FALSE);

    GThread *process_g_thread = g_thread_new("process_thread", (GThreadFunc)process_thread, data);
    g_main_loop_run(data_passer -> gloop);

    g_thread_join(process_g_thread);
    g_main_loop_unref(data_passer -> gloop);
    g_print("After JOIN\n");
    fclose(fp);

    display_results(data_passer);

    return TRUE;
}