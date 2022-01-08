CXX=g++
CXXFLAGS=-Wall -Wextra

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

main: main.o Game.o Player.o Battlefield.o Card.o BasicCard.o SpecialCard.o Creature.o Land.o Enchantment.o Ritual.o Cost.o
	$(CXX) $^ -o $@

.PHONY: run clean

run: main
	./$<

clean:
	rm -f *.o *~ *.core

