#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <string>


#include "Card.hpp"
#include "BasicCard.hpp"
#include "Creature.hpp"
#include "Battlefield.hpp"
#include "Ritual.hpp"

class Player {
    private:
        Player*               m_opponent;
        std::string           m_name;
        int                   m_hp;
        int                   m_played_land;
        Battlefield           m_battlefield;
        std::vector<Card*>    m_deck;
        std::vector<Card*>    m_graveyard;
        std::vector<Card*>    m_library;
        std::vector<Card*>    m_hand;

    public:
        Player(std::string s);
        ~Player();

        Player*                get_opponent() const;
        std::string            get_name() const;
        int                    get_hp() const;
        int                    get_played_land() const;
        Battlefield            get_battlefield() const;
        std::vector<Card*>     get_deck() const;
        std::vector<Card*>     get_graveyard() const;
        std::vector<Card*>     get_library() const;
        std::vector<Card*>     get_hand() const;
   
        void                   set_opponent(Player* p);
        void                   set_name(std::string s);
        void                   set_hp(int i);
        void                   set_library(std::vector<Card*> cards);
        void                   set_hand(std::vector<Card*> cards);
        void                   add_played_land(int i);
 
        void                   add_hand(Card* c);
        void                   remove_battlefield(Card* c);

        void                   sort_hand();
   
        void                   draw_card();
        void                   discard_card(Card* c);
        void                   shuffle_library();
        void                   play_card(Card* c);
        void                   engage_card(BasicCard* bc);
        void                   disengage_card(BasicCard* bc);
        std::vector<Creature*> attack();
        void                   choose_defenders(std::vector<Creature*> opponents);
        void                   deflect_attack(Creature opponent, std::vector<Creature*> defenders);
        void                   battle_creature(Creature opponent, Creature defender);
        void                   destroy_card(Card* c);
        void                   play_ritual(Ritual c);
        void                   loose();   
   
        void                   print();
        void                   print_hand();

};

#endif
