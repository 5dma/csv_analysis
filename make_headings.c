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
    printf("Here is the clean_string: %s\n", clean_string);
    printf("Here is the heading: %s\n", heading);

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

    gchar *barf = (gchar *)malloc(1000);
    barf = strdup((gchar *)g_slist_nth_data(local_list, 0));
    printf("Here is first heading in local_list: %s\n", barf);

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
        //    gchar *suffix = g_ascii_dtostr(buffer, 2, i);
        gchar *suffix = g_ascii_formatd(buffer, 3, "%02.2u", i);

        /* heading is long enough to go up to column_99 and \0 */
        gchar *heading = (gchar *)malloc(strlen("column_") + 3);
        heading = strdup("column_");
        g_strlcat(heading, suffix, 10);
        local_list = g_slist_append(local_list, heading);
    }

    return local_list;
}