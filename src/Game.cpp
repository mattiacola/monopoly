//
// Created by Filippo Artico
//

#include "Game.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

Game::Game(int argc, char* argv[]) {
    // Gestione degli argomenti inseriti da linea di comando
    if (argc != 2) throw std::logic_error("Non hai inserito il numero di argomenti richiesto");

    if (std::string{argv[1]} == "human") hasHuman_ = true;
    else if (std::string{argv[1]} != "computer") throw std::logic_error("L'argomento inserito non e' valido, inserisci \"computer\" o \"human\"");

    // creazione dei giocatori in base a quanto specificato da linea di comando
    createPlayers();
}

// gestisce la partita finché non rimane un solo giocatore o viene raggiunto il numero massimo di turni
void Game::startGame() {
    std::string message = "\n<--------------------    INIZIO PARTITA    -------------------->\n";
    std::cout << message;
    addToLog(message);

    // ordina i giocatori in base al lancio di dadi per decidere l'ordine di gioco
    getStartingOrder();

    int turnCounter = 0;
    // dato che eliminiamo un giocatore ogni volta che va in bancarotta, l'ultimo che rimane ha vinto
    while (turnCounter < kMaxTurns_ && players_.size() > 1) {
        message = "|====================    TURNO " + std::to_string(turnCounter + 1) + "    ====================|\n";
        std::cout << message;
        addToLog(message);

        // ogni giocatore gioca l'n-esimo turno
        for (int i = 0; i < players_.size(); ++i) {
            auto player = players_.at(i);
            player->playTurn(gameBoard_, players_);
            if (!player->actions().empty()) {
                addToLog(player->actions());
                player->resetActions();
            }
            // resetto le proprietà di un giocatore che ha perso prima di rimuoverlo
            if (player->isBankrupt()) {
                // cerca le caselle di proprietà del giocatore che ha perso e resettale
                for_each(gameBoard_.begin(), gameBoard_.end(),[&player](const std::shared_ptr<Cell>& current_cell) { resetCell(current_cell, player); });
                // ottieni puntato al giocatore che ha perso e va rimosso
                auto player_to_remove = remove_if(players_.begin(), players_.end(), [](const std::shared_ptr<Player>& player) { return player->isBankrupt(); });
                // rimuovi il giocatore dalla partita
                players_.erase(player_to_remove);
                //Giocatore N è stato eliminato
                message = "- Giocatore " + std::to_string(player->id()) + " e' stato eliminato\n";
                std::cout << message;
                addToLog(message);
            } else {
                message = "- Giocatore " + std::to_string(player->id()) + " ha finito il turno\n\n";
                std::cout << message;
                addToLog(message);
            }
        }

        turnCounter++;
    }

    // deduci chi ha vinto la partita
    pickWinners();
}

// riempimento del vettore di giocatori con Computer e Human (in base a quanto richiesto)
void Game::createPlayers() {
    std::string message;
    // creazione dei giocatori specificando un numero da 1 a 4 come id
    for (int i = 0; i < kNumberOfPlayers_; ++i) {
        try {
            // Giocatore 1 viene controllato dall'utente in caso sia stato così specificato dagli argomenti dalla linea di comando
            if (hasHuman_ && i == 0) players_.push_back(std::make_shared<Human>(i + 1));
            // altrimenti tutti i giocatori sono controllati dal computer
            else players_.push_back(std::make_shared<Computer>(i + 1));
        } catch (const std::logic_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    message = (hasHuman_ ? "Creati 1 giocatore umano e 3 controllati dal computer\n" : "Creati 4 giocatori controllati dal computer\n");
    std::cout << message;
    addToLog(message);
}

// lancio di dadi iniziale e ordinamento dei giocatori in base al risultato, con rilancio in caso di pareggi
void Game::getStartingOrder() {
    std::string message;
    // Giocatore N ha tirato i dadi ottenendo un valore di X
    for (const auto& player: players_) {
        message += "- Giocatore " + std::to_string(player->id()) + " ha tirato i dadi ottenendo un valore di " + std::to_string(player->order()) + "\n";
    }
    std::cout << message;
    addToLog(message);

    // metti in ordine decrescente per lancio di dadi i giocatori per decidere l'ordine di gioco
    sort(players_.begin(), players_.end(), [](const std::shared_ptr<Player>& p1, const std::shared_ptr<Player>& p2) { return p1->order() > p2->order(); });

    while (true) {
        // fornisce puntatore al primo giocatore il cui successivo ha ottenuto lo stesso lancio di dadi
        auto begin = adjacent_find(players_.begin(), players_.end(), [](const std::shared_ptr<Player>& p1, const std::shared_ptr<Player>& p2) { return p1->order() == p2->order(); });
        // se non ce n'è uno semplicemente significa che sono tutti diversi, quindi esci
        if (begin == players_.end()) break;
        // fornisce puntatore al primo giocatore il cui successivo ha ottenuto lancio di dadi diverso (cioè l'ultimo con lo stesso lancio di dadi)
        auto end = adjacent_find(begin, players_.end(), [](const std::shared_ptr<Player>& p1, const std::shared_ptr<Player>& p2) { return p1->order() != p2->order(); });
        // se si tratta dell'ultimo elemento iterato diminuisci il puntatore di 1, in quanto sarebbe fuori dal vettore
        if (end == players_.end()) end--;
        // fai rilanciare i dadi a tutti i giocatori nel range trovato
        for_each(begin, end + 1, [](const std::shared_ptr<Player>& player) { player->updateOrder(); });
        // rimetti in ordine decrescente per lancio di dadi i giocatori per decidere l'ordine di gioco
        sort(players_.begin(), players_.end(), [](const std::shared_ptr<Player>& p1, const std::shared_ptr<Player>& p2) { return p1->order() > p2->order(); });

        message = "Alcuni giocatori hanno ottenuto lo stesso valore dal lancio di dadi, devono rilanciare\n";
        for (const auto& player: players_) {
            message += "- Giocatore " + std::to_string(player->id()) + " ha tirato i dadi ottenendo un valore di " + std::to_string(player->order()) + "\n";
        }
        std::cout << message;
        addToLog(message);
    }

    message = "Dopo aver verificato che tutti abbiano lanciato dadi diversi, l'ordine di gioco sara':\n";
    for (int i = 0; i < players_.size(); ++i) {
        message += std::to_string(i+1) + ". Giocatore " + std::to_string(players_.at(i)->id()) + " che ha tirato i dadi ottenendo un valore di " + std::to_string(players_.at(i)->order()) + "\n";
    }
    std::cout << message;
    addToLog(message);
}

// gestione dei giocatori, eliminando chi ha perso
void Game::pickWinners() {
    std::string message;
    std::string endGameMessage = "\n<--------------------    FINE PARTITA    -------------------->\n";

    if (players_.size() == 1) {
        // Giocatore N ha vinto la partita
        message = endGameMessage + "- Giocatore " + std::to_string(players_.at(0)->id()) + " ha vinto la partita\n";
        std::cout << message;
        addToLog(message);
    } else {
        // ordina i giocatori in ordine decrescente in base ai fiorini rimasti
        sort(players_.begin(), players_.end(), [](const std::shared_ptr<Player>& p1, const std::shared_ptr<Player>& p2) { return p1->budget() > p2->budget(); });
        message = "Massimo numero di turni raggiunto, giocatori in ordine di fiorini rimasti:\n";
        for (const auto& player: players_) {
            message += "- Giocatore " + std::to_string(player->id()) + " ha ancora " + std::to_string(player->budget()) + " fiorini\n";
        }
        std::cout << message;
        addToLog(message);

        // trova giocatori che non hanno vinto pareggiando, cioè che hanno perso
        auto losers = adjacent_find(players_.begin(), players_.end(), [](const std::shared_ptr<Player>& p1, const std::shared_ptr<Player>& p2) { return p1->budget() != p2->budget(); });
        if (losers == players_.begin() || distance(players_.begin(), losers) == 1) losers++;  // +1 perché altrimenti punterebbe al primo diverso (cioè uno dei vincitori)
        // elimina i giocatori che hanno perso
        players_.erase(losers, players_.end());

        if (players_.size() == 1) {
            message = endGameMessage + "- Giocatore " + std::to_string(players_.at(0)->id()) + " ha vinto la partita per maggior numero di fiorini rimasti\n";
            std::cout << message;
            addToLog(message);
        } else {
            message = endGameMessage + "Hanno vinto a pari merito:\n";
            for (const auto& player : players_) {
                message += "- Giocatore " + std::to_string(player->id()) + " con " + std::to_string(player->budget()) + " fiorini rimasti\n";
            }
            std::cout << message;
            addToLog(message);
        }
    }
}

// aggiunge un messaggio al file di log
void Game::addToLog(const std::string& message) const {
    // crea un file di log (se già non esiste) con ora e data attuali
    std::ofstream logFile(kLogFileName_ + timeAndDate_ + (hasHuman_ ? "-human" : "-computer") + ".txt", std::ios_base::out | std::ios_base::app);
    if (!logFile.is_open()) throw std::invalid_argument("Impossibile scrivere sul file di log");
    logFile << message;
}

std::string currentTimeAndDate() {
    // ottengo ora e data attuali e trasformo in stringa
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    std::stringstream s(std::string{});
    s << std::put_time(&tm, "-%H%M%S-%d%m%Y");
    std::string date_string;
    s >> date_string;
    
    return date_string;
}
