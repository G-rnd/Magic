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

enum Token					{ White, Blue, Black, Red, Green };

enum Ability				{ Flight, Scope, Vigilance, Touch_of_death, Defender, Initiative, Double_initiative, Haste,
								Unblockable, Life_link, Threat, Trampling, White_protection, Blue_protection, Black_protection, Red_protection, Green_protection };

enum Type					{ Angel };

// TODO : Destroy détruit complètement la carte : delete
enum White_ritual_effects	{ More_3_HP, More_1_1_creature_current, Destroy_engaged_creature, Destroy_enchantment };
enum Blue_ritual_effects	{ Draw_2_cards, Back_hand_creature };
enum Black_ritual_effects	{ Kill_creature, Kill_creature_2_power, Kill_not_angel, Less_2_2_creature_current };
enum Red_ritual_effects		{ Damage_3_creature_or_player, Damage_4_creatures };
enum Green_ritual_effects	{ Play_another_land, Take_2_lands_library_shuffle };

template <typename T>
void print_card(T* c)
{

}

/**
 * @brief return true T is an instance of Base, else false
 *
 * @tparam Base ?
 * @tparam T ?
 * @return ?
 */
template <class Base, typename T>
inline bool instanceof(const T*)
{
	return std::is_base_of<Base, T>::value;
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
int element_position(const T* elem, const std::vector<T*>& vec)
{
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
inline bool contain(const T* elem, const std::vector<T*>& vec)
{
    std::cout << std::any_of(vec.begin(), vec.end(), [elem] (auto i) {return i == elem;}) << std::endl;
}

template <typename T>
inline void remove(const T* elem, std::vector<T*>& vec)
{
	vec.erase(vec.begin() + element_position(elem, vec));
}

#endif
