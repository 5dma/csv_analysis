#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>

#include "headers.h"

gboolean omg(gboolean has_header_line) {
    FILE *fp = fopen("/home/abba/maryland-politics/checkbook_2020/checkbook.csv", "r");
    if (fp == NULL)
        return FALSE;

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

    GHashTable *field_analysis_hash = g_hash_table_new(g_int_hash, g_int_equal);

    while (getline(&csv_line, &len, fp) != -1) {
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
        while ((token = strsep(&csv_line, delimiter)) != NULL) {
            gchar *key = strdup((gchar *)g_slist_nth_data(headings, i));
            g_print("The key is %s\n", key);
            gpointer value = g_hash_table_lookup(field_analysis_hash, key);
            if (value == NULL) {
                g_print("There was a critical failure in looking up the key.\n");
                exit(-1);
            }
            Field_analysis *field_analysis = (Field_analysis *)value;
            enum data_types field_type = field_analysis->field_type;

            gboolean passes_test;
            switch (field_type) {
                case TINYINT_UNSIGNED:
                    passes_test = field_unsigned_int(token, 0, 255);
                    if (passes_test) break;
                    passes_test = field_unsigned_int(token, 0, 65535);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_UNSIGNED;
                        break;
                    }
                    passes_test = field_unsigned_int(token, 0, 16777215);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_UNSIGNED;
                        break;
                    }
                    passes_test = field_unsigned_int(token, 0, 4294967295);
                    if (passes_test) {
                        field_analysis->field_type = INT_UNSIGNED;
                        break;
                    }
                    passes_test = field_unsigned_int(token, 0, (unsigned long long )pow(2,64) - 1);
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_UNSIGNED;
                        break;
                    }
                    passes_test = field_signed_int(token, -128, 127);
                    if (passes_test) {
                        field_analysis->field_type = TINYINT_SIGNED;
                        break;
                    }
                    passes_test = field_signed_int(token, -32768, 32767);
                    if (passes_test) {
                        field_analysis->field_type = SMALLINT_SIGNED;
                        break;
                    }
                    passes_test = field_signed_int(token, -8388608, 8388607);
                    if (passes_test) {
                        field_analysis->field_type = MEDIUMINT_SIGNED;
                        break;
                    }
                    passes_test = field_signed_int(token, -2147483648, 2147483647);
                    if (passes_test) {
                        field_analysis->field_type = INT_SIGNED;
                        break;
                    }
                   passes_test = field_signed_int(token, -((unsigned long long )pow(2,63) - 1), (unsigned long long )(pow(2,63) - 1));
                    if (passes_test) {
                        field_analysis->field_type = BIGINT_SIGNED;
                        break;
                    }

                default:
                    g_print("Do nothing\n");
            }
            i++;
            g_print("Barf\n");
        }
        /*
        gchar *omg2 = strdup((gchar *)g_slist_nth_data(headings, 1));
        printf("omg2: %s\n", omg2);
        Field_analysis *fa = (Field_analysis *)g_hash_table_lookup(field_analysis_hash, "program");*/
        g_print("OMG\n");
    }
    /* Free memory in the list of headings */
    g_slist_free_full(headings, (GDestroyNotify)free_headings);

    fclose(fp);
    return TRUE;
}