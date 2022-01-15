#ifndef ENCHANTMENT_HPP
#define ENCHANTMENT_HPP

#include "SpecialCard.hpp"

class Enchantment : public virtual SpecialCard{

    private:

    public:
        Enchantment(int, std::string, int, std::string, Cost*, std::vector<int>);
        ~Enchantment();

        virtual void print();

};

#endif
