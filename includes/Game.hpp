#ifndef GAME_HPP
#define GAME_HPP

#include <string>

#include "Player.hpp"
#include "Vector.hpp"

class Game {
    private:
        bool                m_player_turn;
        Vector<Player>      m_players;
        
    public:
        Game();
        ~Game();
        
        bool                get_player_turn() const;
        void                set_player_turn(bool b);
        void                next_player_turn();

        Vector<Player>      get_players() const;
        Player              get_first_player() const;
        Player              get_second_player() const;
        Player              get_current_player() const;

        void                start();
        void                choose_name(Player p, std::string name);
        void                choose_deck();
        void                save();
        void                choose_save();
        void                load(std::string filename);
        void                exit();
        void                victory(Player p);


};

#endif

