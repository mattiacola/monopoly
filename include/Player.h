//
// Created by Giovanni Sgaravatto
//

#ifndef PLAYER_H
#define PLAYER_H


#include <memory>
#include <vector>
#include <random>
#include <iostream>
#include <string>

// forward declaration necessaria per la funzione playTurn
class GameBoard;

// eredita la classe necessaria a usare this nelle funzioni membro
class Player : public std::enable_shared_from_this<Player> {
public:
    // costruttore che accetta un numero intero usato per identificare il giocatore
    explicit Player(int id) : id_{id} { if (id < 0) throw std::logic_error("Un giocatore deve avere numero identificativo positivo"); }

    // getter
    int id() const { return id_; }
    int position() const { return position_; }
    int budget() const { return budget_; }
    bool isBankrupt() const { return isBankrupt_; }
    int order() const { return order_; }
    std::string actions() const { return actions_; }

    // lancia due dadi e ritorna la somma
    int rollDice();

    // gioca il proprio turno, implementato negli override di Human e Computer
    virtual void playTurn(const GameBoard& board, const std::vector<std::shared_ptr<Player>>& players) = 0;

    // sposta la pedina, se passa per il via riceve 20 fiorini
    virtual void move(int steps);
    // paga la banca
    void charge(int amount);
    // paga un altro giocatore
    void pay(const std::shared_ptr<Player>& creditor, int amount);
    // ricevi un pagamento
    void getPayed(int amount) { budget_ += amount; }
    // ottieni nuovo ordine di gioco
    void updateOrder() { order_ = rollDice(); }
    // aggiungi azione compiuta dal giocatore
    void recordAction(const std::string& message) { actions_ += message; }
    // resetta le azioni per un nuovo turno
    void resetActions() { actions_ = ""; }

protected:
    // identificatore del giocatore
    int id_ {0};
    // posizione (iniziando dalla partenza in H8)
    int position_ {0};
    // budget iniziale di ogni giocatore pari a 100 fiorini
    int budget_ {100};
    // se il giocatore è eliminato
    bool isBankrupt_ {false};
    // ordine in cui deve giocare
    int order_ {rollDice()};
    // registro delle azioni compiute dal giocatore
    std::string actions_;
};

// overloading operatore allo stream di output che riporta l'id del giocatore
std::ostream& operator<<(std::ostream& os, const Player& player);


#endif //PLAYER_H
