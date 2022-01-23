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
    std::cin.clear();
    if (s != "") {    
        std::string lign(s.size(), '-');
        std::cout << std::endl << get_color(Color::BrightBlack) << lign << std::endl << std::endl;

        std::cout << get_color(Color::Yellow) << s << std::endl << std::endl;

        std::cout << get_color(Color::BrightBlack) << lign << std::endl << std::endl;
    }
    std::cout << get_color(Color::BrightBlack)  << std::endl << "<Entrée> pour continuer." << get_color(Color::Reset) << std::endl;

    std::string cmd;
    std::getline(std::cin, cmd);
}

void print_err(const std::string& s)  {
    std::cin.clear();   
    
    std::string prefix = "Erreur : ";
    std::string lign(prefix.size() + s.size() , '-');

    std::cout << std::endl << get_color(Color::BrightBlack) << lign << std::endl << std::endl;

    std::cout << get_color(Color::Cyan) << prefix << s << std::endl << std::endl;
    std::cout << get_color(Color::BrightBlack) << lign << std::endl << std::endl << get_color(Color::Reset);

    std::string cmd;
    std::getline(std::cin, cmd);
}

void print_list(const std::vector<std::pair<std::string, std::string>>& options, const std::string& separator) {
    size_t max_title_size = 0;
    for (auto i : options)
        if (i.first.size() > max_title_size)
            max_title_size = i.first.size();

    
    for(auto i : options) {
        std::string padding(max_title_size - i.first.size(), ' ');
        std::cout << get_color(Color::Yellow) << i.first << padding << get_color(Color::BrightBlack) << separator << get_color(Color::Reset) << i.second << std::endl;
    }

}

void print_actions(const std::string& title, const std::vector<std::pair<std::string, std::string> >& options, const std::string& end_message, bool dot_opt, const std::string& separator) {
    size_t size = title.size();
    if (end_message.size() > size)
        size = end_message.size();

    size_t nb_options = options.size();
    size_t max_option_title_size = 0;
    std::string dot = ".";

    for(auto i : options) {
        size_t new_size = i.first.size() + i.second.size() + separator.size() + (dot_opt ? dot.size() : 0);
        if (new_size > size)
            size = new_size;
        if (max_option_title_size < i.first.size()) {
            max_option_title_size = i.first.size();
        }
    }
    
    std::string lign(size, '-');
    std::cout << get_color(Color::BrightBlack) << lign <<std::endl << std::endl << get_color(Color::BrightRed) << title << std::endl << get_color(Color::BrightBlack) << lign << get_color(Color::Reset) << std::endl;
    
    bool nl = false;
    if (nb_options > 0) {
        nl = true;
        for(auto i : options) {
            std::string padding(max_option_title_size - i.first.size(), ' ');
            std::cout << get_color(Color::Yellow) << i.first << padding << get_color(Color::BrightBlack) << separator << get_color(Color::Reset) << i.second;
            if (dot_opt)
                std::cout << dot;
            std::cout << std::endl;
        }
    }
    if (end_message != "") {
        nl = true;
        std::cout << std::endl;
        std::cout << get_color(Color::BrightWhite) << end_message << get_color(Color::Reset) << std::endl;
    }
    if (nl)
        std::cout << get_color(Color::BrightBlack) << lign << get_color(Color::Reset) << std::endl;
}

void cls() {
    std::system("clear");
}

std::string get_color(Color c) {
    switch (c) {
        case Color::Reset:
            return "\033[0m";
        case Color::Black:
            return "\033[30m";
        case Color::Red:
            return "\033[31m";
        case Color::Green:
            return "\033[32m";
        case Color::Yellow:
            return "\033[33m";
        case Color::Blue:
            return "\033[34m";
        case Color::Magenta:
            return "033[35m";
        case Color::Cyan:
            return "\033[36m";
        case Color::White:
            return "\033[37m";
        case Color::BrightBlack:
            return "\u001b[30;1m";
        case Color::BrightRed:
            return "\u001b[31;1m";
        case Color::BrightGreen:
            return "\u001b[32;1m";
        case Color::BrightYellow:
            return "\u001b[33;1m";
        case Color::BrightBlue:
            return "\u001b[34;1m";
        case Color::BrightMagenta:
            return "\u001b[35;1m";
        case Color::BrightCyan:
            return "\u001b[36;1m";
        case Color::BrightWhite:
            return "\u001b[37;1m";
        default:
            return "";
    }
}

std::string list_int_to_string(std::vector<int> l) {
    std::string s = "";
    if (l.size() == 0)
        return s;

    for(size_t i = 0; i < l.size()-1; i++)
        s += std::to_string(l[i]) + ", ";
    
    s += std::to_string(l.back());
    return s;
}