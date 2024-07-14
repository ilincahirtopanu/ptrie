#include <ptrie.h>
#include <sunit.h>

#include <stdio.h>
#include <string.h>

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

sunit_ret_t
test_alloc(void)
{
	struct ptrie *pt;

	pt = ptrie_allocate();
	SUNIT_ASSERT("allocating prefix trie", pt != NULL);
	ptrie_free(pt);

	return SUNIT_SUCCESS;
}

sunit_ret_t
test_add(void)
{
	struct ptrie *pt;
	int i;

	pt = ptrie_allocate();
	SUNIT_ASSERT("allocating ptrie", pt != NULL);

	for (i = 0; test[i]; i++) {
		SUNIT_ASSERT("ptrie add", ptrie_add(pt, test[i]) == 0);
	}
	ptrie_free(pt);

	return SUNIT_SUCCESS;
}

int
main(void)
{
	struct sunit_test tests[] = {
		SUNIT_TEST("allocation test", test_alloc),
		SUNIT_TEST("addition test", test_add),
		SUNIT_TEST_TERM
	};
	sunit_execute("addition", tests);

	return 0;
}
