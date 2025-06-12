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

int main(void) {
	ASSERT(test_check_help_option() == 0);
	ASSERT(test_check_argc() == 0);
	ASSERT(test_get_file_name() == 0);

	print_results();
	return 0;
}
