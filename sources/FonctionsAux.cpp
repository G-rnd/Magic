#include <string>

#include "FonctionsAux.hpp"
/*
    Affiche une chaîne de caractères s au format suivant :

    --------------------------------------------------------

    Lorem ipsum dolor sit amet, consectetur adipiscing elit.

    --------------------------------------------------------


    <Entrée> pour continuer
*/
void print_info(const std::string& s)  {

    std::string padding(s.size(), '-');
    std::cout << std::endl << padding << std::endl << std::endl;

    std::cout << s << std::endl << std::endl;

    std::cout << padding << std::endl << std::endl;
    
    std::cout << std::endl << "<Entrée> pour continuer." << std::endl;

    std::string cmd;
    std::getline(std::cin, cmd);
}