#include <glib-2.0/glib.h>
#include <gtk/gtk.h>
#include <regex.h>

gboolean is_unsigned_int(const gchar *token, gshort min, guint64 max) {
    GError *error = NULL;
    guint64 num = 0;
    gboolean success = g_ascii_string_to_unsigned(token, 10, min, max, &num, &error);
    if (success) {
        return TRUE;
    }
    return FALSE;
}

gboolean is_signed_int(const gchar *token, gint64 min, gint64 max) {
    GError *error = NULL;
    guint64 num = 0;
    gboolean success = g_ascii_string_to_signed(token, 10, min, max, &num, &error);
    if (success) {
        return TRUE;
    }
    return FALSE;
}

gboolean is_decimal(const gchar *token, regex_t *decimal_regex) {
    regmatch_t pmatch[2];

    int success = regexec(decimal_regex, token, 1, pmatch, 0);
    return (success == 0) ? TRUE : FALSE;
}

gboolean is_float(const gchar *token) {

    gchar *end_ptr;
    size_t token_length = strlen(token);
    gdouble result = g_ascii_strtod(token, &end_ptr);

    if (((result == 0) && 
    (g_ascii_strcasecmp(token, "0.00") != 0)) || 
    ((token + token_length) != end_ptr)) {
        return FALSE;
    } else {
        return TRUE;
    }
}


gboolean is_timestamp(const gchar *token, regex_t *timestamp_regex) {
    regmatch_t pmatch[2];

    int success = regexec(timestamp_regex, token, 1, pmatch, 0);
    return (success == 0) ? TRUE : FALSE;
}

