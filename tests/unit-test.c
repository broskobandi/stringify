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

// int test_check_help_option() {
// 	int argc = 2;
// 	char *argv[] = {"stringify", "--help"};
// 	_is_stringify_exit_called = false;
//
// 	return 0;
// }

int main(void) {

	print_results();
	return 0;
}
