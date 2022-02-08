#include <ctype.h>
#include <gtk/gtk.h>

void clean_column_headings(gpointer original_heading_ptr, gpointer data) {
    char *heading = (gchar *)original_heading_ptr;
    char *clean_string;

    clean_string = strdup(heading); /* Memory freed below */
    /* Remove any trailing whitespace, particularly from last item in the list */
    clean_string = g_strchomp(clean_string);
    /* Change characters to lower case */
    clean_string = g_ascii_strdown(clean_string, -1);

    /* Examine each character; if not in set [a-z-_] the replace with underscore */
    char *ptr = clean_string;

    while (*ptr) {
        if (!isalnum(*ptr) &&
            (*ptr != '_') &&
            (*ptr != '-')) {
            *ptr = '_';
        }
        ptr++;
    }

    /* Copy the normalized string into the memory holding the original string. */
    strcpy(heading, clean_string);
    printf("Here is the heading: %s\n", heading);

    g_free(clean_string);
}

GSList *make_headings(char *csv_line) {
    char *token = NULL;
    char *delimiter = "\t";
    GSList *local_list = NULL;
    gchar *heading = NULL;
    while ((token = strsep(&csv_line, delimiter)) != NULL) {
        heading = strdup(token); /* Why is the heading variable necessary? */
        local_list = g_slist_append(local_list, heading);
    }

    g_slist_foreach(local_list, clean_column_headings, NULL);

    /*   gchar *barf = strdup((gchar *)g_slist_nth_data(local_list, 1));
    printf("Here is the heading: %s\n", barf); */
    gchar *omg2 = strdup((gchar *)g_slist_nth_data(local_list, 0));
    printf("omg2: %s\n", omg2);
    return local_list;
}

GSList *make_forced_headings(char *csv_line) {
    char *token;
    char *delimiter = "\t";
    GSList *local_list = NULL;

    int number_columns = 0;
    while (strsep(&csv_line, delimiter) != NULL) {
        number_columns++;
    }

    gchar *prefix = strdup("column_");
    gchar suffix[3];
    for (gdouble i = 0; i < number_columns; i++) {
        g_ascii_formatd(suffix, sizeof(suffix), "%02.0f", i);
        gchar *buffer = g_strconcat(prefix, suffix, NULL);

        local_list = g_slist_append(local_list, buffer);
    }
    g_free(prefix);
    return local_list;
}

GDestroyNotify free_headings(gpointer data) {
    gchar *heading = (gchar *)data;
    g_print("We are deleting %s\n", heading);
    g_free(heading);
}