#ifndef LAND_HPP
#define LAND_HPP

#include <string>

#include "BasicCard.hpp"
#include "Enchantment.hpp"
#include "Vector.hpp"

class Land : public virtual BasicCard{

    private:
        int             m_value;

    public:
        Land(std::string, int, int);
        ~Land();

        int             get_value() const;

        void            set_value(int i);

        virtual void    print();
};

#endif
