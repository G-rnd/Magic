#ifndef SPECIALCARD_HPP
#define SPECIALCARD_HPP

#include <string>
#include <vector>

#include "Card.hpp"

class SpecialCard : public virtual Card{

    private:
        int m_id;
        std::string m_info;

    public:
        SpecialCard(std::string, int, int, std::string);
        virtual ~SpecialCard() = 0;

        virtual int get_id() const;
        virtual std::string get_info() const;

        virtual void set_id(int);
        virtual void set_info(std::string);

        virtual void print() = 0;

};

#endif