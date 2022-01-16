#ifndef DECKBUILD_HPP
#define DECKBUILD_HPP

#include <string>
#include <fstream>

class DeckBuild{

    private:

    std::string                m_filename;
    std::ofstream              m_file;
    const int                  m_nb_cards = 30;
    int                        m_crea = 0;
    int                        m_land = 0;
    int                        m_ritu = 0;
    int                        m_ench = 0;

    public:

    DeckBuild() {}

    int                        get_nb_cards_current() const;

    void                       create_file();
    void                       create_creature();
    void                       create_land();
    void                       create_ritual();
    void                       create_enchantment();

};

#endif