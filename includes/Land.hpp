#ifndef LAND_HPP
#define LAND_HPP

#include <string>

#include "BasicCard.hpp"
#include "Enchantment.hpp"

class Land : public virtual BasicCard {

    private:
        int             m_value;

    public:
        Land(int, std::string, int, int);
        ~Land();

        int             get_value() const;
        void            set_value(int i);

        void            print();
        std::string     to_string();
};

#endif
