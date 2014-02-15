###### User-configurable variables ######
# Executable name
EXEC=scheme

# Directory for source files
SOURCESDIR=src
# Directory to store object files
OBJECTDIR=obj

# Compiler
CC=gcc
# Compiler flags
CFLAGS=-g -Wall -std=c99
# Linker flags
LDFlAGS=

###### Generated variables ######
SOURCES=$(wildcard $(SOURCESDIR)/*.c)
OBJECTS=$(addprefix $(OBJECTDIR)/,$(notdir $(SOURCES:.c=.o)))
SOURCESWC=$(SOURCESDIR)/%.c
OBJECTWC=$(OBJECTDIR)/%.o

###### Targets ######
all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

$(OBJECTWC): $(SOURCESWC)
	mkdir -p $(OBJECTDIR)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXEC) $(OBJECTS)
