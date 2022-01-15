#ifndef CARDPARSER_HPP
#define CARDPARSER_HPP

#include <string>
#include <vector>

#include "Card.hpp"
#include "Creature.hpp"
#include "Land.hpp"
#include "Enchantment.hpp"
#include "Ritual.hpp"

class CardParser {
    private:
        CardParser() {}

    public:
		static const std::string begin_card;
		static const std::string end_card;
		static const std::string type_card;
		static const std::string name;
		static const std::string token;
		static const std::string power;
		static const std::string toughness;
		static const std::string abilities;
		static const std::string types;
		static const std::string cost;
        static const std::string value;
        static const std::string info;
        static const std::string effects;
        static const std::string comment;
        static const std::string creature;    
        static const std::string land;        
        static const std::string enchantment; 
        static const std::string ritual;      

        static std::vector<Card*>       parse(const std::string& filename);
        static std::vector<std::string> get_line_data(const std::string& data);
        static void                     clean_data(std::vector<std::string>& data);
        static Card*                    parse_card(const std::vector<std::string>& data);
        static Creature*                parse_creature(const std::vector<std::string>& data);
        static Land*                    parse_land(const std::vector<std::string>& data);
        static Enchantment*             parse_enchantment(const std::vector<std::string>& data);
        static Ritual*                  parse_ritual(const std::vector<std::string>& data);
        static std::vector<int>         parse_int_list(std::string& s);
};

#endif
