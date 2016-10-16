# ----------------------------------------------------------------------------
# Makefile Raspberry Pi GPIO lib: pi_lib.a
# Version 1, 15 Oct 2016
# ----------------------------------------------------------------------------
# Note:
# $@ = File name of the target.
# $< = Name of the first dependency.
# .PHONY tells make that the target does not correspond to a real file.
# A '-' before a command tells make to ingnore errors.  eg. "-rm -f *.o"
# ----------------------------------------------------------------------------
# CC      = 
# CDEFS   = -DHAVE_PROTOTYPES

# -----------------------------------------------------------------------------
# Directories:
# -----------------------------------------------------------------------------
BUILD_DIR   = ../build/
LIB_DIR	    = $(BUILD_DIR)lib/
BIN_DIR     = $(BUILD_DIR)bin/
OBJ_DIR     = $(BUILD_DIR)lib_obj/
INSTALL_DIR = $(LIB_DIR)

# -----------------------------------------------------------------------------
# Flags:
# -----------------------------------------------------------------------------
CFLAGS  =  -Wall -std=c++11 
IFLAGS  =
LDFLAGS = 
COMPILE_DYNAMIC  = g++ $(IFLAGS) $(CFLAGS) -fpic $(CDEFS) -c
COMPILE_STATIC   = g++ $(IFLAGS) $(CFLAGS) $(CDEFS) -c

# -----------------------------------------------------------------------------
# Targets
# -----------------------------------------------------------------------------
TARGET = $(INSTALL_DIR)pi_lib.a

.PHONY: all
all:  $(TARGET)

# -----------------------------------------------------------------------------
# Libraries that we need
# -----------------------------------------------------------------------------
LIBS = 

# -----------------------------------------------------------------------------
# We list all of our .obj files here.`
# -----------------------------------------------------------------------------
OBJS = 	$(OBJ_DIR)gpio.o

# -----------------------------------------------------------------------------
# We list the individual source file dependencies here.
# -----------------------------------------------------------------------------
$(OBJ_DIR)gpio.o        : gpio.cpp     gpio.hpp



# -----------------------------------------------------------------------------
# Make all of the objects dependent on this makefile.
# Recompile everything if we change this makefile.
# -----------------------------------------------------------------------------
$(OBJS): Makefile

# -----------------------------------------------------------------------------
# Compile pattern rule for making a .o from .cpp files for static
# -----------------------------------------------------------------------------
$(OBJ_DIR)%.o: %.cpp
	$(COMPILE_STATIC) $< -o $@ 

# -----------------------------------------------------------------------------
# Link macro for static
# -----------------------------------------------------------------------------
$(TARGET): $(OBJS) 
	ar rvsc $@ $(OBJS)

# -----------------------------------------------------------------------------
# Targets for making the build directories
# -----------------------------------------------------------------------------
.PHONY: directories
directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(INSTALL_DIR)

	
# -----------------------------------------------------------------------------
# Targets for erasing intermediate and release files.
# -----------------------------------------------------------------------------
.PHONY: clean
clean:
	-rm -f *.out *.[oa] $(OBJS) *.[oa] *.bin *~ *.bak core *.utf8 .#*

.PHONY: cleanall
cleanall: clean
	-rm -f $(TARGET) 


