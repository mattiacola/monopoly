//
// Created by Mattia Colapietro
//

#include "GameBoard.h"
#include "Human.h"

bool Human::getCommand(const std::string& question, const GameBoard &board, const std::vector<std::shared_ptr<Player>>& players) const {
    std::string command;

    std::cout << question;
    std::cin >> command;

    // gestione input dell'utente
    while (command[0] != 'N' && command[0] != 'S') {
        if (command == "show") {
            printGameBoard(board, players);
            printProperties(board, players);
            printBalances(players);

            std::cout << question;
            std::cin >> command;
        } else {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Comando errato, inserisci S, N o show: ";
            std::cin >> command;
        }
    }

    return toupper(command[0]) == 'S';
}

void Human::playTurn(const GameBoard& board, const std::vector<std::shared_ptr<Player>>& players) {
    std::string message;

    int dice_rolled = rollDice();
    // Giocatore N ha tirato i dadi ottenendo un valore di X
    message = "- Giocatore " + std::to_string(id_) + " ha tirato i dadi ottenendo un valore di " + std::to_string(dice_rolled) + "\n";
    std::cout << message;
    recordAction(message);

    move(dice_rolled);
    // Giocatore N è arrivato alla casella Y
    message = "- Giocatore " + std::to_string(id_) + " e' arrivato alla casella " + indexToCoordinate(position_) + "\n";
    std::cout << message;
    recordAction(message);

    auto player = shared_from_this();
    const auto &current_cell = board.at(position_);
    if (!current_cell->isOwned()) {
        if (current_cell->isProperty()) {
            if (getCommand("Vuoi acquistare la proprieta'? [S/N/show]\n", board, players)) {
                try {
                    current_cell->buyProperty(player);
                    // Giocatore N ha acquistato il terreno Y
                    message = "- Giocatore " + std::to_string(id_) +
                              " ha acquistato il terreno " +
                              indexToCoordinate(position_) + "\n";
                    std::cout << message;
                    recordAction(message);
                } catch (const std::logic_error &e) {
                    std::cout << e.what() << std::endl;
                }
            }
        }
    } else {
        if (current_cell->owner()->id() != id_) {
            if (current_cell->hasHouse() || current_cell->hasHotel()) {
                try {
                    pay(current_cell->owner(), current_cell->overnightStayFee());
                    // Giocatore N ha pagato Z fiorini a giocatore M per pernottamento nella casella Y
                    message = "- Giocatore " + std::to_string(id_) + " ha pagato " +
                              std::to_string(current_cell->overnightStayFee()) +
                              " fiorini a giocatore " +
                              std::to_string(current_cell->owner()->id()) +
                              " per pernottamento nella casella " +
                              indexToCoordinate(position_) + "\n";
                    std::cout << message;
                    recordAction(message);
                } catch (const std::logic_error &e) {
                    std::cout << e.what() << std::endl;
                }
            }
        } else if (!current_cell->hasHotel()) {
            std::string question =
                    (current_cell->hasHouse()
                     ? "Vuoi migliorare la casa in albergo? [S/N/show]\n"
                     : "Vuoi costruire una casa? [S/N/show]\n");
            if (getCommand(question, board, players)) {
                try {
                    current_cell->upgradeProperty(player);
                    // Giocatore N ha costruito una casa sul terreno Y
                    // OPPURE Giocatore N ha migliorato una casa in albergo sul terreno Y
                    message = "- Giocatore " + std::to_string(id_) +
                              (current_cell->hasHouse()
                               ? " ha costruito una casa sul terreno "
                               : " ha migliorato una casa in albergo sul terreno ") +
                              indexToCoordinate(position_) + "\n";
                    std::cout << message;
                    recordAction(message);
                } catch (const std::logic_error &e) {
                    std::cout << e.what() << std::endl;
                }
            }
        }
    }
}

void Human::printGameBoard(const GameBoard& board, const std::vector<std::shared_ptr<Player>>& players) const {
    // ottieni l'insieme delle caselle aggiungendo case/alberghi e giocatori attualmente posizionati su di esse
    std::vector<std::string> cellsToPrint;
    for (int i = 0; i < board.size(); i++) {
        std::string cellToPrint {board.at(i)->type()};
        cellToPrint += (board.at(i)->hasHouse() ? "*" : "");
        cellToPrint += (board.at(i)->hasHotel() ? "^" : "");
        for (const auto& player : players) {
            if (player->position() == i) cellToPrint += std::to_string(player->id());
        }
        cellsToPrint.push_back(cellToPrint);
    }

    // prima riga
    std::cout << " \t";
    for (int i = 0; i < 8 * 2; i++) {
        if (i < 8) std::cout << i + 1;
        else if (i == 8) std::cout << "\nA";

        else std::cout << cellsToPrint.at(board.size() / 2 - 9 + i);
        std::cout << "\t";
    }
    // prima e ultima colonna (tranne prima e ultima riga)
    std::cout << std::endl;
    for (int i = 0; i < 6; i++) {
        std::string s;
        s = 'B' + i;
        std::cout << s << "\t" << cellsToPrint.at(board.size() / 2 - 1 - i) << "\t\t\t\t\t\t\t" << cellsToPrint.at(board.size() - 6 + i) << std::endl;
    }
    // ultima riga
    std::cout << "H\t";
    for (int j = 8; j > 0; j--) {
        std::cout << cellsToPrint.at(j - 1) << "\t";
    }
    std::cout << std::endl;
}

void Human::printProperties(const GameBoard& board, const std::vector<std::shared_ptr<Player>>& players) const {
    std::cout << "Terreni, case e alberghi posseduti: " << std::endl;

    for (const auto& player : players) {
        // vettore contenente numero corrispondente all'indice della casella relativa in board_ (coordinata)
        std::vector<int> properties;
        for (int i = 0; i < board.size(); ++i) {
            if (board.at(i)->owner() == player) properties.push_back(i);
        }
        std::cout << *player << ": ";
        for (int i = 0; i < properties.size(); ++i) {
            std::cout << indexToCoordinate(properties.at(i));
            if (i != properties.size()-1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
}

void Human::printBalances(const std::vector<std::shared_ptr<Player>>& players) const {
    std::cout << "Bilancio attuale di ogni giocatore: " << std::endl;
    for (const auto& player : players) {
        std::cout << *player << ": " << player->budget() << std::endl;;
    }
}
