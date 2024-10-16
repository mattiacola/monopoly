//
// Created by Mattia Colapietro
//

#include "Cell.h"

// compra una proprietà
void Cell::buyProperty(std::shared_ptr<Player>& buyer) {
    if (isOwned()) throw std::logic_error("La proprieta' appartiene gia' a Giocatore " + std::to_string(owner_->id()));
    if (!isProperty()) throw std::logic_error("Questa proprieta' non puo' essere acquistata");

    if (buyer->budget() >= price_) {
        buyer->charge(price_);
        owner_ = buyer;
    } else {
        throw std::logic_error("Non hai abbastanza fiorini per completare l'acquisto");
    }
}

// costruisce una casa o la migliora in albergo
void Cell::upgradeProperty(std::shared_ptr<Player>& player) {
    if (owner_ != player) throw std::logic_error("La proprieta' non appartiene a questo giocatore, ma a Giocatore " + std::to_string(owner_->id()));
    if (hasHotel_) throw std::logic_error("La proprieta' ha gia' un albergo");

    if (player->budget() >= construction_cost_) {
        player->charge(construction_cost_);
        if (!hasHouse_ && !hasHotel_) hasHouse_ = true;
        else {
            hasHotel_ = true;
            hasHouse_ = false;
        }
    } else {
        throw std::logic_error("Non hai abbastanza fiorini per completare la costruzione");
    }
}

// reimposta ai valori iniziali la casella
void Cell::resetProperty() {
    owner_.reset();
    hasHouse_ = false;
    hasHotel_ = false;
}

// invoca il reset della casella nel caso in cui il giocatore sia il proprietario
void resetCell(const std::shared_ptr<Cell>& current_cell, const std::shared_ptr<Player>& current_player) {
    if (current_cell->owner() == current_player) current_cell->resetProperty();
}

// overloading operatore allo stream di output che riproduce "graficamente" la casella (tipo ed eventuale casa o albergo)
std::ostream& operator<<(std::ostream& os, const Cell& cell){
    os << cell.type() << (cell.hasHouse() ? "*" : "") << (cell.hasHotel() ? "^" : "");
    return os;
}
