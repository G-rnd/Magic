#ifndef BASICCARD_HPP
#define BASICCARD_HPP

#include <string>
#include <vector>

#include "Cards/Card.hpp"
#include "Cards/SpecialCards/Enchantment.hpp"

class BasicCard : public virtual Card{

    private:
        bool                             m_engaged = false;
        bool                             m_is_first_turn = false; // TODO : Land n'en a pas besoin, transférer pour Creature
        std::vector<Enchantment>         m_enchantments;

    public:
        BasicCard(std::string, int, bool, bool);
        virtual ~BasicCard() = 0;

        virtual bool                     get_engaged() const;
        virtual bool                     get_is_first_turn() const;
        virtual std::vector<Enchantment> get_enchantments() const;

        virtual void                     set_engaged(bool);
        virtual void                     set_is_first_turn(bool);
        virtual void                     add_enchantment(Enchantment*);
            
        virtual void                     print() = 0;

};

#endif