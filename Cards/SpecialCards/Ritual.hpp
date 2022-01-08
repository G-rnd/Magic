#ifndef RITUAL_HPP
#define RITUAL_HPP

#include "Cards/SpecialCards/SpecialCard.hpp"

class Ritual : public virtual SpecialCard{

    private:
    
    public:
        Ritual(std::string, int, int, std::string);
        ~Ritual();

};

#endif