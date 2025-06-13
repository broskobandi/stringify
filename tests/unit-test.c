#include "stringify.h"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <c-tools/dynamic-string.h>

uint completed = 0;
uint failed = 0;
uint passed = 0;

#define ASSERT(expr)\
	if (!(expr)) {\
		printf("***[FAILED]: %s\n", #expr);\
		printf("\tLine: %d\n", __LINE__);\
		failed++;\
	} else {\
		printf("[PASSED]: %s\n", #expr);\
		passed++;\
	}\
	completed++;

void print_results() {
	printf("\nCompleted:\t%d\n", completed);
	if (!failed) {
		printf("All tests passed.\n");
	} else {
		printf("Passed:\t\t%d\n", passed);
		printf("Failed:\t\t%d\n", failed);
	}
}

int test_check_help_option() {
	{
		int argc = 2;
		char *argv[] = {"stringify", "--help"};
		_is_stringify_exit_called = false;
		check_help_option(argc, argv);
		if (_is_stringify_exit_called != true) return 1;
	}
	{
		int argc = 2;
		char *argv[] = {"stringify", "-h"};
		_is_stringify_exit_called = false;
		check_help_option(argc, argv);
		if (_is_stringify_exit_called != true) return 2;
	}
	{
		int argc = 2;
		char *argv[] = {"stringify", "--carrot"};
		_is_stringify_exit_called = false;
		check_help_option(argc, argv);
		if (_is_stringify_exit_called != true) return 3;
	}
	{
		int argc = 4;
		char *argv[] = {"stringify", "something", "--carrot", "something-else"};
		_is_stringify_exit_called = false;
		check_help_option(argc, argv);
		if (_is_stringify_exit_called != true) return 4;
	}
	{
		int argc = 4;
		char *argv[] = {"stringify", "something", "--help", "something-else"};
		_is_stringify_exit_called = false;
		check_help_option(argc, argv);
		if (_is_stringify_exit_called != true) return 5;
	}

	return 0;
}

int test_check_argc() {
	{
		int argc = 2;
		_is_stringify_exit_called = false;
		check_argc(argc);
		if (_is_stringify_exit_called == true) return 1;
	}
	{
		int argc = 1;
		_is_stringify_exit_called = false;
		check_argc(argc);
		if (_is_stringify_exit_called != true) return 2;
	}
	{
		int argc = 3;
		_is_stringify_exit_called = false;
		check_argc(argc);
		if (_is_stringify_exit_called != true) return 3;
	}
	return 0;
}

int test_get_file_name() {
	{
		const char *path = "some_file.txt";
		str_auto file_name = str_create();
		if (!file_name) return 1;
		if (!str_append(file_name, path)) return 2;
		if (!str_cmp(file_name, path)) return 3;
	}
	{
		const char *path = NULL;
		str_auto file_name = str_create();
		if (!file_name) return 1;
		if (str_append(file_name, path)) return 2;
	}

	return 0;
}

int test_get_file_content() {
	FILE *file = fopen("dummy_file.txt", "w");
	if (!file) return 1;
	{
		str_auto file_name = str_create();
		if (!file_name) return 2;
		if (!str_append(file_name, "dummy_file.txt")) return 3;
		str_auto file_content = get_file_content(file_name);
		if (!file_content) return 4;
	}
	{
		str_auto file_name = NULL;
		str_auto file_content = get_file_content(file_name);
		if (file_content) return 5;
	}
	if (remove("dummy_file.txt") != 0) return 6;
	return 0;
}

int test_format_file_content() {
	str_auto file_name = str_create();
	if (!file_name) return 1;
	if (!str_append(file_name, "dummy_file.txt")) return 2;
	FILE *file = fopen(str_data(file_name), "w");
	if (!file) return 3;
	const char *original_content = "this\tis\nsome \"cool\"\n\t\ttext...\n";
	if (fputs(original_content, file) == -1) return 4;
	fclose(file);
	const char *expected_content = "\"this\\tis\\n\"\n\"some \\\"cool\\\"\\n\"\n\"\\t\\ttext...\\n\"\n\"\"";
	str_auto file_content = get_file_content(file_name);
	if (!file_content) return 5;
	if (format_file_content(file_content)) return 6;
	if (!str_cmp(file_content, expected_content)) return 7;
	if (remove("dummy_file.txt") != 0) return 8;
	return 0;
}

int test_format_file_name() {
	str_auto file_name = str_create();
	if (!file_name) return 1;
	if (!str_append(file_name, "dummy-file.txt")) return 2;
	if (format_file_name(file_name)) return 4;
	if (str_cmp(file_name, "dummy_file_txt") != true) return 5;
	return 0;
}

int test_create_header_file_content() {
	str_auto file_name = str_create();
	if (!file_name) return 1;
	if (!str_append(file_name, "dummy-file.txt")) return 2;

	FILE *file = fopen(str_data(file_name), "w");
	if (!file) return 3;
	const char *original_content = "this\tis\nsome \"cool\"\n\t\ttext...\n";
	if (fputs(original_content, file) == -1) return 4;
	if (fclose(file) != 0) return 5;

	str_auto file_content = get_file_content(file_name);
	if (!file_content) return 6;
	if (format_file_content(file_content)) return 7;
	if (format_file_name(file_name)) return 8;

	str_auto header_file_content = create_header_file_content(file_name);

	const char *expected_header_file_content = 
		"#ifndef dummy_file_txt_H\n"
		"#define dummy_file_txt_H\n\n"
		"extern const char *dummy_file_txt_str;\n\n"
		"#endif\n";

	if (str_cmp(header_file_content, expected_header_file_content) != true) return 9;

	if (remove("dummy-file.txt") != 0) return 10; // this seems to fail. why?

	return 0;
}

int main(void) {
	ASSERT(test_check_help_option() == 0);
	ASSERT(test_check_argc() == 0);
	ASSERT(test_get_file_name() == 0);
	ASSERT(test_get_file_content() == 0);
	ASSERT(test_format_file_content() == 0);
	ASSERT(test_format_file_name() == 0);
	ASSERT(test_create_header_file_content() == 0);

	print_results();
	return 0;
}
