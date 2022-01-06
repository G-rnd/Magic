#ifndef LAND_HPP
#define LAND_HPP

#include <string>
#include <vector>

#include "BasicCard.hpp"
#include "Enchantment.hpp"
#include "Cost.hpp"

class Land : public virtual BasicCard{

    private:
        bool m_engaged;
        bool m_is_first_turn;
        std::vector<Enchantment> m_enchantments;
        int m_value;

    public:
        Land(std::string, int, bool, bool, int);
        ~Land();

        void print() const;

        void engage_lands(std::vector<Land>);
        // disengage useless ?

};

#endif