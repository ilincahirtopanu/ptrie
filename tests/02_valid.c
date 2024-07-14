#include <ptrie.h>
#include <sunit.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char *test[] = {
	"a",
	"test",
	"testimonial",
	"testy",
	"testify",
	"and",
	"andanother",
	"terminal",
	NULL
};

struct test {
	char *query, *answer;
};

static struct test tests[] = {
	{ .query = "testi" },
	{ .query = "tes" },
	{ .query = "a" },
	{ .query = NULL, .answer = NULL},
};

sunit_ret_t
test_add_complete(void)
{
	int i;
	struct ptrie *pt;

	pt = ptrie_allocate();
	SUNIT_ASSERT("allocating ptrie", pt);

	for (i = 0; test[i]; i++) {
		SUNIT_ASSERT("ptrie add", ptrie_add(pt, test[i]) == 0);
	}

	for (i = 0; tests[i].query; i++) {
		char *ret = ptrie_autocomplete(pt, tests[i].query);
		int j;
		int found = 0;

		/* Just test the values returned were previously inserted, and match the prefix. */
		for (j = 0; test[j]; j++) {
			if (strcmp(ret, test[j]) == 0) {
				found = 1;
				break;
			}
		}
		SUNIT_ASSERT("testing previously added string", found == 1);
		SUNIT_ASSERT("testing valid prefix", ret && strncmp(tests[i].query, ret, strlen(tests[i].query)) == 0);

		free(ret);
	}
	ptrie_free(pt);

	return SUNIT_SUCCESS;
}

int
main(void)
{
	struct sunit_test tests[] = {
		SUNIT_TEST("add and completion", test_add_complete),
		SUNIT_TEST_TERM
	};
	sunit_execute("valid prefixes", tests);

	return 0;
}
