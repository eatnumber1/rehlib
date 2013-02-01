CC ?= clang
DEPGEN ?= gcc
PKG_CONFIG ?= pkg-config

PREFIX ?= /usr/local
GAMEDIR ?= $(PREFIX)/nethack
VARDIR ?= $(GAMEDIR)/var

-include config.mk

SUBDIRS := src test

TOPDIR := $(PWD)
INCDIR := $(TOPDIR)/include
SRCDIR := $(TOPDIR)/src

CPPFLAGS := $(CPPFLAGS) -I$(INCDIR)
CFLAGS := $(CFLAGS) -std=c1x -Wall -Werror -Wextra -ggdb
LDFLAGS := $(LDFLAGS) -fvisibility=hidden

GLIB_CPPFLAGS ?= $(shell $(PKG_CONFIG) --cflags glib-2.0)
GLIB_LIBRARIES ?= $(shell $(PKG_CONFIG) --libs glib-2.0)
GMODULE_LIBRARIES ?= $(shell $(PKG_CONFIG) --libs gmodule-2.0)

UNAME := $(shell uname -s)
ifeq ($(UNAME), Darwin)
LIBRARY_SUFFIX := dylib
else
LIBRARY_SUFFIX := so
endif

CLEAN_TARGETS := $(SUBDIRS:=/clean)
DEPCLEAN_TARGETS := $(SUBDIRS:=/depclean)
ALL_TARGETS := $(SUBDIRS:=/all)
TEST_TARGETS := $(SUBDIRS:=/test)

.PHONY: all clean depclean test
.DEFAULT_GOAL: all

all: $(ALL_TARGETS)
clean: $(CLEAN_TARGETS)
depclean: clean $(DEPCLEAN_TARGETS)
test: $(TEST_TARGETS)

# Define default hooks so a subdir doesn't need to define them.
$(CLEAN_TARGETS):
$(DEPCLEAN_TARGETS):
$(TEST_TARGETS):

define variableRule
 CURDIR := $$(TOPDIR)/$$$(1)
 include $$(CURDIR)/variables.mk
endef
$(foreach subdir, $(SUBDIRS), $(eval $(call variableRule, $(subdir))))

# This defines the following for every dir in SUBDIRS:
#   Sets CURDIR to the $(TOPDIR)/$(dir)
#   Includes a makefile in $(CURDIR)/Makefile
define subdirRule
 CURDIR := $$(TOPDIR)/$$$(1)
 $$$(1)/all: CURDIR := $$(CURDIR)
 $$$(1)/clean: CURDIR := $$(CURDIR)
 $$$(1)/depclean: CURDIR := $$(CURDIR)
 $$$(1)/test: CURDIR := $$(CURDIR)
 include $$(CURDIR)/Makefile
endef
# This is what actually does the work.
# The "call" command replaces every $(1) variable reference in subdirRule with $(subdir)
# The "eval" command parses the result of the "call" command as make syntax
$(foreach subdir, $(SUBDIRS), $(eval $(call subdirRule, $(subdir))))
# Reset CURDIR back to what it should be.
CURDIR := $(TOPDIR)

%.exe:
	$(CC) $(LIBRARIES) $(LDFLAGS) -o $@ $(OBJECTS)

%.dylib:
	$(CC) $(LIBRARIES) $(LDFLAGS) -shared -o $@ $(OBJECTS)

%.d: %.c
	$(DEPGEN) -MM $(CPPFLAGS) -MQ $(@:.d=.o) -MQ $@ -MF $*.d $<

%.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

# vim:tw=80
