.PHONY: test/all test/clean test/depclean test/test

test/all:

test/clean:
	$(RM) $(TEST_OBJECTS) $(TEST_EXE)

test/depclean:
	$(RM) $(TEST_DEPS)

test/test: $(TEST_EXE)
	$(TEST_EXE)

-include $(TEST_DEPS)

$(TEST_OBJECTS) $(TEST_DEPS): CPPFLAGS := $(CPPFLAGS) $(GLIB_CPPFLAGS) -I$(SRCDIR)

$(TEST_EXE): $(TEST_OBJECTS)
$(TEST_EXE): OBJECTS := $(TEST_OBJECTS)
$(TEST_EXE): LIBRARIES := $(LIBRARIES) $(GLIB_LIBRARIES) $(GMODULE_LIBRARIES) $(TEST_LIBRARIES)
