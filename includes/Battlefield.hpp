#ifndef BATTLEFIELD_HPP
#define BATTLEFIELD_HPP

#include <vector>

#include "BasicCard.hpp"
#include "Creature.hpp"
#include "Land.hpp"
#include "Enchantment.hpp"

class Battlefield {
    private:
        std::vector<BasicCard*>  m_basic_cards;
        std::vector<Enchantment> m_enchantments;

    public:
        Battlefield();
        ~Battlefield();
        
        std::vector<BasicCard*>  get_basic_cards();
        std::vector<Enchantment> get_enchantments();
        
        std::vector<Creature>    get_engaged_creatures();
        std::vector<Creature>    get_available_creatures();
        std::vector<Land>        get_available_lands();

        void                     disengage_card(BasicCard* bc);

        void                     print();

};

#endif
