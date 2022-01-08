#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <string>
#include <vector>

#include "BasicCard.hpp"
#include "Enchantment.hpp"
#include "Cost.hpp"
#include "Land.hpp"

class Creature : public virtual BasicCard{

    private:
        int              m_power;
        int              m_toughness;
        int              m_power_current;
        int              m_toughness_current;
        std::vector<int> m_abilities;
        std::vector<int> m_types;
        Cost             m_cost;

    public:
        Creature(std::string, int, bool, bool, int, int, std::vector<int>, std::vector<int>, Cost);
        ~Creature();

        int              get_power() const;
        int              get_toughness() const;
        int              get_power_current() const;
        int              get_toughness_current() const;
        std::vector<int> get_abilities() const;
        std::vector<int> get_types() const;
        Cost             get_cost() const;

        void             set_power(int);
        void             set_toughness(int);
        void             set_power_current(int);
        void             set_toughness_current(int);
             
        void             add_ability(int);
        void             add_type(int);
             
        void             print() const;
             
        void             engage_lands(std::vector<Land>) const;
        // disengage useless ?

};

#endif