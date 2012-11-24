.PHONY: test/all test/clean test/depclean test/test

test/all:

test/clean:
	$(RM) $(TEST_OBJECTS) $(TEST_EXE)

test/depclean:
	$(RM) $(TEST_DEPS)

test/test: $(TEST_EXE)
	$(TEST_EXE) $(TESTS)

-include $(TEST_DEPS) $(TEST_TEST_DEPS)

$(TEST_OBJECTS): CPPFLAGS := $(CPPFLAGS) $(GLIB_CPPFLAGS)

$(TEST_EXE): $(TEST_OBJECTS) $(REHLIB)
$(TEST_EXE): OBJECTS := $(TEST_OBJECTS)
$(TEST_EXE): LIBRARIES := $(LIBRARIES) $(GLIB_LIBRARIES) $(GMODULE_LIBRARIES) -L$(SRCDIR) -lrehlib
