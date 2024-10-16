//
// Created by Giovanni Sgaravatto
//

#ifndef GAMEBOARD_H
#define GAMEBOARD_H


#include "Cell.h"
#include <algorithm>

class GameBoard {
public:
    // costruttore che può accettare numero diverso di caselle di tipo E, S, L ma che devono comunque ammontare a 28 (aggiungendo i 4 angoli)
    explicit GameBoard(int nEconomiche = kNumEconomiche, int nStandard = kNumStandard, int nLusso = kNumLusso);

    // getter
    int size() const { return kSize; }

    // funzione at utilizzata per accedere a elementi singoli del tabellone di gioco
    std::shared_ptr<Cell>& at(int index) { return board_.at(index); }
    const std::shared_ptr<Cell>& at(int index) const { return board_.at(index); }

    // iteratori sfruttati per accedere a intervalli di elementi del tabellone di gioco
    std::vector<std::shared_ptr<Cell>>::const_iterator begin() const { return board_.begin(); }
    std::vector<std::shared_ptr<Cell>>::const_iterator end() const { return board_.end(); }

    // riempimento del tabellone di gioco con caselle in posizioni casuali (ad eccezione della partenza e gli angoli vuoti)
    void randomFill(int nEconomiche, int nStandard, int nLusso);

private:
    static constexpr int kNumEconomiche = 8;
    static constexpr int kNumStandard = 10;
    static constexpr int kNumLusso = 6;

    // dimensione del tabellone di gioco (8x8 ha 28 caselle)
    static constexpr int kSize = 28;
    // tabellone di gioco, implementato come vettore di smart pointer a caselle
    std::vector<std::shared_ptr<Cell>> board_;
};

// trasforma un indice relativo al vettore di caselle nella rispettiva coordinata del tabellone 8x8
std::string indexToCoordinate(int index);


#endif // GAMEBOARD_H
