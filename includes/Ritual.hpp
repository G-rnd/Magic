#ifndef RITUAL_HPP
#define RITUAL_HPP

#include <vector>
#include <string>

#include "SpecialCard.hpp"

class Ritual : public virtual SpecialCard{

    private:
        std::vector<int> m_effects;
    
    public:
        Ritual(std::string, int, int, std::string, Cost*, std::vector<int>);
        ~Ritual();

        std::vector<int> get_effects() const;

        virtual void     print();

};

#endif
