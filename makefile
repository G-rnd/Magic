CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++2a

EXEC = magic

SRCDIR = sources
OBJDIR = obj
HPPDIR = includes
BIN = bin

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ_ = $(SRC:$(SRCDIR)/%=$(OBJDIR)/%) 
OBJ = $(OBJ_:%.cpp=%.o)

INCLUDES = $(wildcard $(HPPDIR)/*.hpp)
EXEPATH = $(BIN)/$(EXEC)

all: $(EXEPATH)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) $< -c -o $@ -I ./$(HPPDIR)/

$(EXEPATH): $(OBJ)
	mkdir -p obj
	mkdir -p bin
	$(CXX) $^ -o $@ -I ./$(HPPDIR)/


.PHONY: run clean

print:
	@echo $(SRC)
	@echo $(OBJ)

run: $(EXEPATH)
	clear
	./$<

clean:
	rm -rf bin || true
	rm -rf obj || true
