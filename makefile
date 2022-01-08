CXX=g++
CXXFLAGS=-Wall -Wextra

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

main: main.o Game.o Player.o Battlefield.o Cards/Card.o Cards/BasicCards/BasicCard.o Cards/SpecialCards/SpecialCard.o Cards/BasicCards/Creature.o Cards/BasicCards/Land.o Cards/SpecialCards/Enchantment.o Cards/SpecialCards/Ritual.o Cost.o
	$(CXX) $^ -o $@

.PHONY: run clean

run: main
	./$<

clean:
	rm -f *.o *~ *.core

