CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LFLAGS = `pkg-config --libs gtk+-3.0`
OBJFILES = app_activate.o cleanup.o compile_regexes.o control.o copy.o display_results.o field_tests.o initialize_field_analysis.o main.o make_headings.o open_close_file.o parse_csv.o view.o 

all: csv_analysis

csv_analysis: $(OBJFILES)
	$(CC) -o $@ $^ $(LFLAGS)
 
app_activate.o: app_activate.c
	$(CC) -c $< $(CFLAGS)

cleanup.o: cleanup.c
	$(CC) -c $< $(CFLAGS)

compile_regexes.o: compile_regexes.c
	$(CC) -c $< $(CFLAGS)

control.o: control.c
	$(CC) -c $< $(CFLAGS)

copy.o: copy.c
	$(CC) -c $< $(CFLAGS)

display_results.o: display_results.c
	$(CC) -c $< $(CFLAGS)

field_tests.o: field_tests.c
	$(CC) -c $< $(CFLAGS)

initialize_field_analysis.o: initialize_field_analysis.c
	$(CC) -c $< $(CFLAGS)

main.o: main.c
	$(CC) -c $< $(CFLAGS)

make_headings.o: make_headings.c
	$(CC) -c $< $(CFLAGS)

open_close_file.o: open_close_file.c
	$(CC) -c $< $(CFLAGS)

parse_csv.o: parse_csv.c
	$(CC) -c $< $(CFLAGS)

view.o: view.c
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJFILES)

