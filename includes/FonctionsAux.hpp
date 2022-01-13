#ifndef FCT_AUX_HPP
#define FCT_AUX_HPP

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>


enum Token                {White, Blue, Black, Red, Green};

enum Ability              {Flight, Scope, Vigilance, Touch_of_death, Defender, Initiative, Double_initiative, Haste, 
                        Unblockable, Life_link, Threat, Trampling, White_protection, Blue_protection, Black_protection, Red_protection, Green_protection};

// TODO : Destroy détruit complètement la carte : delete
enum White_ritual_effects {More_3_HP, More_1_1_creature_current, Destroy_engaged_creature, Destroy_enchantment};
enum Blue_ritual_effects  {Draw_2_cards, Back_hand_creature};
enum Black_ritual_effects {Kill_creature, Kill_creature_2_power, Kill_not_angel, Less_2_2_creature_current};
enum Red_ritual_effects   {Damage_3_creature_or_player, Damage_4_creatures};
enum Green_ritual_effects {Play_another_land, Take_2_lands_library_shuffle};

// TODO : à virer
bool check_info(std::string s);

void pri();

template<typename T>
void print_card(T* c);

template<typename Base, typename T>
inline bool instanceof(const T*);

template<typename T>
int element_position(const T* elem, std::vector<T*> vec);

template<typename T>
bool contain(const T* elem, std::vector<T*> vec);

template<typename T>
std::vector<T*> vec_to_vec_pointer(std::vector<T> vec);


#endif