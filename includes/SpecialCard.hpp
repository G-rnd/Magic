#ifndef SPECIALCARD_HPP
#define SPECIALCARD_HPP

#include <vector>
#include <string>

#include "Card.hpp"
#include "Cost.hpp"

class SpecialCard : public virtual Card{

    private:
        std::string              m_info;
        Cost*                    m_cost;
        std::vector<int>         m_effects;

    public:
        SpecialCard(int, std::string, int, std::string, Cost*, std::vector<int>);
        virtual ~SpecialCard() = 0;

        virtual std::string      get_info() const;
        virtual Cost*            get_cost() const;
        virtual std::vector<int> get_effects() const;

        virtual void             set_info(std::string s);
     
        virtual void             print() = 0;

};

#endif
