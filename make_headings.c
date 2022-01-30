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
    char *token;
    char *delimiter = "\t";
    GSList *local_list = NULL;

    gchar *heading;
    while ((token = strsep(&csv_line, delimiter)) != NULL) {
        heading = strdup(token);
        local_list = g_slist_append(local_list, heading);
    }
    g_free(heading);
    g_slist_foreach(local_list, clean_column_headings, NULL);

    gchar *barf = strdup((gchar *)g_slist_nth_data(local_list, 1));
    printf("Here is the heading: %s\n", barf);

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

    gchar buffer[3];
    for (int i = 0; i < number_columns; i++) {
        gchar *suffix = g_ascii_formatd(buffer, 3, "%02.2u", i);

        /* heading is long enough to go up to column_99 and \0 */
        gchar *heading = (gchar *)malloc(strlen("column_") + 3);
        heading = strdup("column_");
        g_strlcat(heading, suffix, 10);
        local_list = g_slist_append(local_list, heading);
        g_free(suffix);
    }

    return local_list;
}

GDestroyNotify free_headings(gpointer data) {
    gchar *heading = (gchar *)data;
    g_print("We are deleting %s\n", heading);
    g_free(heading);
}