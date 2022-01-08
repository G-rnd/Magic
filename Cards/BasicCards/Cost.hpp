#ifndef COST_HPP
#define COST_HPP

#include <string>
#include <vector>

#include "Creature.hpp"

class Cost{

    private:
        int  m_any;
        int  m_white;
        int  m_black;
        int  m_red;
        int  m_green;
        int  m_blue;

    public:
        Cost(int, int, int, int, int, int);
        ~Cost();

        int  get_any() const;
        int  get_white() const;
        int  get_black() const;
        int  get_red() const;
        int  get_green() const;
        int  get_blue() const;
 
        int  set_any(int);
        int  set_white(int);
        int  set_black(int);
        int  set_red(int);
        int  set_green(int);
        int  set_blue(int);

        void print() const;
};

#endif