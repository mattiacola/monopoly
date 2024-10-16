//
// Created by Giovanni Sgaravatto
//

#include "Player.h"

// sposta il giocatore nel tabellone e ritira 20 fiorini in caso di passaggio per il Via
void Player::move(int steps) {
    position_ += steps;
    if (position_ >= 27) {
        position_ -= 27;
        getPayed(20);
        // Giocatore N è passato dal via e ha ritirato 20 fiorini
        std::string message = "- Giocatore " + std::to_string(id_) + " e' passato dal via e ha ritirato 20 fiorini\n";
        std::cout << message;
        recordAction(message);
    }
}

// lancia 2 dadi e fa la somma dei risultati
int Player::rollDice() {
    constexpr int from = 1, to = 6;
    std::random_device rand1, rand2;
    std::mt19937 generator1(rand1()), generator2(rand2());
    std::uniform_int_distribution<int> d(from, to);

    // genera due numeri casuali da 1 a 6
    int dice1 = d(generator1);
    int dice2 = d(generator2);

    return dice1 + dice2;
}

// diminuisce il budget e controlla se manda in bancarotta
void Player::charge(int amount) {
    budget_ -= amount;
    if (budget_ <= 0) isBankrupt_ = true;
}

// toglie dal budget del debitore e aggiunge al creditore
void Player::pay(const std::shared_ptr<Player>& creditor, int amount) {
    charge(amount);
    if (!isBankrupt_) creditor->getPayed(amount);
}

// overloading operatore allo stream di output che riporta l'id del giocatore
std::ostream& operator<<(std::ostream& os, const Player& player) {
    return os << "Giocatore " << player.id();
}
