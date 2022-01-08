#ifndef BATTLEFIELD_HPP
#define BATTLEFIELD_HPP

#include <vector>

#include "Cards/BasicCards/BasicCard.hpp"
#include "Cards/BasicCards/Creature.hpp"
#include "Cards/BasicCards/Land.hpp"
#include "Cards/SpecialCards/Enchantment.hpp"


class Battlefield {
    private:
        std::vector<BasicCard*>      m_basic_cards;
        std::vector<Enchantment*>    m_enchantments;

    public:
        Battlefield();
        ~Battlefield();
        
        std::vector<BasicCard*>      get_basic_cards() const;
        std::vector<Enchantment*>    get_enchantments() const;

        std::vector<Creature*>       get_engaged_creatures() const;
        std::vector<Creature*>       get_available_creatures() const;
        std::vector<Land*>           get_available_lands() const;

        void                         print();

};

#endif