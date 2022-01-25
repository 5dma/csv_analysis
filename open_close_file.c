#include <gtk/gtk.h>
#include <stdio.h>

gboolean omg() {
    FILE *fp = fopen("/home/abba/maryland-politics/checkbook_2020/checkbook.csv", "r");
    if (fp == NULL)
        return FALSE;

    char *csv_line = NULL;
    size_t len = 0;

    while (getline(&csv_line, &len, fp) != -1) {
        printf("line length: %zd\n", strlen(csv_line));
        char *token;
        char *delimiter = "\t";

        for (int j = 1;; j++) {
            token = strsep(&csv_line, delimiter);
            if (token == NULL) break;
            printf("%d: '%s'\n", j, token);
        }
    }
   
    fclose(fp);
    return TRUE;
}