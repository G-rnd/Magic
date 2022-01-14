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
        std::vector<Enchantment*> m_enchantments;

    public:
        Battlefield();
        ~Battlefield();
        
        std::vector<BasicCard*>  get_basic_cards() const;
        std::vector<Enchantment*>get_enchantments() const;
        
        void                     set_basic_cards(std::vector<BasicCard*> bc);
        void                     set_enchantments(std::vector<Enchantment*> e);

        void                     remove_basic_card(BasicCard* bc);
        void                     remove_enchantment(Enchantment* e);
    
        
        void                     place_basic_card(BasicCard* bc);
        std::vector<Creature*>   get_engaged_creatures();
        std::vector<Creature*>   get_available_creatures();
        std::vector<Land*>       get_available_lands();

        void                     disengage_card(BasicCard* bc);
        bool                     is_playable(Creature c); 

        void                     print();

};

#endif
