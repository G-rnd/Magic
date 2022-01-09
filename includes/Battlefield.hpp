#ifndef BATTLEFIELD_HPP
#define BATTLEFIELD_HPP

#include "BasicCard.hpp"
#include "Creature.hpp"
#include "Land.hpp"
#include "Enchantment.hpp"
#include "Vector.hpp"

class Battlefield {
    private:
        Vector<BasicCard>   m_basic_cards;
        Vector<Enchantment> m_enchantments;

    public:
        Battlefield();
        ~Battlefield();
        
        Vector<BasicCard>   get_basic_cards();
        Vector<Enchantment> get_enchantments();

        Vector<Creature>    get_engaged_creatures();
        Vector<Creature>    get_available_creatures();
        Vector<Land>        get_available_lands();

        void                print();

};

#endif