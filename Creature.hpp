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
        int m_power;
        int m_toughness;
        int m_power_current;
        int m_toughness_current;
        std::vector<int> m_abilities;
        std::vector<int> m_types;

    public:
        Creature(std::string, int, bool, bool, int, int, std::vector<int>, std::vector<int>);
        ~Creature();

        void print() const;

};

#endif