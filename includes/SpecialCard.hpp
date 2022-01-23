#ifndef SPECIALCARD_HPP
#define SPECIALCARD_HPP

#include <vector>
#include <string>

#include "Card.hpp"
#include "Cost.hpp"

class SpecialCard : public virtual Card {

    private:
        Cost*                    m_cost;
        std::vector<int>         m_effects;

    public:
        SpecialCard(int, std::string, int, Cost*, std::vector<int>);
        virtual ~SpecialCard() = 0;

        virtual Cost*            get_cost() const;
        virtual std::vector<int> get_effects() const;
     
        virtual void             print() = 0;

};

#endif
