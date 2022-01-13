#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

#include "CardParser.hpp"
#include "Card.hpp"
#include "Cost.hpp"
#include "Creature.hpp"
#include "Land.hpp"
#include "Enchantment.hpp"
#include "Ritual.hpp"

const std::string CardParser::begin_card    = "#BEGIN_CARD";
const std::string CardParser::end_card      = "#END_CARD";
const std::string CardParser::type_card     = "#TYPE_CARD: ";
const std::string CardParser::name          = "#NAME: ";
const std::string CardParser::token         = "#TOKEN: ";
const std::string CardParser::power         = "#POWER:";
const std::string CardParser::toughness     = "#TOUGHNESS: ";
const std::string CardParser::abilities     = "#ABILITIES: ";
const std::string CardParser::types         = "#TYPES: ";
const std::string CardParser::cost          = "#COST: ";
const std::string CardParser::value         = "#VALUE: ";
const std::string CardParser::id            = "#ID: ";
const std::string CardParser::info          = "#INFO: ";
const std::string CardParser::effects       = "#EFFECTS: ";
const std::string CardParser::comment       = "//";
const std::string CardParser::creature      = "Creature";
const std::string CardParser::land          = "Land";
const std::string CardParser::enchantment   = "Enchantment";
const std::string CardParser::ritual        = "Ritual";


void CardParser::clean_data(std::vector<std::string>& data) {
    std::transform(data.begin(), data.end(), data.begin(), [] (std::string& s) {
        s.erase(std::remove(s.begin(), s.end(), '\t'), s.end());
        while(s[0] == ' ')
			s.erase(s.begin());
		while(s.back() == ' ')
			s.erase(s.end()-1);
        return s;
    });
}

std::vector<std::string> CardParser::get_line_data(const std::string& data) {
	std::vector<std::string> array{ "" };

    for(auto i : data) {
    	if (i == '\n')
    		array.push_back("");
    	else
    		array.back() += i;
    }
    return array;
}

std::vector<Card*> CardParser::parse(const std::string& filename) {
    std::vector<Card*> v{};

    std::string read_file;
	std::getline(std::ifstream(filename), read_file, '\0');

    std::vector<std::string> data = get_line_data(read_file);
    CardParser::clean_data(data);
    
    for(auto& i : data)
    	if (!i.empty() && (i.back() == '\r' || i.back() == '\n' || i.back() == '\t'))
    		i = i.substr(0, i.size()-1);
	
    std::erase_if(data, [] (const std::string& s) { return s == "" || s.find('\r') == 0; });
	std::erase_if(data, [] (const std::string& s) { return s.find(CardParser::comment) == 0; });

    for(unsigned int i = 0; i < data.size(); i++) {
        if(data[i].find(CardParser::begin_card) != std::string::npos) {
            bool ok = false;
            for(unsigned int j = 1; ((j < data.size() - i) && (!ok)); j++) {
                if (data[i+j].find(CardParser::end_card) != std::string::npos) {
                    std::vector<std::string> card_lines = { data.begin()+i+1, data.begin()+i+j };
                    data.erase(data.begin()+i, data.begin()+i+j+1);
                    Card* c = parse_card(card_lines);
                    if (c)
                        v.push_back(c);
                    ok = true;
                }
            }
            i--;
        }    
    }
    return v;
}

Card* CardParser::parse_card(const std::vector<std::string>& data) {
    if (data.size() > 0) {
        std::string type_card = data[0].substr(CardParser::type_card.size());
        if (type_card == CardParser::creature)
            return parse_creature({data.begin() + 1, data.end()});

        if (type_card == CardParser::land)
            return parse_land({data.begin() + 1, data.end()});
            
        if (type_card == CardParser::enchantment)
            return parse_enchantment({data.begin() + 1, data.end()});

        if (type_card == CardParser::ritual)
            return parse_ritual({data.begin() + 1, data.end()});

    }
    return nullptr;
}

Creature* CardParser::parse_creature(const std::vector<std::string>& data) {
    uint8_t valid = 0;

    std::string name = "";
    int token = 0;
    int power = 0;
    int toughness = 0;
    std::vector<int> abilities {};
    std::vector<int> types {};
    std::vector<int> cost {};

    for(auto& s : data) {
        if(s.find(CardParser::name) != std::string::npos) {
            name = s.substr(CardParser::name.size());
            valid |= 1;
            continue;
        }

        if(s.find(CardParser::token) != std::string::npos) {
            token = stoi(s.substr(CardParser::token.size()));
            valid |= 1 << 1;
            continue;
        }

        if(s.find(CardParser::power) != std::string::npos) {
            power = stoi(s.substr(CardParser::power.size()));
            valid |= 1 << 2;
            continue;
        }
        
        if(s.find(CardParser::toughness) != std::string::npos) {
            toughness = stoi(s.substr(CardParser::toughness.size()));
            valid |= 1 << 3;
            continue;
        }

        if(s.find(CardParser::abilities) != std::string::npos) {
            std::string s_list = s.substr(CardParser::abilities.size());
            abilities = parse_int_list(s_list);
            valid |= 1 << 4;
            continue;
        }

        if(s.find(CardParser::types) != std::string::npos) {
            std::string s_list = s.substr(CardParser::types.size());
            types = parse_int_list(s_list);
            valid |= 1 << 5;
            continue;
        }

        if(s.find(CardParser::cost) != std::string::npos) {
            std::string s_list = s.substr(CardParser::cost.size());
            cost = parse_int_list(s_list);
            if (cost.size() == 6)
                valid |= 1 << 6;
        }
    }

/*
    std::cout << "Reconnaissance d'une créature: " << std::endl;
    std::cout << "Nom:       " << name << std::endl;
    std::cout << "Token:     " << token << std::endl;
    std::cout << "Stats:     " << power << '/' << toughness << std::endl;
    std::cout << "Abilities: (";
    for(auto i : abilities)
        std::cout << i << ", ";
    
    std::cout << ")" << std::endl << "Types:     (";
    for(auto i : types)
        std::cout << i << ", ";

    std::cout << ")" << std::endl << "Coûts:     (";
    for(auto i : cost)
        std::cout << i << ", ";
    std::cout << ")" << std::endl << std::endl;
*/

    if (valid == 0b1111111)
        return new Creature(name, token, power, toughness, abilities, types, new Cost(cost[0], cost[1], cost[2], cost[3], cost[4], cost[5]));        

    return nullptr;
}

Land* CardParser::parse_land(const std::vector<std::string>& data) {
    uint8_t valid = 0;

    std::string name = "";
    int token = 0;
    int value = 0;

    for(auto& s : data) {
        if(s.find(CardParser::name) != std::string::npos) {
            name = s.substr(CardParser::name.size());
            valid |= 1;
            continue;
        }

        if(s.find(CardParser::token) != std::string::npos) {
            token = stoi(s.substr(CardParser::token.size()));
            valid |= 1 << 1;
            continue;
        }

        if(s.find(CardParser::value) != std::string::npos) {
            value = stoi(s.substr(CardParser::value.size()));
            valid |= 1 << 2;
            continue;
        }
    }

    if (valid == 0b111)
        return new Land(name, token, value);

    return nullptr;
}

Enchantment* CardParser::parse_enchantment(const std::vector<std::string>& data) {
    uint8_t valid = 0;

    std::string name = "";
    int token = 0;
    int id = 0;
    std::string info = "";
    std::vector<int> cost {};
    std::vector<int> effects {};

    for(auto& s : data) {
        if(s.find(CardParser::name) != std::string::npos) {
            name = s.substr(CardParser::name.size());
            valid |= 1;
            continue;
        }

        if(s.find(CardParser::token) != std::string::npos) {
            token = stoi(s.substr(CardParser::token.size()));
            valid |= 1 << 1;
            continue;
        }

        if(s.find(CardParser::id) != std::string::npos) {
            id = stoi(s.substr(CardParser::id.size()));
            valid |= 1 << 2;
            continue;
        }

        if(s.find(CardParser::info) != std::string::npos) {
            info = s.substr(CardParser::info.size());
            valid |= 1 << 3;
            continue;
        }

        if(s.find(CardParser::cost) != std::string::npos) {
            std::string s_list = s.substr(CardParser::cost.size());
            cost = parse_int_list(s_list);
            if (cost.size() == 6)
                valid |= 1 << 4;
        }

        if(s.find(CardParser::effects) != std::string::npos) {
            std::string s_list = s.substr(CardParser::effects.size());
            effects = parse_int_list(s_list);
            valid |= 1 << 5;
            continue;
        }

    }

    if (valid == 0b1111)
        return new Enchantment(name, token, id, info, new Cost(cost[0], cost[1], cost[2], cost[3], cost[4], cost[5]), effects);

    return nullptr;
}

Ritual* CardParser::parse_ritual(const std::vector<std::string>& data) {
    uint8_t valid = 0;

    std::string name = "";
    int token = 0;
    int id = 0;
    std::string info = "";
    std::vector<int> cost {};
    std::vector<int> effects {};

    for(auto& s : data) {
        if(s.find(CardParser::name) != std::string::npos) {
            name = s.substr(CardParser::name.size());
            valid |= 1;
            continue;
        }

        if(s.find(CardParser::token) != std::string::npos) {
            token = stoi(s.substr(CardParser::token.size()));
            valid |= 1 << 1;
            continue;
        }

        if(s.find(CardParser::id) != std::string::npos) {
            id = stoi(s.substr(CardParser::id.size()));
            valid |= 1 << 2;
            continue;
        }

        if(s.find(CardParser::info) != std::string::npos) {
            info = s.substr(CardParser::info.size());
            valid |= 1 << 3;
            continue;
        }

        if(s.find(CardParser::cost) != std::string::npos) {
            std::string s_list = s.substr(CardParser::cost.size());
            cost = parse_int_list(s_list);
            if (cost.size() == 6)
                valid |= 1 << 4;
        }

        if(s.find(CardParser::effects) != std::string::npos) {
            std::string s_list = s.substr(CardParser::effects.size());
            effects = parse_int_list(s_list);
            valid |= 1 << 5;
            continue;
        }
    }

    if (valid == 0b1111)
        return new Ritual(name, token, id, info, new Cost(cost[0], cost[1], cost[2], cost[3], cost[4], cost[5]), effects);

    return nullptr;
}

std::vector<int> CardParser::parse_int_list(std::string& s) {
    std::vector<int> l{};

    std::string delimiter = ", ";
    int pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        l.push_back(stoi(s.substr(0, pos)));
        s.erase(0, pos + delimiter.length());
    }
    l.push_back(stoi(s));

    return l;
}
