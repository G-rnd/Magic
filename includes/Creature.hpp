#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <string>
#include <vector>

#include "BasicCard.hpp"
#include "Enchantment.hpp"
#include "Land.hpp"
#include "Cost.hpp"

class Creature : public virtual BasicCard{

    private:
        int               m_power;
        int               m_toughness;
        int               m_power_current;
        int               m_toughness_current;
        std::vector<int>  m_abilities;
        std::vector<int>  m_types;
        Cost*             m_cost;
        bool              m_is_first_turn;

    public:
        Creature(std::string, int, int, int, std::vector<int>, std::vector<int>, Cost*);
        ~Creature();

        int               get_power() const;
        int               get_toughness() const;
        int               get_power_current() const;
        int               get_toughness_current() const;
        std::vector<int>  get_abilities() const;
        std::vector<int>  get_types() const;
        Cost*             get_cost() const;
        virtual bool      get_is_first_turn() const;


        void              set_power(int i);
        void              set_toughness(int i);
        void              set_power_current(int i);
        void              set_toughness_current(int i);
        virtual void      set_is_first_turn(bool b);
              
        void              add_ability(int i);
        void              add_type(int i);
              
        void              engage_lands(std::vector<Land> lands) const;

        virtual void      print();
};

#endif