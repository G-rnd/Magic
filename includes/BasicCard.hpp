#ifndef BASICCARD_HPP
#define BASICCARD_HPP

#include <vector>
#include <string>

#include "Card.hpp"
#include "Enchantment.hpp"

class BasicCard : public virtual Card{

    private:
        bool                             m_engaged;
        std::vector<Enchantment>         m_enchantments;

    public:
        BasicCard(std::string, int);
        virtual ~BasicCard() = 0;

        virtual bool                     get_engaged() const;
        virtual std::vector<Enchantment> get_enchantments() const;

        virtual void                     set_engaged(bool b);
        virtual void                     add_enchantment(Enchantment e);
             
        virtual void                     print() = 0;

};

#endif
