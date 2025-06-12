#include "stringify.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_help() {
	printf(
		"Creates .h and .c files containing a const char* variable\n"
		"with the contents of 'input_file'.\n\n"
	);
	printf("Usage:\n\tstringify [input_file] [--help | -h]\n");
}

bool _is_stringify_exit_called = false;
void check_help_option(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		if (strstr(argv[i], "--help") || strstr(argv[i], "-h")) {
#ifndef TESTING
			print_help();
			exit(0);
#else
			_is_stringify_exit_called = true;
			return;
#endif
		}
		if (strstr(argv[i], "--") || strstr(argv[i], "-")) {
#ifndef TESTING
			printf("Invalid option: %s\n", argv[i]);
			print_help();
			exit(1);
#else
			_is_stringify_exit_called = true;
			return;
#endif
		}
	}
}

void check_argc(int argc) {
	if (argc > 2) {
#ifndef TESTING
		printf("Please provide only one input file.\n");
		print_help();
		exit(1);
#else
		_is_stringify_exit_called = true;
		return;
#endif
	} else if (argc < 2) {
#ifndef TESTING
		printf("Please provide an input file.\n");
		print_help();
		exit(1);
#else
		_is_stringify_exit_called = true;
		return;
#endif
	}
}

str_t *get_file_name(char *argv[]) {
	str_t *file_name = str_create();
	if (!file_name) return NULL;
	if (!str_append(file_name, argv[1])) return NULL;
	return file_name;
}

str_t *get_file_content(const str_t *file_name) {
	if (!file_name) return NULL;
	FILE *file = fopen(str_data(file_name), "r");
	if (!file) {
		printf("Invalid input file: %s\n", str_data(file_name));
		print_help();
		exit(1);
	}

	str_t *file_content = str_create();
	if (!file_content) return NULL;

	char c;
	while ((c = (char)fgetc(file)) != EOF) {
		if (str_push_back(file_content, c)) return NULL;
	}

	fclose(file);
	return file_content;
}

int format_file_content(str_t *file_content) {
	if (!file_content) return 1;

	if (str_push_front(file_content, '\"')) return 1;
	if (!str_replace(file_content, "\n", "\"\n\"")) return 1;
	if (!str_replace(file_content, "\t", "\\t")) return 1;
	if (!str_replace(file_content, "\"", "\\\"")) return 1;
	if (str_push_back(file_content, '\"')) return 1;

	return 0;
}

int format_file_name(str_t *file_name) {
	if (!file_name) return 1;

	if (!str_replace(file_name, ".", "_")) return 1;
	if (!str_replace(file_name, "-", "_")) return 1;

	return 0;
}

str_t *create_header_file_content_content(const str_t *file_name) {
	if (!file_name) return NULL;

	str_t *header_file_content = str_create();
	if (!header_file_content) return NULL;

	if (!str_append(header_file_content, "#ifndef ")) return NULL;
	if (!str_append(header_file_content, str_data(file_name))) return NULL;
	if (!str_append(header_file_content, "_H\n")) return NULL;
	if (!str_append(header_file_content, "#define ")) return NULL;
	if (!str_append(header_file_content, str_data(file_name))) return NULL;
	if (!str_append(header_file_content, "_H\n\n")) return NULL;

	if (!str_append(header_file_content, "extern const char* ")) return NULL;
	if (!str_append(header_file_content, str_data(file_name))) return NULL;
	if (!str_append(header_file_content, ";\n\n")) return NULL;

	if (!str_append(header_file_content, "#endif\n")) return NULL;

	return header_file_content;
}

str_t *create_src_file_content_content(str_t *file_content, const str_t *file_name) {
	if (!file_content || !file_name) return NULL;

	str_t *src_file_content = str_create();
	if (!src_file_content) return NULL;

	if (!str_append(src_file_content, "#include \"")) return NULL;
	if (!str_append(src_file_content, str_data(file_name))) return NULL;
	if (!str_append(src_file_content, ".h\"\n\n")) return NULL;

	if (!str_append(src_file_content, "const char *")) return NULL;
	if (!str_append(src_file_content, str_data(file_name))) return NULL;
	if (!str_append(src_file_content, " = \n")) return NULL;

	if (!str_append(src_file_content, str_data(file_content))) return NULL;
	
	return src_file_content;
}

int write_output_files(
	const str_t *header_file_content,
	const str_t *src_file_content,
	const str_t *file_name
) {
	if (!header_file_content || !src_file_content || !file_name) return 1;

	// header
	str_auto header_file_name = str_create();
	if (!header_file_content) return 1;
	if (!str_append(header_file_name, str_data(file_name))) return 1;
	if (!str_append(header_file_name, ".h")) return 1;

	FILE  *header_file = fopen(str_data(header_file_name), "r");
	if (header_file) {
		printf("File %s already exists.", str_data(header_file_name));
		exit(0);
	} else {
		header_file = fopen(str_data(header_file_name), "w");
		if (!header_file) return 1;
		if (fputs(str_data(header_file_content), header_file) == -1) return 1;
	}
	fclose(header_file);

	// src
	str_auto src_file_name = str_create();
	if (!src_file_content) return 1;
	if (!str_append(src_file_name, str_data(file_name))) return 1;
	if (!str_append(src_file_name, ".h")) return 1;

	FILE  *src_file = fopen(str_data(src_file_name), "r");
	if (src_file) {
		printf("File %s already exists.", str_data(src_file_name));
		exit(0);
	} else {
		src_file = fopen(str_data(src_file_name), "w");
		if (!src_file) return 1;
		if (fputs(str_data(src_file_content), src_file) == -1) return 1;
	}
	fclose(src_file);

	return 0;
}
