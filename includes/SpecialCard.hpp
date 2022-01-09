#ifndef SPECIALCARD_HPP
#define SPECIALCARD_HPP

#include <string>

#include "Card.hpp"

class SpecialCard : public virtual Card{

    private:
        int                 m_id;
        std::string         m_info;

    public:
        SpecialCard(std::string, int, int, std::string);
        virtual ~SpecialCard() = 0;

        virtual int         get_id() const;
        virtual std::string get_info() const;

        virtual void        set_id(int i);
        virtual void        set_info(std::string s);

        virtual void        print() = 0;

};

#endif
