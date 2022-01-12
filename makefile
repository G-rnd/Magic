CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++2a

EXEC = magic

SRCDIR = sources
OBJDIR = obj
HPPDIR = includes
BIN = bin

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:.cpp =$(OBJDIR)/*.o)
INCLUDES = $(wildcard $(HPPDIR)/*.hpp)
EXEPATH = $(BIN)/$(EXEC)

all: $(EXEPATH)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@ -I ./$(HPPDIR)/

$(EXEPATH): $(OBJ)
#	mkdir bin
#	mkdir obj
	$(CXX) $(CXXFLAGS) $^ -o $@ -I ./$(HPPDIR)/

.PHONY: run clean

run: $(EXEPATH)
	cls
	./$<

clean:
	rm -f $(OBJDIR)/*.o *~ *.core
