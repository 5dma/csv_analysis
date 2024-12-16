#include <gtk/gtk.h>
#include <regex.h>
/**
 * @file compile_regexes.c
 * @brief Compiles regular expressions.
*/

/**
 * Compiles a regular expression for identifying a decimal number.
 * The string has an optional decimal point; if it has a decimal point, it is followed by an optional 0&ndash;2 digits. 
 * It appears that the C library `regex.h` requires [POSIX character classes](https://riptutorial.com/regex/example/17891/posix-character-classes). 
 * @returns A regular expression.
 * @see is_decimal()
*/ 
regex_t make_decimal_regex() {
	regex_t decimal_regex;
	int value;
	
	value = regcomp(&decimal_regex, "^[+-]?[[:digit:]]+\\.?[[:digit:]]{0,2}$", REG_EXTENDED);

	if (value != 0) {
		g_print("Compilation error of regex\n");
		exit(-2);
	}
	return decimal_regex;
}

/**
 * Compiles a regular expression for identifying a timestamp. The pattern this regex is
 * 
 * * YYYY-MM-DDTHH:MM:SS:MMM
 *
 * It appears that the C library `regex.h` requires [POSIX character classes](https://riptutorial.com/regex/example/17891/posix-character-classes).
 * @returns A regular expression.
 * @see is_timestamp()
*/ 
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