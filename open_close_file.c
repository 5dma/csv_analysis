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

    GSList *headings;
    gboolean on_first_line = TRUE;

    char *token;
    char *delimiter = "\t";

    while (getline(&csv_line, &len, fp) != -1) {
        printf("line length: %zd\n", strlen(csv_line));

        if (on_first_line) {
            if (has_header_line) {
                headings = make_headings(csv_line);
            } else {
                headings = make_forced_headings(csv_line);
            }
            gchar *barf = (gchar *)malloc(1000);
            barf = strdup((gchar *)g_slist_nth(headings, 1));
            printf("Here is the heading: %s\n", barf);

            on_first_line = FALSE;
            continue;
        }

        GHashTable *field_analysis_hash = NULL;

        g_slist_foreach(headings, initialize_field_analysis, field_analysis_hash);

        for (int j = 1;; j++) {
            token = strsep(&csv_line, delimiter);
            if (token == NULL) break;
            printf("%d: '%s'\n", j, token);
        }
    }

    fclose(fp);
    return TRUE;
}