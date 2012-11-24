TEST_SOURCES := rehlib_test.c
TEST_SOURCES := $(TEST_SOURCES:%=$(CURDIR)/%)
TEST_OBJECTS := $(TEST_SOURCES:.c=.o)
TEST_DEPS := $(TEST_SOURCES:.c=.d)
TEST_EXE := $(CURDIR)/rehlib_test.exe
