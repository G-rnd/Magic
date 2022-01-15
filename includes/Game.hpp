#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>

#include "Player.hpp"

class Game {

    private:
        bool                 m_player_turn;
        std::vector<Player*> m_players;
         
    public: 
        Game(); 
        ~Game(); 
         
        bool                 get_player_turn() const;
        void                 set_player_turn(bool b);
        void                 next_player_turn();
 
        std::vector<Player*> get_players() const;
        Player*              get_first_player() const;
        Player*              get_second_player() const;
        Player*              get_current_player() const;

        void                 start();
        bool                 check_defeat();
        void                 main_phase();
        void                 combat_phase();
        void                 turn_end_phase();
        void                 save();
        void                 choose_save();
        void                 load();
        void                 exit();
        void                 victory(Player p);

};

#endif
