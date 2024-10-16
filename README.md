# Prova Finale di Programmazione - Monopoly

Gruppo: è un 2.1 o un 2.0?<br />
Membri: Filippo Artico, Mattia Colapietro, Giovanni Sgaravatto

<hr />

### Directory structure

```
monopoly
├── log-computer.txt
├── log-human.txt
├── README.md
├── CMakeLists.txt
├── include
│   ├── Cell.h
│   ├── Computer.h
│   ├── Game.h
│   ├── GameBoard.h
│   ├── Human.h
│   └── Player.h
└── src
    ├── Cell.cpp
    ├── Computer.cpp
    ├── Game.cpp
    ├── GameBoard.cpp
    ├── Human.cpp
    ├── Player.cpp
    └── main.cpp
```

### Metodo consigliato per compilare con CMake

1. Aprire un terminale all'interno della cartella `monopoly/`
2. Creare una cartella per i file di build: `mkdir build`
3. Eseguire: `cmake . -B build`
4. Compilare i file di progetto appena creati: `cmake --build build`
5. Lanciare l'eseguibile: `build/main` specificando `human` o `computer`

<hr />

#### Osservazioni

Abbiamo notato che impostando anche numeri altissimi (migliaia) come limite massimo di turni non porta a una
terminazione naturale della partita con un solo vincitore. Durante i nostri test abbiamo quindi diminuito il numero di
fiorini ottenuto da ogni giocatore quando passa per il via per ottenere un solo vincitore o al massimo dei vincitori a
pari merito, cioè con alcuni giocatori eliminati in quanto rimasti senza fiorini.
Nella nostra implementazione del progetto abbiamo scelto di utilizzare un vettore per gestire le 28 caselle del
tabellone di gioco al posto di un array bidimensionale (8x8) o altre strutture dati, per poi convertirne gli indici
durante la stampa a schermo.
Non abbiamo trovato metodi migliori per la nostra implementazione del gioco di Monopoly con le richieste specificate
nella consegna per stampare il tabellone di gioco, le proprietà possedute dai giocatori e il loro bilancio attuale su
richiesta dell'utente, dunque abbiamo ritenuto necessario, per mantenere il polimorfismo dinamico delle classi `Human` e
`Computer`, di dover fornire come argomento della funzione `playTurn()` anche il vettore contenente i giocatori nonostante
solo `Human` lo necessiti realmente.