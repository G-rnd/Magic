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

    cls();

    print_info("Bienvenue dans le mode création de deck");

    // Load a deck or create a new deck

    std::string path = "data/in_construction";
    std::string name;

    while (true) {
        cls();

        print_actions("Selectionnez ce que vous souhaitez faire : ",{
        {"new", "pour créer un nouveau deck."}, 
        {"load", "pour charger un commencé."} });

        std::string cmd;
        std::cin >> cmd;

        if(cmd == "new"){
            cls();
            print_actions("Choisissez une nom de deck : ");

            std::cin.clear();
            std::getline(std::cin, name);

            path = "data/in_construction" + name + ".txt";
            m_filename = path;
            break;
        } else if(cmd == "load"){
            cls();

            std::vector<std::pair<std::string, std::string > > available_decks = {};

            for (const auto & file : std::filesystem::directory_iterator(path))
                available_decks.push_back({"", (file.path()).string().substr(path.size())});
            for(size_t i = 0; i < available_decks.size(); i++) {
                available_decks[i].first = (available_decks.size() > 10) ? ((i/10 == 1) ? " " : "") : "" + std::to_string(i);
            }

            if (available_decks.size() == 0) {
                cls();
                print_info("Erreur : Aucun deck n'est disponible !");
                return;
            }

            print_actions("Sélectionnez votre deck à compléter : ",{
            {"crea", "pour créer une créature."}, 
            {"land", "pour créer un terrain."},
            {"ritu", "pour créer un rituel."},
            {"ench", "pour créer un enchantement."},
            {"save", "pour enregistrer l'état actuel du deck et le continuer plus tard."} }, "Choississez votre type de carte : ");

            break;
        } else {
            print_info("Commande invalide.");
        }

    }

    std::ofstream flux(m_filename.c_str(), std::ios::app);

    m_file.open(m_filename, std::ios::app);

    bool saved = false;
    
    while(m_nb_cards != get_nb_cards_current() || saved){

        bool quit = false;
        std::string card;

        while(!quit){

            cls();

            print_info("Vous avez créé \n - " + std::to_string(m_crea) + " créatures \n - " + std::to_string(m_land) + " terrains \n - " + std::to_string(m_ritu) + " rituels \n - " + std::to_string(m_ench) + " enchantements");

            print_actions("Sélectionnez votre type de carte : ",{
            {"crea", "pour créer une créature."}, 
            {"land", "pour créer un terrain."},
            {"ritu", "pour créer un rituel."},
            {"ench", "pour créer un enchantement."},
            {"save", "pour enregistrer l'état actuel du deck et le continuer plus tard."} }, "Choississez votre type de carte : ");

            std::cin.clear();
            std::getline(std::cin, card);
            if(card == "crea"){
                cls();
                print_actions("Créez une créature ! ");
                create_creature();
                m_crea += 1;
                quit = true;
            } else if(card == "land"){
                cls();
                print_actions("Créez un terrain ! ");
                create_land();
                m_land += 1;
                quit = true;
            } else if(card == "ritu"){
                cls();
                print_actions("Créez un rituel ! ");
                create_ritual();
                m_ritu += 1;
                quit = true;
            } else if(card == "ench"){
                cls();
                print_actions("Créez un enchantement ! ");
                create_enchantment();
                m_ench += 1;
                quit = true;
            } else if(card == "save"){
                quit = true;
                saved = true;
            } else{
                print_info("Commande invalide.");
            }

        }

    }

    if(saved){
        print_info("Votre deck " + name + " de " + std::to_string(get_nb_cards_current()) + " est sauvegardé avec succès !");
        m_file.close();
    } else {
        print_info("Votre deck " + name + " est complet, il a été créé avec succès !");
        std::string new_name = "data/complet/" + name + ".txt";
        rename(m_filename.c_str(), new_name.c_str());
        m_file.close();
    }

}

void DeckBuild::create_creature(){
    
    // Begin
    m_file << "#BEGIN_CARD" <<std::endl;
    m_file << "    #TYPE_CARD: Creature" <<std::endl;

    // Name
    std::cout<< std::endl << "Entrez le nom de votre créature : " <<std::endl;

    std::string name;
    std::cin.clear();
    std::getline(std::cin, name);

    m_file << "    #NAME: " + name <<std::endl;

    std::cout<<std::endl<<std::endl;;

    // Token
    print_list({
        {std::to_string(0), "Créature blanche"},
        {std::to_string(1), "Créature bleue"},
        {std::to_string(2), "Créature noire"},
        {std::to_string(3), "Créature rouge"},
        {std::to_string(4), "Créature verte"},
    }, " - ");

    std::cout<< std::endl << "Entrez la couleur de votre créature : " <<std::endl;

    int token;
    bool quit = false;

    while(!quit){
        std::cin>> token;
        std::cin.ignore();
        if(token < 5 && token >= 0 ){
            quit = true;
        } else{
            print_info("Commande invalide.");
        }
    }

    m_file << "    #TOKEN: " + std::to_string(token) <<std::endl << std::endl;

    std::cout<<std::endl;

    // Power
    std::cout<< "Entrez la force de votre créature : " <<std::endl;

    int power;
    std::cin>> power;
    std::cin.ignore();

    m_file << "    #POWER: " + std::to_string(power) <<std::endl;

    std::cout<<std::endl;

    // Toughness
    std::cout<< "Entrez l'endurance de votre créature : " <<std::endl;

    int toughness;
    std::cin>> toughness;
    std::cin.ignore();

    m_file << "    #TOUGHNESS: " + std::to_string(toughness) <<std::endl;

    std::cout<<std::endl;

    // Abilities
    print_list({
    {"0 ", "Vol."}, 
    {"1 ", "Portée."},
    {"2 ", "Vigilance."},
    {"3 ", "Toucher de mort."},
    {"4 ", "Défenseur."},
    {"5 ", "Initiative."},
    {"6 ", "Double initiative."},
    {"7 ", "Hâte."},
    {"8 ", "Imblocable."},
    {"9 ", "Lien de vie."},
    {"10", "Menace."},
    {"11", "Piétinement."},
    {"12", "Protection contre blanc."},
    {"13", "Protection contre bleu."}, 
    {"14", "Protection contre noir."},
    {"15", "Protection contre rouge."},
    {"16", "Protection contre vert."}}, " - ");

    print_actions("Choix capacités, cf. ci-dessus", {
    {"valid", "valider vos choix"}, 
    {"reset", "réinitialiser vos choix"}}, "Entrez les capacités de votre créature : " );
    
    std::string cmd2;
    bool quit2 = false;
    std::vector<int> abilities;
    bool add_protection = false;

    while (!quit2) {
        std::cin.clear();
        std::getline(std::cin, cmd2);

        if (cmd2.find("valid") != std::string::npos) {
            quit2 = true;
        } else if (cmd2.find("reset") != std::string::npos) {
            abilities = {};
            print_info("Reset reussi !");
        } else {
            try {
                int num = std::stoi(cmd2);
                if (num > 16 || num < 0) {
                    print_info("Id invalide.");
                } else if (std::find(abilities.begin(), abilities.end(), num) != abilities.end()) {
                    print_info(std::to_string(num) + " déjà ajouté.");
                } else if ((num > 11 && num < 17) && add_protection ) {
                    print_info("Protection contre une couleur déjà ajoutée.");
                } else{
                    if((num > 11 && num < 17)){
                        add_protection = true;
                    }
                    abilities.push_back(num);
                }
            } catch (std::invalid_argument &e) {
                print_info("Commande invalide."); 
            }
        }
    }

    std::string str_abilities = "";
    for (auto a : abilities){
        str_abilities.append(" ");
        if(a == abilities[abilities.size()-1]){
            str_abilities = str_abilities + std::to_string(a);
        } else {
            str_abilities = str_abilities + std::to_string(a) + ", ";
        }
    }

    m_file << "    #ABILITIES:" + str_abilities <<std::endl << std::endl;  

    std::cout<<std::endl<<std::endl;

    // Types
    // TODO : remplir types
    print_list({
    {"0 ", "."}, 
    {"1 ", "."},
    {"2 ", "."},
    {"3 ", "  ."},
    {"4 ", "."},
    {"5 ", "."},
    {"6 ", " ."},
    {"7 ", "."},
    {"8 ", "."},
    {"9 ", "  ."},
    {"10", "."},
    {"11", "."},
    {"12", "."},
    {"13", "."}, 
    {"14", "  ."},
    {"15", "  ."},
    {"16", "  ."}}, " - ");

    print_actions("Choix types, cf. ci-dessus", {
    {"valid", "valider vos choix"}, 
    {"reset", "réinitialiser vos choix"}}, "Entrez les types de votre créature : " );

    std::string cmd3;
    bool quit3 = false;
    std::vector<int> types;

    while (!quit3) {
        std::cin.clear();
        std::getline(std::cin, cmd3);

        if (cmd3.find("valid") != std::string::npos) {
            if(types.empty()){
                print_info("Votre créature doit avoir au moins un type ! ");
            } else {
                quit3 = true;
            }
        } else if (cmd3.find("reset") != std::string::npos) {
            types = {};
            print_info("Reset reussi !");
        } else {
            try {
                int num = std::stoi(cmd3);
                if (num > 15 || num < 0) {
                    print_info("Id invalide.");
                } else if (std::find(types.begin(), types.end(), num) != types.end()) {
                    print_info(std::to_string(num) + " déjà ajouté.");
                } else{
                    types.push_back(num);
                }
            } catch (std::invalid_argument &e) {
                print_info("Commande invalide.");   
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

    std::cout<<std::endl<<std::endl;

    // Cost
    std::cout<< "Entrez le coût en terrain blanc : " <<std::endl;

    int cost_white;
    std::cin>> cost_white;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain bleu : " <<std::endl;

    int cost_blue;
    std::cin>> cost_blue;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain noir : " <<std::endl;

    int cost_black;
    std::cin>> cost_black;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain rouge : " <<std::endl;

    int cost_red;
    std::cin>> cost_red;
    std::cin.ignore();

    std::cout<< std::endl <<"Entrez le coût en terrain vert : " <<std::endl;

    int cost_green;
    std::cin>> cost_green;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain de n'importe quelle couleur : " <<std::endl;

    int cost_any;
    std::cin>> cost_any;
    std::cin.ignore();

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
    std::cout<< std::endl <<  "Entrez le nom de votre terrain : " <<std::endl;

    std::string name;
    std::cin.clear();
    std::getline(std::cin, name);

    m_file << "    #NAME: " + name <<std::endl;

    std::cout<<std::endl<<std::endl;;

    // Token
    print_list({
        {std::to_string(0), "Terrain blanc"},
        {std::to_string(1), "Terrain bleu"},
        {std::to_string(2), "Terrain noir"},
        {std::to_string(3), "Terrain rouge"},
        {std::to_string(4), "Terrain vert"},
    }, " - ");

    std::cout<< std::endl << "Entrez la couleur de votre terrain : " <<std::endl;

    int token;
    bool quit = false;

    while(!quit){
        std::cin>> token;
        std::cin.ignore();
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

    // Begin
    m_file << "#BEGIN_CARD" <<std::endl;
    m_file << "    #TYPE_CARD: Ritual" <<std::endl;

    // Name
    std::cout<< std::endl <<  "Entrez le nom de votre rituel : " <<std::endl;

    std::string name;
    std::cin.clear();
    std::getline(std::cin, name);

    m_file << "    #NAME: " + name <<std::endl;

    std::cout<<std::endl;

    // Token
    print_list({
        {std::to_string(0), "Rituel blanc"},
        {std::to_string(1), "Rituel bleu"},
        {std::to_string(2), "Rituel noir"},
        {std::to_string(3), "Rituel rouge"},
        {std::to_string(4), "Rituel vert"},
    }, " - ");

    std::cout<< std::endl << "Entrez la couleur de votre rituel : " <<std::endl;

    int token;
    bool quit = false;

    std::cout<<std::endl;

    while(!quit){
        std::cin>> token;
        std::cin.ignore();
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

        print_list({
        {"0", "Gagne 3 points de vie."},
        {"1", "Ajouter 1 / 1 à une créature pour un tour."},
        {"2", "Détruire une créature engagée adverse."},
        {"3", "Détruire un enchantement engagé adverse."} });

        print_actions("Choix effets, cf. ci-dessus", {
        {"valid", "valider vos choix"}, 
        {"reset", "réinitialiser vos choix"}}, "Entrez les effets de votre rituel : " );

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::cin.clear();
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                if(effects.empty()){
                    print_info("Votre rituel doit avoir au moins un effet");
                } else{
                    quit = true;
                }
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                print_info("Reset reussi !");
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 3 || num < 0) {
                        print_info("Id invalide.");
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        print_info(std::to_string(num) + " déjà ajouté.");
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    print_info("Commande invalide."); 
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


        print_list({
        {"0", "Piocher 2 cartes."},
        {"1", "Retourner une créature jouée adverse dans sa main."} });

        print_actions("Choix effets, cf. ci-dessus", {
        {"valid", "valider vos choix"}, 
        {"reset", "réinitialiser vos choix"}}, "Entrez les effets de votre rituel : " );

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::cin.clear();
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                quit = true;
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                print_info("Reset reussi !");
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 1 || num < 0) {
                        print_info("Id invalide.");
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        print_info(std::to_string(num) + " déjà ajouté.");
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    print_info("Commande invalide."); 
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

        print_list({
        {"0", "Tuer une créature adverse"},
        {"1", "ATuer une créature adverse ayant plus de 2 de force."},
        {"2", "Tuer une créature avderse, hors Ange."},
        {"3", "Tuer une créature adverse ayant moins de 2 / 2 à ce tour."} });

        print_actions("Choix effets, cf. ci-dessus", {
        {"valid", "valider vos choix"}, 
        {"reset", "réinitialiser vos choix"}}, "Entrez les effets de votre rituel : " );

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::cin.clear();
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                quit = true;
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                print_info("Reset reussi !");
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 3 || num < 0) {
                        print_info("Id invalide.");
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        print_info(std::to_string(num) + " déjà ajouté.");
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    print_info("Commande invalide."); 
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

        print_list({
        {"0", "Infliger 3 dégâts à une créature adverse ou à l'adervsaire."},
        {"1", "Infliger 4 dégâts à une créature adverse."} });

        print_actions("Choix effets, cf. ci-dessus", {
        {"valid", "valider vos choix"}, 
        {"reset", "réinitialiser vos choix"}}, "Entrez les effets de votre rituel : " );

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::cin.clear();
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                quit = true;
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                print_info("Reset reussi !");
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 1 || num < 0) {
                        print_info("Id invalide.");
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        print_info(std::to_string(num) + " déjà ajouté.");
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    print_info("Commande invalide."); 
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

        print_list({
        {"0", "Jouer un autre terrain ce tour-ci."},
        {"1", "Prendre 2 terrains dans la bibliothèque et la mélanger."} });

        print_actions("Choix effets, cf. ci-dessus", {
        {"valid", "valider vos choix"}, 
        {"reset", "réinitialiser vos choix"}}, "Entrez les effets de votre rituel : " );

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::cin.clear();
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                quit = true;
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                print_info("Reset reussi !");
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 1 || num < 0) {
                        print_info("Id invalide.");
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        print_info(std::to_string(num) + " déjà ajouté.");
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    print_info("Commande invalide."); 
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
    std::cout<< std::endl << "Entrez le coût en terrain blanc : " <<std::endl;

    int cost_white;
    std::cin>> cost_white;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain bleu : " <<std::endl;

    int cost_blue;
    std::cin>> cost_blue;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain noir : " <<std::endl;

    int cost_black;
    std::cin>> cost_black;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain rouge : " <<std::endl;

    int cost_red;
    std::cin>> cost_red;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain vert : " <<std::endl;

    int cost_green;
    std::cin>> cost_green;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain de n'importe quelle couleur : " <<std::endl;

    int cost_any;
    std::cin>> cost_any;
    std::cin.ignore();

    std::string cost;
    cost = std::to_string(cost_any) + ", " + std::to_string(cost_white) + ", " + std::to_string(cost_blue) + ", " + std::to_string(cost_black) + ", " + std::to_string(cost_red) + ", " + std::to_string(cost_green);

    m_file << "    #COST: " + cost <<std::endl;

    m_file << "#END_CARD" <<std::endl << std::endl;
    
}

void DeckBuild::create_enchantment(){
    
    // Begin
    m_file << "#BEGIN_CARD" <<std::endl;
    m_file << "    #TYPE_CARD: Ritual" <<std::endl;

    // Name
    std::cout<< std::endl <<  "Entrez le nom de votre enchantement : " <<std::endl;

    std::string name;
    std::cin.clear();
    std::getline(std::cin, name);

    m_file << "    #NAME: " + name <<std::endl;

    std::cout<<std::endl;

    // Token
    print_list({
        {std::to_string(0), "Enchantement blanc"},
        {std::to_string(1), "Enchantement bleu"},
        {std::to_string(2), "Enchantement noir"},
        {std::to_string(3), "Enchantement rouge"},
        {std::to_string(4), "Enchantement vert"},
    }, " - ");

    std::cout<< std::endl << "Entrez la couleur de votre enchantement : " <<std::endl;

    int token;
    bool quit = false;

    while(!quit){
        std::cin>> token;
        std::cin.ignore();
        if(token < 5 && token >= 0 ){
            quit = true;
        } else{
            print_info("Commande invalide.");
        }
    }

    m_file << "    #TOKEN: " + std::to_string(token) <<std::endl << std::endl;

    std::cout<<std::endl;

    // Effects
    switch (token)
    {
    case 0:{

        print_list({
        {"0", "Gagne un point de vie à chaque fois que tu joues une carte blanche."},
        {"1", "La créature enchantés à le vol et le lien de vie."}, });

        print_actions("Choix effets, cf. ci-dessus", {
        {"valid", "valider vos choix"}, 
        {"reset", "réinitialiser vos choix"}}, "Entrez les effets de votre enchantement : " );

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::cin.clear();
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                if(effects.empty()){
                    print_info("Votre enchantement doit avoir au moins un effet");
                } else{
                    quit = true;
                }
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                print_info("Reset reussi !");
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 1 || num < 0) {
                        print_info("Id invalide.");
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        print_info(std::to_string(num) + " déjà ajouté.");
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    print_info("Commande invalide."); 
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


        print_list({
        {"0", "Prendre le contrôle d'une créature tant qu'elle est en jeu."}, });

        print_actions("Choix effets, cf. ci-dessus", {
        {"valid", "valider vos choix"}, 
        {"reset", "réinitialiser vos choix"}}, "Entrez les effets de votre enchantement : " );

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::cin.clear();
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                if(effects.empty()){
                    print_info("Votre enchantement doit avoir au moins un effet");
                } else{
                    quit = true;
                }
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                print_info("Reset reussi !");
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 0 || num < 0) {
                        print_info("Id invalide.");
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        print_info(std::to_string(num) + " déjà ajouté.");
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    print_info("Commande invalide."); 
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

        print_list({
        {"0", "Chaque fois qu'une créature meurt, l'aversaire perd un point de vie"}, });

        print_actions("Choix effets, cf. ci-dessus", {
        {"valid", "valider vos choix"}, 
        {"reset", "réinitialiser vos choix"}}, "Entrez les effets de votre enchantement : " );

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::cin.clear();
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                if(effects.empty()){
                    print_info("Votre enchantement doit avoir au moins un effet");
                } else{
                    quit = true;
                }
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                print_info("Reset reussi !");
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 0 || num < 0) {
                        print_info("Id invalide.");
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        print_info(std::to_string(num) + " déjà ajouté.");
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    print_info("Commande invalide."); 
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

        print_list({
        {"0", "Créatures gagne 1 / 0 à chaque phase d'attaque."}, });

        print_actions("Choix effets, cf. ci-dessus", {
        {"valid", "valider vos choix"}, 
        {"reset", "réinitialiser vos choix"}}, "Entrez les effets de votre enchantement : " );

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::cin.clear();
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                if(effects.empty()){
                    print_info("Votre enchantement doit avoir au moins un effet");
                } else{
                    quit = true;
                }
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                print_info("Reset reussi !");
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 0 || num < 0) {
                        print_info("Id invalide.");
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        print_info(std::to_string(num) + " déjà ajouté.");
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    print_info("Commande invalide."); 
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

        print_list({
        {"0", "Terrain enchanté rapporte 2 ressources au lieu d'une."},
        {"1", "Créature enchantée gagne X / X, X le nombre de forêt contrôlées."} });

        print_actions("Choix effets, cf. ci-dessus", {
        {"valid", "valider vos choix"}, 
        {"reset", "réinitialiser vos choix"}}, "Entrez les effets de votre enchantement : " );

        std::string cmd;
        bool quit = false;
        std::vector<int> effects;

        while (!quit) {
            std::cin.clear();
            std::getline(std::cin, cmd);

            if (cmd.find("valid") != std::string::npos) {
                if(effects.empty()){
                    print_info("Votre enchantement doit avoir au moins un effet");
                } else{
                    quit = true;
                }
            } else if (cmd.find("reset") != std::string::npos) {
                effects = {};
                print_info("Reset reussi !");
            } else {
                try {
                    int num = std::stoi(cmd);
                    if (num > 1 || num < 0) {
                        print_info("Id invalide.");
                    } else if (std::find(effects.begin(), effects.end(), num) != effects.end()) {
                        print_info(std::to_string(num) + " déjà ajouté.");
                    } else{
                        effects.push_back(num);
                    }
                } catch (std::invalid_argument &e) {
                    print_info("Commande invalide."); 
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
    std::cout<< std::endl << "Entrez le coût en terrain blanc : " <<std::endl;

    int cost_white;
    std::cin>> cost_white;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain bleu : " <<std::endl;

    int cost_blue;
    std::cin>> cost_blue;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain noir : " <<std::endl;

    int cost_black;
    std::cin>> cost_black;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain rouge : " <<std::endl;

    int cost_red;
    std::cin>> cost_red;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain vert : " <<std::endl;

    int cost_green;
    std::cin>> cost_green;
    std::cin.ignore();

    std::cout<< std::endl << "Entrez le coût en terrain de n'importe quelle couleur : " <<std::endl;

    int cost_any;
    std::cin>> cost_any;
    std::cin.ignore();

    std::string cost;
    cost = std::to_string(cost_any) + ", " + std::to_string(cost_white) + ", " + std::to_string(cost_blue) + ", " + std::to_string(cost_black) + ", " + std::to_string(cost_red) + ", " + std::to_string(cost_green);

    m_file << "    #COST: " + cost <<std::endl;

    m_file << "#END_CARD" <<std::endl << std::endl;
    

}