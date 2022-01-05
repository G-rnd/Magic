#ifndef CARD_HPP
#define CARD_HPP

#include <string>

class Card {

    private:
        std::string m_name;
        int m_token;

    public:

        Card(std::string, int);
        ~Card();

        virtual void print() const;

};

#endif