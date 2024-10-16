//
// Created by Filippo Artico
//

#ifndef GAME_H
#define GAME_H


#include "GameBoard.h"
#include "Human.h"
#include "Computer.h"
#include <fstream>
#include <sstream>

// forward declaration di helper function per ottenere ora e data usata nella classe
std::string currentTimeAndDate();

class Game {
public:
    // costruttore che accetta gli argomenti forniti da linea di comando
    Game(int argc, char* argv[]);
    // fai iniziare la partita
    void startGame();

private:
    // in base agli argomenti inseriti da linea di comando, determina se umano contro computer o tutti computer
    bool hasHuman_ {false};
    // 4 giocatori, di cui al massimo 1 solo giocatore umano
    static constexpr int kNumberOfPlayers_ = 4;
    // giocatori, possono essere umani o computer
    std::vector<std::shared_ptr<Player>> players_;
    // numero massimo di turni dopo cui si decide il vincitore in base ai fiorini rimasti
    static constexpr int kMaxTurns_ = 100;
    // tavolo da gioco
    GameBoard gameBoard_;
    // nome del file di log
    std::string kLogFileName_ = "../log";
    // ora e data della partita
    std::string timeAndDate_ {currentTimeAndDate()};

    // crea il vettore di giocatori in base a quanto specificato nel costruttore
    void createPlayers();
    // ottiene i giocatori che hanno vinto
    void pickWinners();
    // lancio di dadi iniziale per decidere ordine di partenza, fa rilanciare giocatori che hanno ottenuto stesso risultato
    void getStartingOrder();

    // aggiunge il messaggio al file di log
    void addToLog(const std::string& message) const;
};


#endif //GAME_H
