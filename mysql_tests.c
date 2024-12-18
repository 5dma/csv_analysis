#include <gtk/gtk.h>
#include <math.h>
#include <headers.h>
/**
 * @file mysql_tests.c
 * @brief Statements for determining the MySQL data type of a character string.
 */

/**
 * Determines the MySQL data type of a passed value read from the CSV file. The passed value is a `gchar *`, and the objective is to determine the smallest data type that value represents. For example, a value `12` corresponds to the MySQL `TINYINT_UNSIGNED`.
 * 
 * The field tests go from smallest to largest. For example, the first test checks if the value is a `TINYINT_UNSIGNED`. If not that, the next check is for `SMALLINT_UNSIGNED`, and so on. If all tests fail, the value is assumed to be `CHAR(N)`, where `N` is the value's length.
 * 
 * The field tests are cumulative such that a field can never return to a smaller data type. For example, once a field is determined to be `BIGINT_SIGNED`, it can never go back to `TINYINT_UNSIGNED` even if all subsequent values are indeed `TINYINT_UNSIGNED`.
 * 
 * @see [MySQL data types](https://www.mysqldatatypes.com/)
 * 
 * @param csv_value Value in the CSV file whose MySQL type we want to determine.
 * @param field_analysis The column's current Field_analysis.
 * @param data_passer Pointer to the data-passer structure.
 *  */
void do_mysql_tests(const gchar *csv_value, Field_analysis *field_analysis, Data_passer *data_passer) {
	enum data_types_mysql original_field_type = field_analysis->field_type;
	gboolean passes_test = FALSE;
	switch (original_field_type) {
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
			break;
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
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
			break;
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
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
			break;
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
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
			break;
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
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
			break;
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
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
			break;
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
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
			break;
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
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
			break;
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
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
			break;
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
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
			break;
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
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
			break;
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
		case FLOAT:
			passes_test = is_float(csv_value);
			if (passes_test) break;
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) {
				field_analysis->field_type = TIMESTAMP;
				break;
			}
			break;
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
		case TIMESTAMP:
			passes_test = is_timestamp(csv_value, &(data_passer->timestamp_regex));
			if (passes_test) break;
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
		default:
			assign_char_field_type(csv_value, data_passer->current_line_number, field_analysis);
	}

	gboolean status_changed = (original_field_type != field_analysis->field_type);
	if (passes_test && status_changed) {
		field_analysis->last_line_change = data_passer->current_line_number;
		g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
	};
}