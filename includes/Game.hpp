#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>

#include "Player.hpp"

class Game {

    private:
        size_t               m_phase;
        bool                 m_player_turn;
        std::vector<Player*> m_players;
         
    public: 
        Game(); 
        ~Game(); 
         
        bool                 get_player_turn() const;
        size_t               get_phase() const;
        void                 set_player_turn(bool b);
        void                 set_phase(size_t i);
        void                 set_players(Player* p1, Player* p2);
        void                 next_player_turn();
        void                 next_phase();
 
        std::vector<Player*> get_players() const;
        Player*              get_first_player() const;
        Player*              get_second_player() const;
        Player*              get_current_player() const;

        void                 start();
        void                 phases();
        bool                 check_defeat();
        bool                 main_phase(bool first);
        void                 combat_phase();
        void                 turn_end_phase();
        void                 save(const std::string& name);
        std::string          to_string();
        void                 exit();
        void                 victory(Player p);

        static void          print_title_screen();
        static void          load();
        static void          choose_save(std::string& data);
};

#endif
