#include <gtk/gtk.h>
#include <glib-2.0/glib.h>


gboolean field_unsigned_int(const gchar *token, gint min, gint max) {

    GError *error = NULL;
    guint64 num = 0;
    gboolean success = g_ascii_string_to_unsigned(token, 10, min, max, &num, &error);
    if (success) {
        return TRUE;
    }
    return FALSE;
}