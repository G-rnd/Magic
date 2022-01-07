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

        virtual std::string get_name() const;
        virtual int get_token() const;

        virtual void set_name(std::string);
        virtual void set_token(int);

        virtual void print() = 0;

};

#endif