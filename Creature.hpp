#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <string>
#include <vector>

#include "BasicCard.hpp"
#include "Enchantment.hpp"

class Creature : public virtual BasicCard{

    private:
        bool m_engaged;
        bool m_is_first_turn;
        std::vector<Enchantment> m_enchantments;

        

    public:
        Creature(std::string, int, bool, bool);
        ~Creature();

        void print() const;

};

#endif