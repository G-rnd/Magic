#ifndef CARD_HPP
#define CARD_HPP

#include <string>

class Card {

    private:
        std::string m_name;
        int m_token;

    public:
        Card(std::string, int);
        virtual ~Card() = 0;

        virtual void print() = 0;

};

#endif