#include <gtk/gtk.h>
#ifndef __HEADER
#define __HEADER

/**
 * @file headers.h
 * @brief C headers.
*/

/**
 * Enum for declaring a list store of accounts.
*/
enum data_types {
    TINYINT_UNSIGNED,
    SMALLINT_UNSIGNED,
    MEDIUMINT_UNSIGNED,
    INT_UNSIGNED,
    BIGINT_UNSIGNED,
    TINYINT_SIGNED,
    SMALLINT_SIGNED,
    MEDIUMINT_SIGNED,
    INT_SIGNED,
    BIGINT_SIGNED,
    DECIMAL,
    FLOAT,
    TIMESTAMP,
    CHAR
};

typedef struct Field_analysis {
    enum data_types field_type;
    gboolean sql_signed;
    int char_width;
    int precision;
    int scale;
} field_analysis;

typedef struct decimal_sizes {
  int precision;
  int scale;
} decimal_size;


gboolean omg();

#endif
