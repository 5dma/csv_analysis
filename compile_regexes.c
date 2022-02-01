#include <gtk/gtk.h>
#include <regex.h>

regex_t make_decimal_regex() {
    regex_t decimal_regex;
    int value;
    
    value = regcomp(&decimal_regex, "^[-]?[[:digit:]]+[.][[:digit:]]{0,2}$", REG_EXTENDED);
    
    if (value != 0) {
        g_print("Compilation error of regex\n");
        exit(-2);
    }
    return decimal_regex;
}

regex_t make_timestamp_regex() {
    regex_t timestamp_regex;
    int value;
    
    value = regcomp(&timestamp_regex, "^[[:digit:]]{4}-[[:digit:]]{2}-[[:digit:]]{2}T[[:digit:]]{2}:[[:digit:]]{2}:[[:digit:]]{2}[.][[:digit:]]{3}$", REG_EXTENDED);
    
    if (value != 0) {
        g_print("Compilation error of regex\n");
        exit(-2);
    }
    return timestamp_regex;
}