//
// Created by Filippo Artico
//

#include "Computer.h"
#include "GameBoard.h"

// restituisce vero con probabilità del 25% (1/4)
bool Computer::doesAction() const {
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, 3);

    return distribution(generator) == 0;
}

// gestisce il turno del giocatore controllato dal computer
void Computer::playTurn(const GameBoard& board, const std::vector<std::shared_ptr<Player>>& players) {
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
    const auto& current_cell = board.at(position_);
    if (!current_cell->isOwned()) {
        if ((current_cell->isProperty() && budget_ >= current_cell->price()) && doesAction()) {
            current_cell->buyProperty(player);
            // Giocatore N ha acquistato il terreno Y
            message = "- Giocatore " + std::to_string(id_) + " ha acquistato il terreno " + indexToCoordinate(position_) + "\n";
            std::cout << message;
            recordAction(message);
        }
    } else {
        if (current_cell->owner()->id() != id_) {
            if (current_cell->hasHouse() || current_cell->hasHotel()) {
                pay(current_cell->owner(), current_cell->overnightStayFee());
                // Giocatore N ha pagato Z fiorini a giocatore M per pernottamento nella casella Y
                message =
                        "- Giocatore " + std::to_string(id_) + " ha pagato " +
                        std::to_string(current_cell->overnightStayFee()) +
                        " fiorini a giocatore " + std::to_string(current_cell->owner()->id()) +
                        " per pernottamento nella casella " + indexToCoordinate(position_) + "\n";
                std::cout << message;
                recordAction(message);
            }
        } else if (!current_cell->hasHotel()) {
            if (budget_ >= current_cell->construction_cost() && doesAction()) {
                current_cell->upgradeProperty(player);
                // Giocatore N ha costruito una casa sul terreno Y
                // OPPURE Giocatore N ha migliorato una casa in albergo sul terreno Y
                message = "- Giocatore " + std::to_string(id_) +
                          (current_cell->hasHouse() ? " ha costruito una casa sul terreno "
                                                    : " ha migliorato una casa in albergo sul terreno ") +
                          indexToCoordinate(position_) + "\n";
                std::cout << message;
                recordAction(message);
            }
        }
    }
}
