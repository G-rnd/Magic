#ifndef LAND_HPP
#define LAND_HPP

#include <string>
#include <vector>

#include "Cards/BasicCards/BasicCard.hpp"
#include "Cards/SpecialCards/Enchantment.hpp"

class Land : public virtual BasicCard{

    private:
        int  m_value;

    public:
        Land(std::string, int, bool, bool, int);
        ~Land();

        int  get_value() const;

        void set_value(int);

        void print() const;
};

#endif