#ifndef SPECIALCARD_HPP
#define SPECIALCARD_HPP

#include <string>

#include "Card.hpp"
#include "Cost.hpp"

class SpecialCard : public virtual Card{

    private:
        int                 m_id;
        std::string         m_info;
        Cost*               m_cost;

    public:
        SpecialCard(std::string, int, int, std::string, Cost*);
        virtual ~SpecialCard() = 0;

        virtual int         get_id() const;
        virtual std::string get_info() const;
        virtual Cost*       get_cost() const;

        virtual void        set_id(int i);
        virtual void        set_info(std::string s);

        virtual void        print() = 0;

};

#endif
