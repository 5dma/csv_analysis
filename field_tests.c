#include <gtk/gtk.h>
#include <glib-2.0/glib.h>


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

gboolean is_decimal(const gchar *token, gint64 min, gint64 max) {

    GError *error = NULL;
    guint64 num = 0;
    gboolean success = g_ascii_string_to_signed(token, 10, min, max, &num, &error);
    if (success) {
        return TRUE;
    }
    return FALSE;
}


