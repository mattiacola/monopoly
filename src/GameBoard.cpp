//
// Created by Giovanni Sgaravatto
//

#include "GameBoard.h"

GameBoard::GameBoard(int nEconomiche, int nStandard, int nLusso) {
    // in un tabellone 8x8 ci sono 28 caselle in totale
    const int nCornerCells = 4;
    if (nEconomiche + nStandard + nLusso + nCornerCells != kSize) throw std::logic_error("Il numero di caselle non rispetta la grandezza del tabellone");

    board_.reserve(kSize);
    randomFill(nEconomiche, nStandard, nLusso);
}

void GameBoard::randomFill(int nEconomiche, int nStandard, int nLusso) {
    // P di partenza
    auto start = std::make_shared<Cell>('P');
    board_.push_back(std::move(start));

    // inserimento dei tipi di cella
    std::vector<std::shared_ptr<Cell>> randomCells;
    for (int i = 0; i < nEconomiche; ++i) {
        randomCells.push_back(std::make_shared<Cell>('E', 6, 3, 2, 4));
    }
    for (int i = 0; i < nStandard; ++i) {
        randomCells.push_back(std::make_shared<Cell>('S', 10, 5, 4, 8));
    }
    for (int i = 0; i < nLusso; ++i) {
        randomCells.push_back(std::make_shared<Cell>('L', 20, 10, 7, 14));
    }

    // randomizzo il vettore di celle
    std::mt19937 generator(std::random_device{}());
    std::shuffle(randomCells.begin(), randomCells.end(), generator);

    // riempio casualmente le celle (tranne gli angoli e la partenza)
    int letterIndex = 0;
    for (int i = 0; i < kSize; ++i) {
        if (i == 7 || i == 14 || i == 21) {
            board_.push_back(std::move(std::make_shared<Cell>()));
        } else if (i != 0) {
            board_.push_back(std::move(randomCells.at(letterIndex++)));
        }
    }
}

std::string indexToCoordinate(int index) {
    std::vector<std::string> coordinate = {"H8", "H7", "H6", "H5", "H4", "H3", "H2", "H1",
                                           "G1", "F1", "E1", "D1", "C1", "B1",
                                           "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8",
                                           "B8", "C8", "D8", "E8", "F8", "G8"};
    return coordinate.at(index);
}