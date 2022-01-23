#ifndef RITUAL_HPP
#define RITUAL_HPP

#include <vector>
#include <string>

#include "SpecialCard.hpp"

class Ritual : public virtual SpecialCard {

    public:
        Ritual(int, std::string, int, Cost*, std::vector<int>);
        ~Ritual();

        virtual void print();

};

#endif
