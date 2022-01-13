#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>

#include "../includes/FonctionsAux.hpp"

bool check_info(std::string s){
    return (s == "info");
}

void pri(){
    std::cout<<"coucou"<<std::endl;
}

template<typename T>
void print_card(T* c){
    c->print();
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
int element_position(const T* elem, std::vector<T*> vec){
    for (int i = 0; i <= vec.size(); i++){
        if(vec[i] == elem){
            return i;
        }
    }
    //return std::find(vec.begin(), vec.end(), elem);
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
std::vector<T*> vec_to_vec_pointer(std::vector<T> vec){
    std::vector<T*> pointer_vec;
    for (auto c : vec){
        pointer_vec.push_back(&c);
    }
    return pointer_vec;
}