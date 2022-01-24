#ifndef SAVEPARSER_HPP
#define SAVEPARSER_HPP

#include <string>
#include <vector>

#include "Game.hpp"
#include "Player.hpp"
#include "Battlefield.hpp"
#include "Card.hpp"

class SaveParser {
	private:
        SaveParser() {}

    public:
		static const std::string begin_game;
        static const std::string end_game;
        static const std::string player_turn;
        static const std::string phase;
        static const std::string begin_player;
        static const std::string end_player;
        static const std::string played_land;
        static const std::string name;
        static const std::string hp;
        static const std::string begin_library;
        static const std::string end_library;
        static const std::string begin_graveyard;
        static const std::string end_graveyard;
        static const std::string begin_hand;
        static const std::string end_hand;
        static const std::string begin_battlefield;
        static const std::string end_battlefield;
        static const std::string begin_enchantments;
        static const std::string end_enchantments;
        static const std::string begin_card_list;
        static const std::string end_card_list;
        static const std::string begin_card;
        static const std::string end_card;
        static const std::string classcard;
        static const std::string token;
        static const std::string engaged;
        static const std::string power_toughness;
        static const std::string abilities;
        static const std::string types;
        static const std::string cost;
        static const std::string first_turn;
        static const std::string effects;
        static const std::string value;

        static std::vector<std::string> crop(const std::string& begin, const std::string& end, std::vector<std::string>& data);
        static std::string 				extract_data(const std::string& key, const std::string& data);
        static std::string 				extract_line(const std::string& key, std::vector<std::string>& data);
        static int 						extract_int(const std::string& key, const std::string& data);
        static std::vector<int> 		extract_int_list(const std::string& key, std::string& data);
		static Game*					load(std::string& data);
        static Game*					extract_game(std::vector<std::string>& data);
        static Player*					extract_player(std::vector<std::string>& data);
        static Battlefield*				extract_battlefield(std::vector<std::string>& data);
        static std::vector<Card*>		extract_cards(std::vector<std::string>& data);
        static Card*					extract_card(std::vector<std::string>& data);
        static Creature*				extract_creature(std::vector<std::string>& data);
        static Land*					extract_land(std::vector<std::string>& data);
        static Ritual*					extract_ritual(std::vector<std::string>& data);
        static Enchantment*				extract_enchantment(std::vector<std::string>& data);

};

#endif
