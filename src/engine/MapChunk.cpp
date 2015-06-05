#ifndef MAPCHUNK_CPP
#define MAPCHUNK_CPP

#include "MapChunk.h"
#include "../game_objects/game_obj.cpp"
/*
 *#include "../game_objects/animated.cpp"
 *#include "../game_objects/EmptySpace.cpp"
 *
 *#include "animated.cpp"
 *#include "game_obj.cpp"
 *#include "EmptySpace.cpp"
 */


MapChunk::MapChunk()
{
    zawartosc = std::vector<game_obj*>();
}

bool MapChunk::canEnter(Map& map,
                        Vector3D& zmiana)        // Sprawdza czy na tym miejscu nie ma elementów uniemożliwiających wejście
{
    bool can = true;

    for (auto x : zawartosc)
        can &= x->canEnter(map, zmiana);

    return can;
}

bool MapChunk::canFall()        // Sprawdza czy to miejsce może utrzymać obiekt
{
    bool can = false;

    for (auto x : zawartosc)
        can |= x->canFall();

    return can;
}

bool MapChunk::drawIt()         // Rysuje cały pion warstw mapy
{
    for (auto x : zawartosc)
        x->drawIt(); // TODO not pointer?

    return true;
}

bool MapChunk::playerEnters(Map& map,
                            Vector3D& delta)    // Informujemy wszystkie obiekty na polu że wchodzi tam gracz
{
    for (auto x : zawartosc)
        x->playerEnters(map, delta);

    return true;
}

void MapChunk::print_zawartosc()
{
    std::cout << "MapChunk :" << std::endl;

    for (auto x : zawartosc)
        std::cout << "- " << x->typKlasy() << std::endl;
}

#endif
