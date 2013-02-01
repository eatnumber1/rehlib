REHLIB := $(CURDIR)/librehlib.$(LIBRARY_SUFFIX)

SRC_SOURCES := common.c refmem.c GBTError.c stdio.c
SRC_SOURCES := $(SRC_SOURCES:%=$(CURDIR)/%)
SRC_OBJECTS := $(SRC_SOURCES:.c=.o)
SRC_DEPS := $(SRC_SOURCES:.c=.d)
