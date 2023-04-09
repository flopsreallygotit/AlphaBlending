# Configure compiler.
CXX ?= g++

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Configure compile flags.
CXXFLAGS ?= -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ 				   \
			-Waggressive-loop-optimizations -Wc++14-compat 				   \
			-Wmissing-declarations -Wcast-align -Wcast-qual 			   \
			-Wchar-subscripts -Wconditionally-supported 				   \
			-Wconversion -Wctor-dtor-privacy -Wempty-body 				   \
			-Wfloat-equal -Wformat-nonliteral -Wformat-security 		   \
			-Wformat-signedness -Wformat=2 -Winline -Wlogical-op 		   \
			-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual 		   \
			-Wpacked -Wpointer-arith -Winit-self -Wredundant-decls		   \
			-Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel \
			-Wstrict-overflow=2 -Wsuggest-attribute=noreturn			   \
			-Wsuggest-final-methods -Wsuggest-final-types				   \
			-Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand  \
			-Wundef -Wunreachable-code -Wunused -Wuseless-cast 			   \
			-Wvariadic-macros -Wno-literal-suffix 						   \
			-Wno-missing-field-initializers -Wno-narrowing 				   \
			-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new \
			-fsized-deallocation -fstack-protector -fstrict-overflow 	   \
			-flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE 	   \

# Configure linker flags.
LINKFLAGS ?= -lsfml-graphics -lsfml-window -lsfml-system

# Configure program perfomance boost flags.
FASTFLAGS ?= -Ofast -mavx -mavx2 -mavx512dq # -DSLOW # <-- Slow flag for debug and tests

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Configure directory flags.
DIRFLAGS ?= -IUniversal

# Configure sources you need to compile.
SOURCES  ?= main.cpp mainUtils.cpp 		\
			Universal/universalUtils.cpp

# Configure sources dependencies.
DEPENDS  ?= mainUtils.hpp

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Configure object directory name.
OBJDIR ?= ./Objects/

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Pick executable name.
EXECUTABLE ?= main

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# WARNING: Don't change values and commands below.

# Making objects names from sources names.
OBJECTS = $(patsubst %.cpp, $(OBJDIR)%.o, $(SOURCES)) 

# Adding directory flags to compiler flags.
CXXFLAGS += $(DIRFLAGS)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(DEPENDS)
	@echo "Making executable from objects;"
	@$(CXX) $(OBJECTS) -o $@ $(LINKFLAGS)
	@echo "Done;"

$(OBJDIR)%.o: %.cpp
	@mkdir -p $(@D)
	@echo "Making object file from $<;"
	@$(CXX) -c $(CXXFLAGS) $(FASTFLAGS) $^ -o $@

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.PHONY: clear
clear:
	@echo "Clearing object files and executables;";
	@rm -rf $(OBJDIR) $(EXECUTABLE)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~