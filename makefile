CXX=g++
CXXFLAGS=-Wall -Wextra

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

main: main.o Game.o Player.o Battlefield.o
	$(CXX) $^ -o $@

.PHONY: run clean

run: main
	./$<

clean:
	rm -f *.o *~ *.core

