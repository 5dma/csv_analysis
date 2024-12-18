#include <gtk/gtk.h>
#include <math.h>
#include <regex.h>
#include <stdio.h>
#define G_LOG_USE_STRUCTURED
#include <glib-2.0/glib.h>
#include <headers.h>
/**
 * @file my_sql_tests.c
 * @brief Statements for determining the MySQL data type of a character string.
 */
void do_mysql_tests(const gchar *csv_value, Field_analysis *field_analysis, Data_passer *data_passer) {
	enum data_types field_type = field_analysis->field_type;
	gboolean passes_test = FALSE;
	switch (field_type) {
		case GARBAGE: /* GARBAGE is a field's initial data type, basically unitialized. */
			passes_test = is_unsigned_int(csv_value, 0, 255);
			if (passes_test) {
				field_analysis->field_type = TINYINT_UNSIGNED;
				break;
			}
			passes_test = is_unsigned_int(csv_value, 0, 65535);
			if (passes_test) {
				field_analysis->field_type = SMALLINT_UNSIGNED;
				break;
			}
			passes_test = is_unsigned_int(csv_value, 0, 16777215);
			if (passes_test) {
				field_analysis->field_type = MEDIUMINT_UNSIGNED;
				break;
			}
			passes_test = is_unsigned_int(csv_value, 0, 4294967295);
			if (passes_test) {
				field_analysis->field_type = INT_UNSIGNED;
				break;
			}
			passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
			if (passes_test) {
				field_analysis->field_type = BIGINT_UNSIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -128, 127);
			if (passes_test) {
				field_analysis->field_type = TINYINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -32768, 32767);
			if (passes_test) {
				field_analysis->field_type = SMALLINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -8388608, 8388607);
			if (passes_test) {
				field_analysis->field_type = MEDIUMINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
			if (passes_test) {
				field_analysis->field_type = INT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) {
				field_analysis->field_type = BIGINT_SIGNED;
				break;
			}
			passes_test = is_decimal(csv_value, &(data_passer->decimal_regex));
			if (passes_test) {
				field_analysis->field_type = DECIMAL;
				break;
			}
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = FLOAT;
				break;
			}
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
			break;
		case TINYINT_UNSIGNED:
			passes_test = is_unsigned_int(csv_value, 0, 255);
			if (passes_test) break;
			passes_test = is_unsigned_int(csv_value, 0, 65535);
			if (passes_test) {
				field_analysis->field_type = SMALLINT_UNSIGNED;
				break;
			}
			passes_test = is_unsigned_int(csv_value, 0, 16777215);
			if (passes_test) {
				field_analysis->field_type = MEDIUMINT_UNSIGNED;
				break;
			}
			passes_test = is_unsigned_int(csv_value, 0, 4294967295);
			if (passes_test) {
				field_analysis->field_type = INT_UNSIGNED;
				break;
			}
			passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
			if (passes_test) {
				field_analysis->field_type = BIGINT_UNSIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -128, 127);
			if (passes_test) {
				field_analysis->field_type = TINYINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -32768, 32767);
			if (passes_test) {
				field_analysis->field_type = SMALLINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -8388608, 8388607);
			if (passes_test) {
				field_analysis->field_type = MEDIUMINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
			if (passes_test) {
				field_analysis->field_type = INT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) {
				field_analysis->field_type = BIGINT_SIGNED;
				break;
			}
			passes_test = is_decimal(csv_value, &(data_passer->decimal_regex));
			if (passes_test) {
				field_analysis->field_type = DECIMAL;
				break;
			}
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = FLOAT;
				break;
			}
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
			break;
		case SMALLINT_UNSIGNED:
			passes_test = is_unsigned_int(csv_value, 0, 65535);
			if (passes_test) break;
			passes_test = is_unsigned_int(csv_value, 0, 16777215);
			if (passes_test) {
				field_analysis->field_type = MEDIUMINT_UNSIGNED;
				break;
			}
			passes_test = is_unsigned_int(csv_value, 0, 4294967295);
			if (passes_test) {
				field_analysis->field_type = INT_UNSIGNED;
				break;
			}
			passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
			if (passes_test) {
				field_analysis->field_type = BIGINT_UNSIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -128, 127);
			if (passes_test) {
				field_analysis->field_type = TINYINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -32768, 32767);
			if (passes_test) {
				field_analysis->field_type = SMALLINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -8388608, 8388607);
			if (passes_test) {
				field_analysis->field_type = MEDIUMINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
			if (passes_test) {
				field_analysis->field_type = INT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) {
				field_analysis->field_type = BIGINT_SIGNED;
				break;
			}
			passes_test = is_decimal(csv_value, &(data_passer->decimal_regex));
			if (passes_test) {
				field_analysis->field_type = DECIMAL;
				break;
			}
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = FLOAT;
				break;
			}
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
			break;
		case MEDIUMINT_UNSIGNED:
			passes_test = is_unsigned_int(csv_value, 0, 16777215);
			if (passes_test) break;
			passes_test = is_unsigned_int(csv_value, 0, 4294967295);
			if (passes_test) {
				field_analysis->field_type = INT_UNSIGNED;
				break;
			}
			passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
			if (passes_test) {
				field_analysis->field_type = BIGINT_UNSIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -128, 127);
			if (passes_test) {
				field_analysis->field_type = TINYINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -32768, 32767);
			if (passes_test) {
				field_analysis->field_type = SMALLINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -8388608, 8388607);
			if (passes_test) {
				field_analysis->field_type = MEDIUMINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
			if (passes_test) {
				field_analysis->field_type = INT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) {
				field_analysis->field_type = BIGINT_SIGNED;
				break;
			}
			passes_test = is_decimal(csv_value, &(data_passer->decimal_regex));
			if (passes_test) {
				field_analysis->field_type = DECIMAL;
				break;
			}
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = FLOAT;
				break;
			}
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
			break;
		case INT_UNSIGNED:
			passes_test = is_unsigned_int(csv_value, 0, 4294967295);
			if (passes_test) break;
			passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
			if (passes_test) {
				field_analysis->field_type = BIGINT_UNSIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -128, 127);
			if (passes_test) {
				field_analysis->field_type = TINYINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -32768, 32767);
			if (passes_test) {
				field_analysis->field_type = SMALLINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -8388608, 8388607);
			if (passes_test) {
				field_analysis->field_type = MEDIUMINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
			if (passes_test) {
				field_analysis->field_type = INT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) {
				field_analysis->field_type = BIGINT_SIGNED;
				break;
			}
			passes_test = is_decimal(csv_value, &(data_passer->decimal_regex));
			if (passes_test) {
				field_analysis->field_type = DECIMAL;
				break;
			}
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = FLOAT;
				break;
			}
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
			break;
		case BIGINT_UNSIGNED:
			passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
			if (passes_test) break;
			passes_test = is_signed_int(csv_value, -128, 127);
			if (passes_test) {
				field_analysis->field_type = TINYINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -32768, 32767);
			if (passes_test) {
				field_analysis->field_type = SMALLINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -8388608, 8388607);
			if (passes_test) {
				field_analysis->field_type = MEDIUMINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
			if (passes_test) {
				field_analysis->field_type = INT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) {
				field_analysis->field_type = BIGINT_SIGNED;
				break;
			}
			passes_test = is_decimal(csv_value, &(data_passer->decimal_regex));
			if (passes_test) {
				field_analysis->field_type = DECIMAL;
				break;
			}
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = FLOAT;
				break;
			}
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
			break;
		case TINYINT_SIGNED:
			passes_test = is_signed_int(csv_value, -128, 127);
			if (passes_test) break;
			passes_test = is_signed_int(csv_value, -32768, 32767);
			if (passes_test) {
				field_analysis->field_type = SMALLINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -8388608, 8388607);
			if (passes_test) {
				field_analysis->field_type = MEDIUMINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
			if (passes_test) {
				field_analysis->field_type = INT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) {
				field_analysis->field_type = BIGINT_SIGNED;
				break;
			}
			passes_test = is_decimal(csv_value, &(data_passer->decimal_regex));
			if (passes_test) {
				field_analysis->field_type = DECIMAL;
				break;
			}
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = FLOAT;
				break;
			}
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
			break;
		case SMALLINT_SIGNED:
			passes_test = is_signed_int(csv_value, -32768, 32767);
			if (passes_test) break;
			passes_test = is_signed_int(csv_value, -8388608, 8388607);
			if (passes_test) {
				field_analysis->field_type = MEDIUMINT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
			if (passes_test) {
				field_analysis->field_type = INT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) {
				field_analysis->field_type = BIGINT_SIGNED;
				break;
			}
			passes_test = is_decimal(csv_value, &(data_passer->decimal_regex));
			if (passes_test) {
				field_analysis->field_type = DECIMAL;
				break;
			}
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = FLOAT;
				break;
			}
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
			break;
		case MEDIUMINT_SIGNED:
			passes_test = is_signed_int(csv_value, -8388608, 8388607);
			if (passes_test) break;
			passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
			if (passes_test) {
				field_analysis->field_type = INT_SIGNED;
				break;
			}
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) {
				field_analysis->field_type = BIGINT_SIGNED;
				break;
			}
			passes_test = is_decimal(csv_value, &(data_passer->decimal_regex));
			if (passes_test) {
				field_analysis->field_type = DECIMAL;
				break;
			}
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = FLOAT;
				break;
			}
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
			break;
		case INT_SIGNED:
			passes_test = is_signed_int(csv_value, -2147483648, 2147483647);
			if (passes_test) break;
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) {
				field_analysis->field_type = BIGINT_SIGNED;
				break;
			}
			passes_test = is_decimal(csv_value, &(data_passer->decimal_regex));
			if (passes_test) {
				field_analysis->field_type = DECIMAL;
				break;
			}
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = FLOAT;
				break;
			}
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
			break;
		case BIGINT_SIGNED:
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) break;
			passes_test = is_decimal(csv_value, &(data_passer->decimal_regex));
			if (passes_test) {
				field_analysis->field_type = DECIMAL;
				break;
			}
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = FLOAT;
				break;
			}
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
			break;
		case DECIMAL:
			passes_test = is_decimal(csv_value, &(data_passer->decimal_regex));
			if (passes_test) break;
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = FLOAT;
				break;
			}
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
			break;
		case FLOAT:
			passes_test = is_float(csv_value);
			if (passes_test) break;
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
			break;
		case TIMESTAMP:
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) break;
		default:
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
	}
	if (!passes_test) {
		assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
	}
}