//
// Created by Mattia Colapietro
//

#ifndef CELL_H
#define CELL_H


#include "Player.h"

class Cell {
public:
    // costruttore con valori opzionali per tipo, prezzo, costo di costruzione e pernottamento di casa e albergo 
    explicit Cell(char type = ' ', int price = 0, int construction_cost = 0, int house_stay = 0, int hotel_stay = 0)
    : type_{type}, price_{price}, construction_cost_{construction_cost}, house_stay_{house_stay}, hotel_stay_{hotel_stay} {};

    // getter
    char type() const { return type_; }
    int price() const { return price_; }
    int construction_cost() const { return construction_cost_; }
    int house_stay() const { return house_stay_; }
    int hotel_stay() const { return hotel_stay_; }
    bool hasHouse() const { return hasHouse_; }
    bool hasHotel() const { return hasHotel_; }
    std::shared_ptr<Player> owner() const { return owner_; }

    // se la casella è di un giocatore non inizializzato, non è di proprietà di un vero giocatore
    bool isOwned() const { return owner_ != nullptr; }
    // le caselle agli angoli e la partenza non sono acquistabili
    bool isProperty() const { return type_ != ' ' && type_ != 'P'; }
    // costo del pernottamento per altri giocatori che capitano sulla casella
    int overnightStayFee() const { return (hasHotel_ ? hotel_stay_ : (hasHouse_ ? house_stay_ : 0)); }

    // gestione delle caselle che sono proprietà acquistabili
    void buyProperty(std::shared_ptr<Player>& buyer);
    void upgradeProperty(std::shared_ptr<Player>& player);
    // reimposta ai valori iniziali le caselle (proprietario, case e alberghi se presenti)
    void resetProperty();

private:
    // vuota, partenza, economica, standard, lusso (ma non verificato che sia uno di questi in caso si volesse cambiare)
    char type_;
    // prezzo della casella, 0 se non acquistabile
    int price_;
    // costo di costruzione di casa/albergo
    int construction_cost_; // (usato per specificare sia costo di una casa che miglioramento ad albergo dato che uguali)
    // prezzo del pernottamento con una casa
    int house_stay_;
    // prezzo del pernottamento con un albergo
    int hotel_stay_;
    // se la casa è stata costruita
    bool hasHouse_ {false};
    // se l'albergo è stato costruito
    bool hasHotel_ {false};
    // proprietario della casella
    std::shared_ptr<Player> owner_;
};

// helper function per invocare il reset della casella nel caso in cui il giocatore sia il proprietario
void resetCell(const std::shared_ptr<Cell>& current_cell, const std::shared_ptr<Player>& current_player);
// overloading operatore allo stream di output che riproduce "graficamente" la casella (tipo ed eventuale casa o albergo)
std::ostream& operator<<(std::ostream& os, const Cell& cell);


#endif //CELL_H
