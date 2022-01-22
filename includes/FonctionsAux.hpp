#ifndef FCT_AUX_HPP
#define FCT_AUX_HPP

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <map>

#include "Card.hpp"
#include "BasicCard.hpp"
#include "SpecialCard.hpp"
#include "Creature.hpp"
#include "Land.hpp"
#include "Ritual.hpp"
#include "Enchantment.hpp"

namespace Card_class { 
    enum Card_class                 { CREATURE, LAND, RITUAL, ENCHANTEMENT };
}

namespace Token { 
    enum Token                      { White, Blue, Black, Red, Green, Count };
}

namespace State {
    enum State                      { Normal, Engaged, Enchanted, Engaged_Enchanted};
}

namespace Ability { 
    enum Ability                    { Flight, Scope, Vigilance, Touch_of_death, Defender, Initiative, Double_initiative, Haste, Unblockable,
                                    Life_link, Threat, Trampling, White_protection, Blue_protection, Black_protection, Red_protection, Green_protection, Count };
}
                                    

enum Type					    { Angel, Beast, Human_werewolf, HippoGriff, Kor_warrior, Dinosaur, Humans, Vampire, Spider, Elf, Troll };

namespace White_ritual_effects { 
    enum White_ritual_effects	    { More_3_HP, More_1_1_creature_current, Destroy_engaged_creature, Destroy_enchantment, Count };
}
namespace Blue_ritual_effects { 
    enum Blue_ritual_effects	    { Draw_2_cards, Back_hand_creature, Count };
}
namespace Black_ritual_effects { 
    enum Black_ritual_effects	    { Kill_creature, Kill_creature_2_power, Kill_not_angel, Less_2_2_creature_current, Count };
}
namespace Red_ritual_effects { 
    enum Red_ritual_effects		    { Damage_3_creature_or_player, Damage_4_creatures, Count };
}
namespace Green_ritual_effects { 
    enum Green_ritual_effects	    { Play_another_land, Take_2_lands_library_shuffle, Count };
}

// TODO : faire les enchantements 
namespace White_enchantment_effects { 
    enum White_enchantment_effects  { Win_1_HP_white, Flight_Life_link, Count };
}
namespace Blue_enchantment_effects { 
    enum Blue_enchantment_effects   { Control_creature, Count };
}
namespace Black_enchantment_effects { 
    enum Black_enchantment_effects  { Less_HP_death_creature, Count };
}
namespace Red_enchantment_effects { 
    enum Red_enchantment_effects    { More_1_0_attack_creatures, Count };
}
namespace Green_enchantment_effects { 
    enum Green_enchantment_effects  { More_1_land, More_G_G_creature, Count };
}

/**
 * @brief return the position of elem into vec
 *
 * @tparam T ?
 * @param elem ?
 * @param vec ?
 * @return ?
 */
template <typename T>
int element_position(const T* elem, const std::vector<T*>& vec) {
    auto it = find (vec.begin(), vec.end(), elem);
    return it != vec.end() ?  it - vec.begin() : -1;
}

/**
 * @brief return true if T is an element of vec, else false
 *
 * @tparam T ?
 * @param elem ?
 * @param vec ?
 * @return ?
 */
template <typename T>
inline bool contain(const T* elem, const std::vector<T*>& vec) {
    return std::any_of(vec.begin(), vec.end(), [elem] (auto i) { return i == elem; });
    //return (std::find(vec.begin(), vec.end(), elem) != vec.end());   
}

template <typename T>
inline void remove(const T* elem, std::vector<T*>& vec) {
	vec.erase(vec.begin() + element_position(elem, vec));
}

void print_info(const std::string& s = "");
void print_list(const std::vector<std::pair<std::string, std::string>>& options, const std::string& separator = " - ");
void print_actions(const std::string& title, const std::vector<std::pair<std::string, std::string> >& options = {}, const std::string& end_message = "", bool dot_opt = true, const std::string& separator = " : ");
void cls();

enum Color {
    Reset,
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    BrightBlack,
    BrightRed,
    BrightGreen,
    BrightYellow,
    BrightBlue,
    BrightMagenta,
    BrightCyan,
    BrightWhite
};

std::string get_background_color (Color c);
std::string get_color (Color c);

std::string centered_string(std::string s, int width);

#endif
