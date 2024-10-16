//
// Created by Filippo Artico
//

#ifndef COMPUTER_H
#define COMPUTER_H


#include "Player.h"

class Computer : public Player {
public:
    // costruttore che rimanda a quello implementato per Player dato che uguale a quello di Human
    explicit Computer(int id) : Player(id) {}
    // probabilità del 25% (1/4) di compiere un azione
    bool doesAction() const;
    // gioca il proprio turno
    void playTurn(const GameBoard& board, const std::vector<std::shared_ptr<Player>>& players) override;
};


#endif //COMPUTER_H
