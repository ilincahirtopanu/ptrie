#include <ptrie_test.h>
#include <ptrie.h>
#include <sunit.h>

#include <string.h>
#include <stdlib.h>
#include <assert.h>

sunit_ret_t
ptrie_test_eval(struct ptrie_test_action *actions, int print_ops)
{
	struct ptrie *pt = ptrie_allocate();
	int cont = 1;
	int i;

	SUNIT_ASSERT("ptrie allocate", pt);

	for (i = 0; cont; i++) {
		struct ptrie_test_action *act = &actions[i];

		switch(actions[i].type) {
		case PTRIE_TEST_ADD: {
			if (print_ops) printf("ptrie_add(\"%s\")\n", act->word);
			SUNIT_ASSERT("ptrie add", ptrie_add(pt, act->word) == 0);
			break;
		}
		case PTRIE_TEST_AUTOCOMPLETE: {
			char *ret = ptrie_autocomplete(pt, act->word);

			if (print_ops) printf("ptrie_autocomplete(\"%s\") = \"%s\", should be \"%s\"?\n", act->word, ret, act->answer);
			SUNIT_ASSERT("ptrie autocomplete", strcmp(ret, act->answer) == 0);
			free(ret);
			break;
		}
		case PTRIE_TEST_TERM: {
			cont = 0;
			break;
		}}
	}

	ptrie_free(pt);

	return SUNIT_SUCCESS;
}
