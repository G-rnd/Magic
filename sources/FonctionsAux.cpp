#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

enum Ability     {Flight, Scope, Vigilance, Touch_of_death, Defender, Initiative, Double_initiative, Haste, 
                 Unblockable, Life_link, Menace, Trampling, Protection_against_color};

/**
 * @brief return true T is an instance of Base, else false
 * syntax : instanceof<Base>(T);
 * 
 * @tparam Base 
 * @tparam T 
 * @return boolean
 */
template<typename Base, typename T>
inline bool instanceof(const T*){
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
auto element_position(const T* elem, std::vector<T*> vec){
    return std::find(vec.begin(), vec.end(), elem);
}

template<typename T>
bool contain(const T* elem, std::vector<T*> vec){
    if(std::find(vec.begin(), vec.end(), elem) != vec.end()){
        return true;
    }
    return false;
}