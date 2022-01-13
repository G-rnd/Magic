#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>

#include "../includes/FonctionsAux.hpp"

bool check_info(std::string s){
    return (s == "info");
}

/**
 * @brief return true T is an instance of Base, else false
 * syntax : instanceof<Base>(T);
 * 
 * @tparam Base 
 * @tparam T 
 * @return boolean
 */
template<typename Base, typename T>
bool instanceof(const T*){
    return std::is_base_of<Base, T>::value;
}

/**
 * @brief return the position of elem into vec
 * 
 * @tparam T 
 * @param elem 
 * @param vec 
 * @return auto 
 */
template<typename T>
int element_position(const T* elem, std::vector<T*> vec){
    int i = 0;
    for (int i = 0; i <= (int)vec.size(); i++){
        if(vec[i] == elem){
            return i;
        }
    }
    return i;
}

/**
 * @brief return true if T is an element of vec, else false
 * 
 * @tparam T 
 * @param elem 
 * @param vec 
 * @return true 
 * @return false 
 */
template<typename T>
bool contain(const T* elem, std::vector<T*> vec){
    if(std::find(vec.begin(), vec.end(), elem) != vec.end()){
        return true;
    }
    return false;
}

template<typename T>
void remove(const T* elem, std::vector<T*> vec){
    vec.erase(element_position(elem, vec) + vec.begin());
}

template<typename T>
std::vector<T*> vec_to_vec_pointer(std::vector<T> vec){
    std::vector<T*> pointer_vec;
    for (auto c : vec){
        pointer_vec.push_back(&c);
    }
    return pointer_vec;
}

template bool instanceof<Card, BasicCard>         (const BasicCard*);
template bool instanceof<Card, SpecialCard>       (const SpecialCard*);
template bool instanceof<Card, Creature>          (const Creature*);
template bool instanceof<Card, Land>              (const Land*);
template bool instanceof<Card, Ritual>            (const Ritual*);
template bool instanceof<Card, Enchantment>       (const Enchantment*);

template bool instanceof<BasicCard, Card>         (const Card*);
template bool instanceof<SpecialCard, Card>       (const Card*);
template bool instanceof<Creature, Card>          (const Card*);
template bool instanceof<Land, Card>              (const Card*);
template bool instanceof<Ritual, Card>            (const Card*);
template bool instanceof<Enchantment, Card>       (const Card*);
  
template bool instanceof<BasicCard, Creature>     (const Creature*);
template bool instanceof<BasicCard, Land>         (const Land*);

template bool instanceof<Creature, BasicCard>     (const BasicCard*);
template bool instanceof<Land, BasicCard>         (const BasicCard*);

template bool instanceof<SpecialCard, Ritual>     (const Ritual*);
template bool instanceof<SpecialCard, Enchantment>(const Enchantment*);

template bool instanceof<Ritual, SpecialCard>     (const SpecialCard*);
template bool instanceof<Enchantment, SpecialCard>(const SpecialCard*);


template int element_position<Card>       (const Card*, std::vector<Card*>);
template int element_position<BasicCard>  (const BasicCard*, std::vector<BasicCard*>);
template int element_position<SpecialCard>(const SpecialCard*, std::vector<SpecialCard*>);
template int element_position<Creature>   (const Creature*, std::vector<Creature*>);
template int element_position<Land>       (const Land*, std::vector<Land*>);
template int element_position<Ritual>     (const Ritual*, std::vector<Ritual*>);
template int element_position<Enchantment>(const Enchantment*, std::vector<Enchantment*>);


template bool contain<Card>               (const Card*, std::vector<Card*>);
template bool contain<BasicCard>          (const BasicCard*, std::vector<BasicCard*>);
template bool contain<SpecialCard>        (const SpecialCard*, std::vector<SpecialCard*>);
template bool contain<Creature>           (const Creature*, std::vector<Creature*>);
template bool contain<Land>               (const Land*, std::vector<Land*>);
template bool contain<Ritual>             (const Ritual*, std::vector<Ritual*>);
template bool contain<Enchantment>        (const Enchantment*, std::vector<Enchantment*>);

template void remove<Card>               (const Card*, std::vector<Card*>);
template void remove<BasicCard>          (const BasicCard*, std::vector<BasicCard*>);
template void remove<SpecialCard>        (const SpecialCard*, std::vector<SpecialCard*>);
template void remove<Creature>           (const Creature*, std::vector<Creature*>);
template void remove<Land>               (const Land*, std::vector<Land*>);
template void remove<Ritual>             (const Ritual*, std::vector<Ritual*>);
template void remove<Enchantment>        (const Enchantment*, std::vector<Enchantment*>);

template std::vector<Creature*> vec_to_vec_pointer<Creature>(std::vector<Creature>);
template std::vector<Land*> vec_to_vec_pointer<Land>(std::vector<Land>);
template std::vector<Ritual*> vec_to_vec_pointer<Ritual>(std::vector<Ritual>);
template std::vector<Enchantment*> vec_to_vec_pointer<Enchantment>(std::vector<Enchantment>);
