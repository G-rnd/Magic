#ifndef BASICCARD_HPP
#define BASICCARD_HPP

#include <string>
#include <vector>

#include "Card.hpp"
#include "Enchantment.hpp"

class BasicCard : public Card{

    private:
        bool m_engaged;
        bool m_is_first_turn;
        std::vector<Enchantment> m_enchantments;

    public:
        BasicCard(std::string, int, bool, bool);
        virtual ~BasicCard() = 0;

        virtual void print() = 0;

};

#endif