#include <iostream>
#include <typeinfo>
#include <iomanip>

#include "../includes/Game.hpp"

#include "../includes/FonctionsAux.hpp"

int main() {

    Land* l1  = new Land("terrain 1", 2, 999);
    Land* l2  = new Land("terrain 2", 2, 3);
    Land* l3  = new Land("terrain 333333333", 2, 3);
    Creature* c1 = new Creature("crecre", 1, 1 ,1, {}, {}, new Cost(1, 1, 1, 1, 1, 1));
    Creature* c2 = new Creature("yolo", 1, 1 ,1, {}, {}, new Cost(1, 1, 1, 1, 1, 1));
    Creature* c3 = new Creature("Ange de ces mprts", 1, 1 ,1, {}, {}, new Cost(1, 1, 1, 1, 1, 1));

    std::vector<BasicCard*> bc;
    bc.push_back(l1);
    bc.push_back(c1);
    bc.push_back(l2);
    bc.push_back(l3);
    bc.push_back(c2);
    bc.push_back(c3);

    Battlefield* b = new Battlefield();
    b->set_basic_cards(bc);

    std::cout<< typeid(bc[0]).name() << std::endl;

    Card* c = bc[1];

    std::cout << typeid(c).name() << std::endl;

    if(instanceof<Creature>(c)){
        std::cout << "helo";
        Creature *cr = dynamic_cast<Creature*>(c);
        std::cout << typeid(cr).name() << std::endl;
    }

    //std::cout << c->get_power() << std::endl;;

    //std::cout << typeid(bc[0]).name() << std::endl;

    //std::cout<< (typeid( *dynamic_cast<Land*>(bc[1])) == typeid(*l1)) ;

    //b->print();

    //Game g{};
    //g.start();

    return 0;
}