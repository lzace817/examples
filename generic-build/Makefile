.SUFFIXES:

srcdir  = .
outdir ?= build
OBJS = $(patsubst $(srcdir)/%.c, $(outdir)/%.o, $(wildcard $(srcdir)/libs/*.c))
INC_DIR = $(srcdir)/libs

CFLAGS  = -Wall -Wextra -Wpedantic -Wstrict-prototypes -Wmissing-prototypes -ggdb
# CFLAGS += -Werror
## comment bellow to look for unsused
CFLAGS += -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -I$(INC_DIR)

BIN = $(patsubst $(srcdir)/%.c, $(outdir)/%, $(wildcard $(srcdir)/*.c))
HEADERS = $(wildcard $(srcdir)/libs/*.h)
# CFLAGS += -D DEBUG

# -include config.mk

all: $(BIN)

list:
	echo "objects" $(OBJS)
	echo "bin" $(BIN)

%.a: $(OBJS)
	@ar rcs $@ $^

OBJ_DIRS = $(sort $(patsubst %/,%,$(dir $(OBJS))))

$(OBJS): | $(OBJ_DIRS)

$(OBJ_DIRS):
	@mkdir -p $@

$(outdir)/%.o: $(srcdir)/%.c | $(HEADERS)
	@echo "compiling $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

$(outdir)/%: $(outdir)/%.o $(outdir)/libblob.a | $(HEADERS)
	@echo "linking $@"
	@$(CC) $(CFLAGS) -L$(outdir) -o $@ $< -lblob

clean:
	rm -r $(outdir)

.PHONY: all clean list
