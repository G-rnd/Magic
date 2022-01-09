#ifndef BASICCARD_HPP
#define BASICCARD_HPP

#include <string>

#include "Card.hpp"
#include "Enchantment.hpp"
#include "Vector.hpp"

class BasicCard : public virtual Card{

    private:
        bool                            m_engaged;
        Vector<Enchantment>             m_enchantments;

    public:
        BasicCard(std::string, int);
        virtual ~BasicCard() = 0;

        virtual bool                    get_engaged() const;
        virtual Vector<Enchantment>     get_enchantments() const;

        virtual void                    set_engaged(bool);
        virtual void                    add_enchantment(Enchantment &e);
            
        virtual void                    print() = 0;

};

#endif
