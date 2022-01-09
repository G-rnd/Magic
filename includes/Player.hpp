#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

#include "Card.hpp"
#include "BasicCard.hpp"
#include "Creature.hpp"
#include "Battlefield.hpp"
#include "Vector.hpp"

class Player {
    private:
        std::string         m_name;
        int                 m_hp;
        bool                m_played_land;
        Battlefield         m_battlefield;
        Vector<Card>        m_deck;
        Vector<Card>        m_graveyard;
        Vector<Card>        m_library;
        Vector<Card>        m_hand;

    public:
        Player();
        ~Player();

        std::string         get_name() const;
        int                 get_hp() const;
        bool                get_played_land() const;
        Battlefield         get_battlefield() const;
        Vector<Card>        get_deck() const;
        Vector<Card>        get_graveyard() const;
        Vector<Card>        get_library() const;
        Vector<Card>        get_hand() const;

        void                set_name(std::string s);
        void                set_ph(int i);
        void                set_played_land(bool b);

        void                draw_card();
        void                discard_card(Card* c);
        void                shuffle_library();
        void                play_card(Card* c);
        void                engage_card(BasicCard* bc);
        void                disengage_card(BasicCard* bc);
        void                attack(Creature c);
        void                deflect_attack(Creature opponent, Vector<Creature> defenders);
        void                battle_creature(Creature opponent, Creature defender);
        void                destroy_card(Card* c);
        void                loose();

};

#endif
