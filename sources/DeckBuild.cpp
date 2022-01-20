#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "DeckBuild.hpp"
#include "FonctionsAux.hpp"

int DeckBuild::get_nb_cards_current() const{
    return m_crea + m_land + m_ritu + m_ench;
}

void DeckBuild::create_file(){

    print_info("Bienvenue dans le mode création de deck");

    std::cout<< "Choisissez une nom de deck : "<<std::endl;

    std::string name;
    std::getline(std::cin, name); // TODO verif caractères

    std::string path = "data/" + name + ".txt";
    m_filename = path;

    m_file.open(m_filename, std::ios::out);
    
    while(m_nb_cards != get_nb_cards_current()){

        print_info("Vous avez créé \n - " + std::to_string(m_crea) + " créatures \n - " + std::to_string(m_land) + " terrains \n - " + std::to_string(m_ritu) + " rituels \n - " + std::to_string(m_ench) + " enchantements");

        std::cout << "crea       : pour créer une créature." << std::endl;
        std::cout << "land       : pour créer un terrain." << std::endl;
        std::cout << "ritu       : pour créer un rituel." << std::endl;
        std::cout << "ench       : pour créer un enchantement." << std::endl << std::endl;

        std::cout << "Choississez votre type de carte : " << std::endl;

        bool quit = false;
        std::string card;

        while(!quit){
            std::getline(std::cin, card);
            if(card == "crea"){
                create_creature();
                m_crea += 1;
                quit = true;
            } else if(card == "land"){
                create_land();
                m_land += 1;
                quit = true;
            } else if(card == "ritu"){
                create_ritual();
                m_ritu += 1;
                quit = true;
            } else if(card == "ench"){
                create_enchantment();
                m_ench += 1;
                quit = true;
            } else{
                print_info("Commande invalide.");
            }

        }

    }

    print_info("Votre deck est complet, il a été créé avec succès !");
    m_file.close();

}

void DeckBuild::create_creature(){
    
    // Begin
    m_file << "#BEGIN_CARD" <<std::endl;
    m_file << "    #TYPE_CARD: Creature" <<std::endl;

    // Name
    std::cout<< "Entrez le nom de votre créature : " <<std::endl;

    std::string name;
    std::getline(std::cin, name);

    m_file << "    #NAME: " + name <<std::endl;

    std::cout<<std::endl<<std::endl;;

    // Token
    std::cout << "0       : Créature blanche." << std::endl;
    std::cout << "1       : Créature bleue." << std::endl;
    std::cout << "2       : Créature noire." << std::endl;
    std::cout << "3       : Créature rouge." << std::endl;
    std::cout << "4       : Créature verte." << std::endl << std::endl;

    std::cout<< "Entrez la couleur de votre créature : " <<std::endl;

    int token;
    bool quit = false;

    while(!quit){
        std::cin>> token;
        if(token < 5 && token >= 0 ){
            quit = true;
        } else{
            print_info("Commande invalide.");
        }
    }

    m_file << "    #TOKEN: " + std::to_string(token) <<std::endl << std::endl;

    std::cout<<std::endl<<std::endl;;

    // Power
    std::cout<< "Entrez la force de votre créature : " <<std::endl;

    int power;
    std::cin>> power;

    m_file << "    #POWER: " + std::to_string(power) <<std::endl;

    std::cout<<std::endl<<std::endl;;

    // Toughness
    std::cout<< "Entrez l'endurance de votre créature : " <<std::endl;

    int toughness;
    std::cin>> toughness;

    m_file << "    #TOUGHNESS: " + std::to_string(toughness) <<std::endl;

    std::cout<<std::endl<<std::endl;;

    // Abilities
    std::cout << "0       : Vol." << std::endl;
    std::cout << "1       : Portée." << std::endl;
    std::cout << "2       : Vigilance." << std::endl;
    std::cout << "3       : Toucher de mort." << std::endl;
    std::cout << "4       : Défenseur." << std::endl;
    std::cout << "5       : Initiative." << std::endl;
    std::cout << "6       : Double initiative." << std::endl;
    std::cout << "7       : Hâte." << std::endl;
    std::cout << "8       : Imblocable." << std::endl;
    std::cout << "9       : Lien de vie." << std::endl;
    std::cout << "10      : Menace." << std::endl;
    std::cout << "11      : Piétinement." << std::endl;
    std::cout << "12      : Protection contre blanc." << std::endl;
    std::cout << "13      : Protection contre bleu." << std::endl;
    std::cout << "14      : Protection contre noir." << std::endl;
    std::cout << "15      : Protection contre rouge." << std::endl;
    std::cout << "16      : Protection contre vert." << std::endl <<std::endl;

    std::cout << "valid   : valider vos choix." << std::endl;
    std::cout << "reset   : réinitialiser vos choix." << std::endl<<std::endl;

    std::cout<< "Entrez les capacités de votre créature : " <<std::endl;

    std::string cmd2;
    bool quit2 = false;
    std::vector<int> abilities;
    bool add_protection = false;

    while (!quit2) {
        std::getline(std::cin, cmd2);

        if (cmd2.find("valid") != std::string::npos) {
            quit2 = true;
        } else if (cmd2.find("reset") != std::string::npos) {
            abilities = {};
            std::cout<< "Reset reussi" <<std::endl;
        } else {
            try {
                int num = std::stoi(cmd2);
                if (num > 16 || num < 0) {
                    std::cout << "Id invalide" << std::endl;

                    std::cout << "Entrée pour continuer." << std::endl;
                    std::getline(std::cin, cmd2);
                } else if (std::find(abilities.begin(), abilities.end(), num) != abilities.end()) {
                    std::cout << num << " déjà ajouté." << std::endl;

                    std::cout << "Entrée pour continuer." << std::endl;
                    std::getline(std::cin, cmd2);
                } else if ((num > 11 && num < 17) && add_protection ) {
                    std::cout << "Protection contre une couleur déjà ajoutée." << std::endl;

                    std::cout << "Entrée pour continuer." << std::endl;
                    std::getline(std::cin, cmd2);
                } else{
                    if((num > 11 && num < 17)){
                        add_protection = true;
                    }
                    abilities.push_back(num);
                }
            } catch (std::invalid_argument &e) {
                std::cout << "Commande Invalide" << std::endl; //TODO : s'affiche automatiquement
            }
        }
    }

    std::string str_abilities = "";
    for (auto a : abilities){
        if(a == abilities[abilities.size()-1]){
            str_abilities = str_abilities + std::to_string(a);
        } else {
            str_abilities = str_abilities + std::to_string(a) + ", ";
        }
    }

    m_file << "    #ABILITIES: " + str_abilities <<std::endl << std::endl;  

    std::cout<<std::endl<<std::endl;;

    // Types
    // TODO : remplir types
    std::cout << "0       : ." << std::endl;
    std::cout << "1       : ." << std::endl;
    std::cout << "2       : ." << std::endl;
    std::cout << "3       : ." << std::endl;
    std::cout << "4       : ." << std::endl;
    std::cout << "5       : ." << std::endl;
    std::cout << "6       : ." << std::endl;
    std::cout << "7       : ." << std::endl;
    std::cout << "8       : ." << std::endl;
    std::cout << "9       : ." << std::endl;
    std::cout << "10      : ." << std::endl;
    std::cout << "11      : ." << std::endl;
    std::cout << "12      : ." << std::endl;
    std::cout << "13      : ." << std::endl;
    std::cout << "14      : ." << std::endl;
    std::cout << "15      : ." << std::endl <<std::endl;

    std::cout << "valid   : valider vos choix." << std::endl;
    std::cout << "reset   : réinitialiser vos choix." << std::endl << std::endl;;

    std::cout<< "Entrez les types de votre créature : " <<std::endl;

    std::string cmd3;
    bool quit3 = false;
    std::vector<int> types;

    while (!quit3) {
        std::getline(std::cin, cmd3);

        if (cmd3.find("valid") != std::string::npos) {
            quit3 = true;
        } else if (cmd3.find("reset") != std::string::npos) {
            types = {};
            std::cout<< "Reset reussi" <<std::endl;
        } else {
            try {
                int num = std::stoi(cmd3);
                if (num > 15 || num < 0) {
                    std::cout << "Id invalide" << std::endl;

                    std::cout << "Entrée pour continuer." << std::endl;
                    std::getline(std::cin, cmd3);
                } else if (std::find(types.begin(), types.end(), num) != types.end()) {
                    std::cout << num << " déjà ajouté." << std::endl;

                    std::cout << "Entrée pour continuer." << std::endl;
                    std::getline(std::cin, cmd3);
                } else{
                    types.push_back(num);
                }
            } catch (std::invalid_argument &e) {
                std::cout << "Commande Invalide" << std::endl;    
            }
        }
    }

    std::string str_types = "";
    for (auto t : types){
        if(t == types[types.size()-1]){
            str_types = str_types + std::to_string(t);
        } else {
            str_types = str_types + std::to_string(t) + ", ";
        }
    }

    m_file << "    #TYPES: " + str_types <<std::endl << std::endl;

    std::cout<<std::endl<<std::endl;;

    // Cost
    std::cout<< "Entrez le coût en terrain blanc : " <<std::endl;

    int cost_white;
    std::cin>> cost_white;

    std::cout<< "Entrez le coût en terrain bleu : " <<std::endl;

    int cost_blue;
    std::cin>> cost_blue;

    std::cout<< "Entrez le coût en terrain noir : " <<std::endl;

    int cost_black;
    std::cin>> cost_black;

    std::cout<< "Entrez le coût en terrain rouge : " <<std::endl;

    int cost_red;
    std::cin>> cost_red;

    std::cout<< "Entrez le coût en terrain vert : " <<std::endl;

    int cost_green;
    std::cin>> cost_green;

    std::cout<< "Entrez le coût en terrain de n'importe quelle couleur : " <<std::endl;

    int cost_any;
    std::cin>> cost_any;

    std::string cost;
    cost = std::to_string(cost_any) + ", " + std::to_string(cost_white) + ", " + std::to_string(cost_blue) + ", " + std::to_string(cost_black) + ", " + std::to_string(cost_red) + ", " + std::to_string(cost_green);

    m_file << "    #COST: " + cost <<std::endl;

    // End
    m_file << "#END_CARD" <<std::endl<<std::endl;

}

void DeckBuild::create_land(){
    
    // Begin
    m_file << "#BEGIN_CARD" <<std::endl;
    m_file << "    #TYPE_CARD: Land" <<std::endl;

    // Name
    std::cout<< "Entrez le nom de votre terrain : " <<std::endl;

    std::string name;
    std::getline(std::cin, name);

    m_file << "    #NAME: " + name <<std::endl;

    std::cout<<std::endl<<std::endl;;

    // Token
    std::cout << "0       : Terrain blanche." << std::endl;
    std::cout << "1       : Terrain bleue." << std::endl;
    std::cout << "2       : Terrain noire." << std::endl;
    std::cout << "3       : Terrain rouge." << std::endl;
    std::cout << "4       : Terrain verte." << std::endl << std::endl;

    std::cout<< "Entrez la couleur de votre terrain : " <<std::endl;

    int token;
    bool quit = false;

    while(!quit){
        std::cin>> token;
        if(token < 5 && token >= 0 ){
            quit = true;
        } else{
            print_info("Commande invalide.");
        }
    }

    m_file << "    #TOKEN: " + std::to_string(token) <<std::endl << std::endl;

    m_file << "    #VALUE: 1" <<std::endl;

    m_file << "#END_CARD" <<std::endl << std::endl;

}

void DeckBuild::create_ritual(){

/*
#BEGIN_CARD
	#TYPE_CARD: Ritual
	#NAME: Spirit Lands
	#TOKEN: 4

	#EFFECTS: 1
	#COST: 1, 1, 0, 0, 0, 0
	#INFO: Take 2 lands in your library and shuffle your library
#END_CARD
*/

    // Begin
    m_file << "#BEGIN_CARD" <<std::endl;
    m_file << "    #TYPE_CARD: Ritual" <<std::endl;

    // Name
    std::cout<< "Entrez le nom de votre rituel : " <<std::endl;

    std::string name;
    std::getline(std::cin, name);

    m_file << "    #NAME: " + name <<std::endl;

    std::cout<<std::endl<<std::endl;;

    // Token
    std::cout << "0       : Terrain blanche." << std::endl;
    std::cout << "1       : Terrain bleue." << std::endl;
    std::cout << "2       : Terrain noire." << std::endl;
    std::cout << "3       : Terrain rouge." << std::endl;
    std::cout << "4       : Terrain verte." << std::endl << std::endl;

    std::cout<< "Entrez la couleur de votre rituel : " <<std::endl;

    int token;
    bool quit = false;

    while(!quit){
        std::cin>> token;
        if(token < 5 && token >= 0 ){
            quit = true;
        } else{
            print_info("Commande invalide.");
        }
    }

    m_file << "    #TOKEN: " + std::to_string(token) <<std::endl << std::endl;

    // Effects
    switch (token)
    {
    case 0:{

        std::cout << "0       : Gagne 3 points de vie." << std::endl;
        std::cout << "1       : Ajouter 1 / 1 à une créature pour un tour." << std::endl;
        std::cout << "2       : Détruire une créature engagée adverse." << std::endl;
        std::cout << "3       : Détruire un enchantement engagé adverse." << std::endl;

        std::cout << "valid   : valider vos choix." << std::endl;
        std::cout << "reset   : réinitialiser vos choix." << std::endl<<std::endl;

        std::cout<< "Entrez les effets de votre créature : " <<std::endl;

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                quit = true;
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                std::cout<< "Reset reussi" <<std::endl;
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 3 || num < 0) {
                        std::cout << "Id invalide" << std::endl;

                        std::cout << "Entrée pour continuer." << std::endl;
                        std::getline(std::cin, cmd);
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        std::cout << num << " déjà ajouté." << std::endl;

                        std::cout << "Entrée pour continuer." << std::endl;
                        std::getline(std::cin, cmd);
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    std::cout << "Commande Invalide" << std::endl; //TODO : s'affiche automatiquement
                }
            }
        }

        std::string str_effects = "";
        for (auto e : effects){
            if(e == effects[effects.size()-1]){
                str_effects = str_effects + std::to_string(e);
            } else {
                str_effects = str_effects + std::to_string(e) + ", ";
            }
        }

        m_file << "    #EFFECTS: " + str_effects <<std::endl << std::endl;  
    }
        
    break;

    case 1:{

        std::cout << "0       : Piocher 2 cartes." << std::endl;
        std::cout << "1       : Retourner une créature jouée adverse dans sa main." << std::endl;

        std::cout << "valid   : valider vos choix." << std::endl;
        std::cout << "reset   : réinitialiser vos choix." << std::endl<<std::endl;

        std::cout<< "Entrez les effets de votre créature : " <<std::endl;

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                quit = true;
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                std::cout<< "Reset reussi" <<std::endl;
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 1 || num < 0) {
                        std::cout << "Id invalide" << std::endl;

                        std::cout << "Entrée pour continuer." << std::endl;
                        std::getline(std::cin, cmd);
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        std::cout << num << " déjà ajouté." << std::endl;

                        std::cout << "Entrée pour continuer." << std::endl;
                        std::getline(std::cin, cmd);
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    std::cout << "Commande Invalide" << std::endl; //TODO : s'affiche automatiquement
                }
            }
        }

        std::string str_effects = "";
        for (auto e : effects){
            if(e == effects[effects.size()-1]){
                str_effects = str_effects + std::to_string(e);
            } else {
                str_effects = str_effects + std::to_string(e) + ", ";
            }
        }

        m_file << "    #EFFECTS: " + str_effects <<std::endl << std::endl;  

    }
    break;

    case 2:{

        std::cout << "0       : Tuer une créature adverse." << std::endl;
        std::cout << "1       : Tuer une créature adverse ayant plus de 2 de force." << std::endl;
        std::cout << "2       : Tuer une créature avderse, hors Ange." << std::endl;
        std::cout << "3       : Tuer une créature adverse ayant moins de 2 / 2 à ce tour." << std::endl;

        std::cout << "valid   : valider vos choix." << std::endl;
        std::cout << "reset   : réinitialiser vos choix." << std::endl<<std::endl;

        std::cout<< "Entrez les effets de votre créature : " <<std::endl;

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                quit = true;
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                std::cout<< "Reset reussi" <<std::endl;
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 3 || num < 0) {
                        std::cout << "Id invalide" << std::endl;

                        std::cout << "Entrée pour continuer." << std::endl;
                        std::getline(std::cin, cmd);
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        std::cout << num << " déjà ajouté." << std::endl;

                        std::cout << "Entrée pour continuer." << std::endl;
                        std::getline(std::cin, cmd);
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    std::cout << "Commande Invalide" << std::endl; //TODO : s'affiche automatiquement
                }
            }
        }

        std::string str_effects = "";
        for (auto e : effects){
            if(e == effects[effects.size()-1]){
                str_effects = str_effects + std::to_string(e);
            } else {
                str_effects = str_effects + std::to_string(e) + ", ";
            }
        }

        m_file << "    #EFFECTS: " + str_effects <<std::endl << std::endl;  

    }
    break;

    case 3:{

        std::cout << "0       : Infliger 3 dégâts à une créature adverse ou à l'adervsaire." << std::endl;
        std::cout << "1       : Infliger 4 dégâts à une créature adverse." << std::endl;

        std::cout << "valid   : valider vos choix." << std::endl;
        std::cout << "reset   : réinitialiser vos choix." << std::endl<<std::endl;

        std::cout<< "Entrez les effets de votre créature : " <<std::endl;

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                quit = true;
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                std::cout<< "Reset reussi" <<std::endl;
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 1 || num < 0) {
                        std::cout << "Id invalide" << std::endl;

                        std::cout << "Entrée pour continuer." << std::endl;
                        std::getline(std::cin, cmd);
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        std::cout << num << " déjà ajouté." << std::endl;

                        std::cout << "Entrée pour continuer." << std::endl;
                        std::getline(std::cin, cmd);
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    std::cout << "Commande Invalide" << std::endl; //TODO : s'affiche automatiquement
                }
            }
        }

        std::string str_effects = "";
        for (auto e : effects){
            if(e == effects[effects.size()-1]){
                str_effects = str_effects + std::to_string(e);
            } else {
                str_effects = str_effects + std::to_string(e) + ", ";
            }
        }

        m_file << "    #EFFECTS: " + str_effects <<std::endl << std::endl;  

    }
    break;

    case 4:{


        std::cout << "0       : Jouer un autre terrain ce tour-ci." << std::endl;
        std::cout << "1       : Prendre 2 terrains dans la bibliothèque et la mélanger." << std::endl;

        std::cout << "valid   : valider vos choix." << std::endl;
        std::cout << "reset   : réinitialiser vos choix." << std::endl<<std::endl;

        std::cout<< "Entrez les effets de votre créature : " <<std::endl;

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                quit = true;
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                std::cout<< "Reset reussi" <<std::endl;
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 1 || num < 0) {
                        std::cout << "Id invalide" << std::endl;

                        std::cout << "Entrée pour continuer." << std::endl;
                        std::getline(std::cin, cmd);
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        std::cout << num << " déjà ajouté." << std::endl;

                        std::cout << "Entrée pour continuer." << std::endl;
                        std::getline(std::cin, cmd);
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    std::cout << "Commande Invalide" << std::endl; //TODO : s'affiche automatiquement
                }
            }
        }

        std::string str_effects = "";
        for (auto e : effects){
            if(e == effects[effects.size()-1]){
                str_effects = str_effects + std::to_string(e);
            } else {
                str_effects = str_effects + std::to_string(e) + ", ";
            }
        }

        m_file << "    #EFFECTS: " + str_effects <<std::endl;  

    }
    break;
    
    default:
        break;
    }

    // Cost
    std::cout<< "Entrez le coût en terrain blanc : " <<std::endl;

    int cost_white;
    std::cin>> cost_white;

    std::cout<< "Entrez le coût en terrain bleu : " <<std::endl;

    int cost_blue;
    std::cin>> cost_blue;

    std::cout<< "Entrez le coût en terrain noir : " <<std::endl;

    int cost_black;
    std::cin>> cost_black;

    std::cout<< "Entrez le coût en terrain rouge : " <<std::endl;

    int cost_red;
    std::cin>> cost_red;

    std::cout<< "Entrez le coût en terrain vert : " <<std::endl;

    int cost_green;
    std::cin>> cost_green;

    std::cout<< "Entrez le coût en terrain de n'importe quelle couleur : " <<std::endl;

    int cost_any;
    std::cin>> cost_any;

    std::string cost;
    cost = std::to_string(cost_any) + ", " + std::to_string(cost_white) + ", " + std::to_string(cost_blue) + ", " + std::to_string(cost_black) + ", " + std::to_string(cost_red) + ", " + std::to_string(cost_green);

    m_file << "    #COST: " + cost <<std::endl;

    // Info
    std::cout<< "Entrez les informations de votre rituel: " <<std::endl;

    std::string infos;
    std::getline(std::cin, infos);

    m_file << "    #INFO: " + infos <<std::endl; //TODO : ne laisse pas le temps de taper


    m_file << "#END_CARD" <<std::endl << std::endl;
    
}

void DeckBuild::create_enchantment(){
    


}