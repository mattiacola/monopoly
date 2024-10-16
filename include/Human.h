//
// Created by Mattia Colapietro
//

#ifndef HUMAN_H
#define HUMAN_H


#include "Player.h"

class Human : public Player {
public:
    // costruttore che rimanda a quello implementato per Player dato che uguale a quello di Computer
    explicit Human(int id) : Player(id) {}

    // gioca il proprio turno
    void playTurn(const GameBoard &board, const std::vector<std::shared_ptr<Player>>& players) override;

    // chiede all'utente di inserire un comando
    bool getCommand(const std::string& question, const GameBoard &board, const std::vector<std::shared_ptr<Player>>& players) const;

    // stampa il tabellone di gioco aggiornato con le posizioni dei giocatori
    void printGameBoard(const GameBoard& board, const std::vector<std::shared_ptr<Player>>& players) const;
    // stampa le proprietà dei giocatori
    void printProperties(const GameBoard& board, const std::vector<std::shared_ptr<Player>>& players) const;
    // stampa i fiorini rimasti ai giocatori
    void printBalances(const std::vector<std::shared_ptr<Player>>& players) const;
};


#endif //HUMAN_H
