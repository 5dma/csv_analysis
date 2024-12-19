#include <gtk/gtk.h>
#include <math.h>
#include <headers.h>
/**
 * @file sqlite_tests.c
 * @brief Statements for determining the SQLITE data type of a character string.
 */

/**
 * Determines the SQLite data type of a passed value read from the CSV file. The passed value is a `gchar *`, and the objective is to determine the smallest data type that value represents. For example, a value `12` corresponds to the SQLITE `INTEGER`.
 * 
 * The field tests go from smallest to largest. For example, the first test checks if the value is a `TEXT`. If not that, the next check is for `SMALLINT_UNSIGNED`, and so on. If all tests fail, the value is assumed to be `CHAR(N)`, where `N` is the value's length.
 * 
 * The field tests are cumulative such that a field can never return to a smaller data type. For example, once a field is determined to be `BIGINT_SIGNED`, it can never go back to `TINYINT_UNSIGNED` even if all subsequent values are indeed `TINYINT_UNSIGNED`.
 * 
 * @see [Datatypes In SQLite](https://sqlite.org/datatype3.html)
 * 
 * @param csv_value Value in the CSV file whose SQLite type we want to determine.
 * @param field_analysis The column's current Field_analysis_sqlite.
 * @param data_passer Pointer to the data-passer structure.
 *  */
void do_sqlite_tests(const gchar *csv_value, Field_analysis_sqlite *field_analysis, Data_passer *data_passer) {
	enum data_types_sqlite original_field_type = field_analysis->field_type;
	gboolean passes_test = FALSE;
	switch (original_field_type) {
		case TRASH: /* TRASH is a field's initial data type, basically unitialized. */
			passes_test = is_null(csv_value);
			if (passes_test) {
				field_analysis->field_type = NULL_S;
				break;
			}
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) {
				field_analysis->field_type = INTEGER;
				break;
			}
			passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
			if (passes_test) {
				field_analysis->field_type = INTEGER;
				break;
			}
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = REAL;
				break;
			}
			field_analysis->field_type = TEXT;
			break;
		case NULL_S:
			passes_test = is_null(csv_value);
			if (passes_test) break;
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) {
				field_analysis->field_type = INTEGER;
				break;
			}
			passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
			if (passes_test) {
				field_analysis->field_type = INTEGER;
				break;
			}
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = REAL;
				break;
			}
			field_analysis->field_type = TEXT;
			break;
		case INTEGER:
			passes_test = is_signed_int(csv_value, -((gint64)pow(2, 63) - 1), (gint64)(pow(2, 63) - 1));
			if (passes_test) break;
			passes_test = is_unsigned_int(csv_value, 0, (guint64)pow(2, 64) - 1);
			if (passes_test) break;
			passes_test = is_float(csv_value);
			if (passes_test) {
				field_analysis->field_type = REAL;
				break;
			}
			field_analysis->field_type = TEXT;
			break;
		case REAL:
			passes_test = is_float(csv_value);
			if (passes_test) break;
			field_analysis->field_type = TEXT;
		default:
			field_analysis->field_type = TEXT;
	}

	gboolean status_changed = (original_field_type != field_analysis->field_type);
	if (passes_test && status_changed) {
		field_analysis->last_line_change = data_passer->current_line_number;
		g_strlcpy(field_analysis->determining_value, csv_value, g_utf8_strlen(csv_value, 500) + 1);
	};
}