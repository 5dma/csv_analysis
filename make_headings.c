#include <ctype.h>
#include <gtk/gtk.h>

void clean_column_headings(gpointer original_heading, gpointer data) {
    char *heading = (char *)original_heading;
    char *clean_string;
    clean_string = (char *)g_malloc(strlen(heading) + 1);
    clean_string = strdup(heading);
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

    heading = strdup(clean_string);
    g_free(clean_string);
}

GSList *make_headings(char *csv_line) {
    char *token;
    char *delimiter = "\t";
    GSList *local_list = NULL;

    while ((token = strsep(&csv_line, delimiter)) != NULL) {
        local_list = g_slist_append(local_list, token);
    }
    g_slist_foreach(local_list, clean_column_headings, NULL);

    return local_list;
}
