#ifndef ENCHANTMENT_HPP
#define ENCHANTMENT_HPP

#include "SpecialCard.hpp"

class Enchantment : public SpecialCard{

    private:

    public:
        Enchantment(std::string, int, int, std::string);
        ~Enchantment();

        void print();

};

#endif