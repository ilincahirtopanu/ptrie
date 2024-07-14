#include <ptrie.h>
#include <sunit.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>

static char *test[] = {
	"a",
	"c",
	"ba",
	"bc",
	"bba",
	"bbb",
	"bbc",
	"bbba",
	NULL
};

struct test {
	char *query, *answer;
};

static struct test tests[] = {
	{ .query = "", .answer = "a" },
	{ .query = "b", .answer = "ba" },
	{ .query = "bb", .answer = "bba" },
	{ .query = "bbb", .answer = "bbb" },
	{ .query = NULL, .answer = NULL},
};

sunit_ret_t
test_conditions(void)
{
	int i;
	struct ptrie *pt;
	char *ret;

	pt = ptrie_allocate();
	SUNIT_ASSERT("ptrie alloc", pt);

	ret = ptrie_autocomplete(pt, "a");
	SUNIT_ASSERT("empty ptrie autocomplete", strcmp(ret, "a") == 0);
	free(ret);
	for (i = 0; test[i]; i++) {
		SUNIT_ASSERT("ptrie add", ptrie_add(pt, test[i]) == 0);
	}
	ptrie_free(pt);

	pt = ptrie_allocate();
	SUNIT_ASSERT("ptrie alloc", pt);

	for (i = 0; test[i]; i++) {
		SUNIT_ASSERT("ptrie add", ptrie_add(pt, test[i]) == 0);
	}

	for (i = 0; tests[i].query; i++) {
		char *ret = ptrie_autocomplete(pt, tests[i].query);

		SUNIT_ASSERT("valid prefix", ret && strncmp(tests[i].query, ret, strlen(tests[i].query)) == 0);
		SUNIT_ASSERT("checking answer", ret && strcmp(ret, tests[i].answer) == 0);

		free(ret);
	}
	ptrie_free(pt);

	return SUNIT_SUCCESS;
}

int
main(void)
{
	struct sunit_test tests[] = {
		SUNIT_TEST("conditions", test_conditions),
		SUNIT_TEST_TERM
	};
	sunit_execute("conditions", tests);

	return 0;
}
