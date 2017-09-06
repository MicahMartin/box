# This is the main compiler
CC := avr-gcc

# Source directory
SRCDIR := src

# Build directory
BUILDDIR := build

# Target binary directory
TARGET := bin/bitflip

# extension of source files
SRCEXT := c

# gets all .c files in source directory
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

# This shit is some tricky gnu wizardy. $(VARIABLE:.old_extension:.new_extension) changes file extension
# otherwise, patsubst works pretty much like String.replace in java
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))

# flags being passed to GCC
CFLAGS := -mmcu=atmega328p -DF_CPU=16000000UL -O
# include all header files
INC := -I include -I /usr/local/Cellar/avr-gcc/6.2.0/avr/include

$(TARGET): $(OBJECTS)
	@echo " Linking... Object files are "
	@echo " $(CC) -mmcu=atmega328p $^ -o $(TARGET)"; $(CC) -O -mmcu=atmega328p $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo " I am first" 
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean
