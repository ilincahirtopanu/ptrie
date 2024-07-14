#include <ptrie.h>
#include <ptrie_test.h>
#include <sunit.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct ptrie_test_action actions[] = {
	PTRIE_TEST_AUTOCOMPLETE("", ""),
	PTRIE_TEST_ADD("A"),
	PTRIE_TEST_AUTOCOMPLETE("", "A"),
	PTRIE_TEST_ADD("AC"),
	PTRIE_TEST_ADD("AA"),
	PTRIE_TEST_AUTOCOMPLETE("AC", "AC"),
	PTRIE_TEST_ADD("BCB"),
	PTRIE_TEST_ADD("BB"),
	PTRIE_TEST_AUTOCOMPLETE("B", "BB"),
	PTRIE_TEST_AUTOCOMPLETE("BC", "BCB"),
	PTRIE_TEST_ADD("BC"),
	PTRIE_TEST_ADD("BCA"),
	PTRIE_TEST_AUTOCOMPLETE("BC", "BC"),
	/* Milestone 2 starting here */
	PTRIE_TEST_ADD("AC"),
	PTRIE_TEST_ADD("BCA"),
	PTRIE_TEST_AUTOCOMPLETE("A", "AC"),
	PTRIE_TEST_AUTOCOMPLETE("BC", "BCA"),
	PTRIE_TEST_AUTOCOMPLETE("", "AC"),
	PTRIE_TEST_ADD("BCA"),
	PTRIE_TEST_AUTOCOMPLETE("", "BCA"),
	PTRIE_TEST_ADD("A"),
	PTRIE_TEST_AUTOCOMPLETE("A", "A"),
	PTRIE_TEST_ADD("AC"),
	PTRIE_TEST_AUTOCOMPLETE("A", "AC"),
	PTRIE_TEST_ADD("BC"),
	PTRIE_TEST_ADD("BC"),
	PTRIE_TEST_ADD("BC"),
	PTRIE_TEST_AUTOCOMPLETE("", "BC"),
	PTRIE_TEST_AUTOCOMPLETE("B", "BC"),
	PTRIE_TEST_AUTOCOMPLETE("BC", "BC"),
	PTRIE_TEST_AUTOCOMPLETE("BCD", "BCD"),

	PTRIE_TEST_TERM()
};

sunit_ret_t
test_simple(void)
{
	SUNIT_CHECK_FAIL(ptrie_test_eval(actions, 0));

	return SUNIT_SUCCESS;
}

int
main(void)
{
	struct sunit_test tests[] = {
		SUNIT_TEST("add and autocomplete", test_simple),
		SUNIT_TEST_TERM
	};
	sunit_execute("multi-conditions", tests);

	return 0;
}
