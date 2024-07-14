TEST_FILES = $(wildcard tests/*.c)
TEST_DEPS  = $(patsubst %.c,%.d,$(TEST_FILES))
TEST_BIN   = $(sort $(patsubst %.c,%.test,$(TEST_FILES)))
TEST_OBJS  = $(sort $(patsubst %.c,%.o,$(TEST_FILES)))
LIBDIR     = .
UTIL       = util
INCDIRS    = tests $(LIBDIR) $(UTIL)

CC       = gcc
# generate files that encode make rules for the .h dependencies
DEPFLAGS = -MP -MD
# automatically add the -I onto each include directory
CFLAGS   = -Wall -Wextra -Werror -Wno-unused-function $(foreach D,$(INCDIRS),-I$(D)) -O3 $(DEPFLAGS)

# for-style iteration (foreach) and regular expression completions (wildcard)
CFILES   = $(wildcard *.c)
# regular expression replacement
OBJECTS  = $(patsubst %.c,%.o,$(CFILES))
DEPFILES = $(patsubst %.c,%.d,$(CFILES))

LIBFILES = $(wildcard $(LIBDIR)/*.c)
LIBOBJS  = $(patsubst %.c,%.o,$(LIBFILES))
LIBDEPS  = $(patsubst %.c,%.d,$(LIBFILES))
LIBNAME  = ptrie
LIB      = $(LIBDIR)/lib$(LIBNAME).a

LD       = gcc
LDFLAGS  = -L$(LIBDIR) -l$(LIBNAME)

DOC_OUT  = README.pdf

UTIL_URL = https://github.com/gwu-cs-sysprog/utils/raw/main/util.tgz

all: $(UTIL) $(TEST_BIN)

$(UTIL):
	wget $(UTIL_URL)
	tar zxvf util.tgz
	rm util.tgz

%.test: %.o $(LIB)
	$(CC) -o $@ $< $(LDFLAGS)

$(LIB): $(LIBOBJS)
	$(AR) -crs $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(UTIL) $(TEST_BIN)
	@echo "Running tests..."
	$(foreach T, $(TEST_BIN), ./$(T);)
	@echo "\nRunning valgrind..."
	$(foreach T, $(TEST_BIN), sh $(UTIL)/valgrind_test.sh ./$(T);)
	@echo "\nRunning symbol visibility test..."
	sh $(UTIL)/assess_visibility.sh "ptrie_add\|ptrie_allocate\|ptrie_autocomplete\|ptrie_free\|ptrie_print\|ptrie_test_eval" $(LIB)

%.pdf: %.md
	pandoc -V geometry:margin=1in $^ -o $@

doc: $(DOC_OUT)

clean:
	rm -rf $(TEST_BIN) $(TEST_DEPS) $(TEST_OBJS) $(OBJECTS) $(DEPFILES) $(LIB) $(LIBOBJS) $(LIBDEPS) $(DOC_OUT)

.PHONY: all test clean doc

# include the dependencies
-include $(DEPFILES) $(TEST_DEPS) $(LIBDEPS)
