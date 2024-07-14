#pragma once

#include <sunit.h>

struct ptrie_test_action {
	enum { PTRIE_TEST_ADD, PTRIE_TEST_AUTOCOMPLETE, PTRIE_TEST_TERM } type;
	char *word;
	char *answer;
};

#define PTRIE_TEST_ADD(str)                { .type = PTRIE_TEST_ADD, .word = str }
#define PTRIE_TEST_AUTOCOMPLETE(str, soln) { .type = PTRIE_TEST_AUTOCOMPLETE, .word = str, .answer = soln }
#define PTRIE_TEST_TERM()                  { .type = PTRIE_TEST_TERM }

sunit_ret_t ptrie_test_eval(struct ptrie_test_action *actions, int print_ops);
