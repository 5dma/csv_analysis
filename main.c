#include <gtk/gtk.h>
#include <stdlib.h>
#include <headers.h>

/**
 * @file main.c
 * @brief This small GTK 3.0 application reads a CSV file and generates the corresponding MySQL CREATE TABLE statement.
 *
 * To generate this 
 * code documentation, run `doxygen Doxyfile`. The output is in the `html/` 
 * directory of this source code.
 */


/*! \mainpage CSV Analyzer
 *
 * This application analyzes CSV files and determines the minimal MySQL table definition.
 *
 * \section motivation Motivation
 * Disk space isn't the problem it used to be, but responsiveness is, particularly for web applications. Arguably the most important components of a fast MySQL database are correctly defining the key and index fields, as well as properly constructing queries. Nevertheless, a table with minimal space also helps fast response times on a server. 
 * 
 * @see [Optimizing Schema and Data Types](https://www.oreilly.com/library/view/high-performance-mysql/9781449332471/ch04.html)
 *
 * \section algorithm Algorithm
 * The application opens a CSV file, and counts the number of columns (fields). Each field is assumed to be the smallest data type, `TINYINT`. The application then reads each line in the CSV file. For each line in the file, the application examines each column. If the column is no longer a `TINYINT`, it assigns the smallest data type that represents the current column. As the application reads line after line, and column after column in each line, it preogressively determines the smallest data type for all the values read so far in the column.
 * 
 * \section user_interface User interface
 * The application runs on the GTK runtime. Users specify the following:
 * - CSV file name
 * - If the file includes a header
 * - Delimiter between fields
 * - If the fields are quoted (never, always, optionally)
 * 
 * The output includes the corresponding `CREATE TABLE` and `LOAD DATA` commands.
 */


int main(int argc, char *argv[]) {

	GtkApplication *app = gtk_application_new(
		"net.lautman.csvanalysis",
		G_APPLICATION_FLAGS_NONE);

	g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);

	int status = g_application_run(G_APPLICATION(app), argc, argv);

	/* Decrease reference count because assigning it in on_app_activate */
	g_object_unref(app); 
	return status;
}