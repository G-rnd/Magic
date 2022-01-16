#ifndef FCT_AUX_HPP
#define FCT_AUX_HPP

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>

#include "Card.hpp"
#include "BasicCard.hpp"
#include "SpecialCard.hpp"
#include "Creature.hpp"
#include "Land.hpp"
#include "Ritual.hpp"
#include "Enchantment.hpp"

enum Card_class                 { CREATURE, LAND, RITUAL, ENCHANTEMENT };

enum Token                      { White, Blue, Black, Red, Green };

enum Ability                    { Flight, Scope, Vigilance, Touch_of_death, Defender, Initiative, Double_initiative, Haste, Unblockable,
                                    Life_link, Threat, Trampling, White_protection, Blue_protection, Black_protection, Red_protection, Green_protection };

enum Type					    { Angel };

// TODO : Destroy détruit complètement la carte : delete
enum White_ritual_effects	    { More_3_HP, More_1_1_creature_current, Destroy_engaged_creature, Destroy_enchantment };
enum Blue_ritual_effects	    { Draw_2_cards, Back_hand_creature };
enum Black_ritual_effects	    { Kill_creature, Kill_creature_2_power, Kill_not_angel, Less_2_2_creature_current };
enum Red_ritual_effects		    { Damage_3_creature_or_player, Damage_4_creatures };
enum Green_ritual_effects	    { Play_another_land, Take_2_lands_library_shuffle };

// TODO : faire les enchantements 
enum White_enchantment_effects  {Win_1_HP_white, Flight_Life_link};
enum Blue_enchantment_effects   {Control_creature};
enum Black_enchantment_effects  {Less_HP_death_creature};
enum Red_enchantment_effects    {More_1_0_attack_creatures};
enum Green_enchantment_effects  {More_1_land, More_G_G_creature};

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

#endif
