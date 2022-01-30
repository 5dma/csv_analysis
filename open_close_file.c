#include <gtk/gtk.h>
#include <stdio.h>

#include "headers.h"

gboolean omg(gboolean has_header_line) {
    FILE *fp = fopen("/home/abba/maryland-politics/checkbook_2020/checkbook.csv", "r");
    if (fp == NULL)
        return FALSE;

    char *csv_line;
    size_t max_number_characters = 1000;
    size_t len = 0;

    csv_line = (char *)g_malloc(max_number_characters * sizeof(char));

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

        g_slist_foreach(headings, initialize_field_analysis, &field_analysis_hash);

        g_slist_free_full(headings, g_object_unref);

        Field_analysis *fa = (Field_analysis *)g_hash_table_lookup(field_analysis_hash, "program");
        g_print("OMG\n");
    }
    /* Free memory in the list of headings */
    g_slist_free_full(headings, (GDestroyNotify)free_headings);

    fclose(fp);
    return TRUE;
}