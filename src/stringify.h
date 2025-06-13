#ifndef STRINGIFY_H
#define STRINGIFY_H

#include <stdbool.h>
#include <c-tools/dynamic-string.h>

void print_help();
extern bool _is_stringify_exit_called;
void check_help_option(int argc, char *argv[]);
void check_argc(int argc);
str_t *get_file_name(char *argv[]);
str_t *get_file_content(const str_t *file_name);
int format_file_content(str_t *file_content);
int format_file_name(str_t *file_name);
str_t *create_header_file_content(const str_t *formatted_file_name);
str_t *create_src_file_content(str_t *file_content, const str_t *file_name);
int write_output_files(
		const str_t *header_file_content,
		const str_t *src_file_content,
		const str_t *file_name
);

#endif
