#include <gtk/gtk.h>
#include <regex.h>

regex_t make_decimal_regex() {
    regex_t decimal_regex;
    int value;
    
    value = regcomp(&decimal_regex, "[-]?[[:digit:]]+[.][[:digit:]]{0,2}", REG_EXTENDED);
    
    if (value != 0) {
        g_print("Compilation error of regex\n");
        exit(-2);
    }
    return decimal_regex;
}