#include "MyGame.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>

// bloques
block bloque;
block nextBlock;

int tpoints, lineas;

std::vector<std::pair<int, int>> mapeo;

void MyGame::done(int tpoints, int lineas)
{
    // Recorre desde abajo
    for (int y = 35; y >= 11; y--)
    {
        bool completed = true;

        // Find fila completa
        for (int x = 16; x <= 44; x++)
        {
            con.setCursor(y, x);
            if (con.getCharAt(y, x) == ' ')
            {
                completed = false;
                break;
            }
        }

        // completa?
        if (completed)
        {
            std::cout << "\nFila completa en y: " << y << "\n";

            // Animación de fila completa
            for (int i = 16; i <= 44; i++)
            {
                con.setCursor(y, i);
                con.setForecolor(CColor::SkyBlue2);
                con << "\x1";
                con.delayMs(20);
                con.setCursor(y, i);
                con.setForecolor(CColor::White);
                con.delayMs(20);
                con << "\x1";
            }
            con.setForecolor(CColor::Black);

            // Mover todas las filas hacia abajo.
            for (int row = y; row > 16; row--)
            {
                for (int col = 16; col <= 44; col++)
                {
                    con.setCursor(row, col);
                    char ch = con.getCharAt(row - 1, col);
                    con.setCursor(row, col);
                    con << ch;
                }
            }

            // Limpiar la fila superior después de mover las filas
            for (int col = 16; col <= 44; col++)
            {
                con.setCursor(16, col);
                con << ' ';
            }

            tpoints += 1000;
            lineas++;
            // Puntaje
            con.setCursor(9, 72);
            con << "Puntos";
            con.setCursor(10, 72);
            con << tpoints;

            // Lineas completadas
            con.setCursor(13, 72);
            con << "Lineas";
            con.setCursor(14, 72);
            con << lineas;

            con.refresh();
            y++;
        }
    }
}

void MyGame::falling()
{

    while (con.isActive())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(800));

        bool valido = true;

        for (auto &coord : bloque.coords)
        {
            if (coord.first + 1 > 35)
            {
                valido = false;
                break;
            }
        }

        std::pair<int, int> vp, vp2, vp3, vp4;
        int it = 0;
        
        for (auto &coord : valoresTomados)
        {

            for (auto &coords : bloque.coords)
            {
                if (it == 0)
                {
                    vp = coords;
                    vp.first++;
                }
                else if (it == 1)
                {
                    vp2 = coords;
                    vp2.first++;
                }
                else if (it == 2)
                {
                    vp3 = coords;
                    vp3.first++;
                }
                else
                {
                    vp4 = coords;
                    vp4.first++;
                }
                it++;
            }
            if(vp == coord||vp2 == coord||vp3 == coord||vp4 == coord){
                valido = false;
                break;
            }
        }

        if (valido)
        {
            clearBlock(bloque);
            for (auto &coord : bloque.coords)
            {
                coord.first += 1;
            }
            printBlock();
        }
        else
        {

            done(tpoints, lineas);

            for (auto &coord : bloque.coords)
            {
                valoresTomados.push_back(coord);
            }

            bloque = nextBlock;

            int random = genRandom();

            getBlock(random, nextBlock);

            printBlock();
            printNextBlock(random);
        }
    }
}
// crear bloque
void MyGame::getBlock(int tipo, block &bloque)
{

    if (tipo == 1)
    {
        // linea
        bloque.coords = {{11, 31}, {12, 31}, {13, 31}, {14, 31}};
    }
    else if (tipo == 2)
    {
        // T
        bloque.coords = {{11, 30}, {11, 31}, {11, 32}, {12, 31}};
    }
    else if (tipo == 3)
    {
        // left snake
        bloque.coords = {{11, 30}, {11, 31}, {12, 31}, {12, 32}};
    }
    else if (tipo == 4)
    {
        // right snake
        bloque.coords = {{11, 32}, {11, 31}, {12, 31}, {12, 30}};
    }
    else if (tipo == 5)
    {
        // left L
        bloque.coords = {{11, 30}, {12, 30}, {13, 30}, {13, 29}};
    }
    else if (tipo == 6)
    {
        // right L
        bloque.coords = {{11, 30}, {12, 30}, {13, 30}, {13, 31}};
    }
    else
    {
        // square
        bloque.coords = {{11, 30}, {11, 31}, {12, 30}, {12, 31}};
    }

    bloque.shape = tipo;
}

void MyGame::printNextBlock(int randomNumber)
{
    con.setCursor(1, 72);
    con << "Next";
    for (int i = 3; i < 7; i++)
    {
        con.setCursor(i, 72);
        con << "   ";
    }

    if (randomNumber == 1)
    {
        // line

        for (int i = 3; i < 7; i++)
        {
            con.setCursor(i, 73);
            con << "\x4";
        }
    }
    else if (randomNumber == 2)
    {
        // T

        con.setCursor(3, 72);
        con << "\x4\x4\x4";
        con.setCursor(4, 73);
        con << "\x4";
    }
    else if (randomNumber == 3)
    {
        // left snake

        con.setCursor(3, 72);
        con << "\x4\x4";
        con.setCursor(4, 73);
        con << "\x4\x4";
    }
    else if (randomNumber == 4)
    {
        // right snake

        con.setCursor(3, 73);
        con << "\x4\x4";
        con.setCursor(4, 72);
        con << "\x4\x4";
    }
    else if (randomNumber == 5)
    {
        // left L

        con.setCursor(3, 73);
        con << "\x4";
        con.setCursor(4, 73);
        con << "\x4";
        con.setCursor(5, 72);
        con << "\x4\x4";
    }
    else if (randomNumber == 6)
    {
        // right L

        con.setCursor(3, 73);
        con << "\x4";
        con.setCursor(4, 73);
        con << "\x4";
        con.setCursor(5, 73);
        con << "\x4\x4";
    }
    else
    {
        // square

        con.setCursor(3, 73);
        con << "\x4\x4";
        con.setCursor(4, 73);
        con << "\x4\x4";
    }
}

// imprimir bloque
void MyGame::printBlock()
{

    clearBlock(bloque);
    for (const auto &coord : bloque.coords)
    {
        int y = coord.first;
        int x = coord.second;
        con.setCursor(y, x);
        con.setForecolor(CColor::Black);
        con << "\x4";
    }
}

// generar numero random
int MyGame::genRandom()
{
    int randomNumber = std::rand() % 7 + 1;
    std::cout << "RANDOM: ";
    std::cout << randomNumber;

    return randomNumber;
}

// validacion para rotacion
bool MyGame::isBlockValid(const block &b)
{
    for (auto &coord : b.coords)
    {
        int y = coord.first;
        int x = coord.second;

        if (x < 16 || x > 44 || y > 35)
        {
            return false;
        }
    }
    return true;
}

void MyGame::rotate(block &b)
{
    if (b.shape == 7)
    {
        std::cout << "Cuadrado no puede rotar." << std::endl;
        return;
    }

    int cx = b.coords[1].second;
    int cy = b.coords[1].first;

    std::vector<std::pair<int, int>> newCoords;

    // Rotar cada coordenada alrededor del centro
    for (auto &coord : b.coords)
    {
        int x = coord.second - cx;
        int y = coord.first - cy;
        int newX = -y + cx;
        int newY = x + cy;
        newCoords.push_back({newY, newX});
    }

    block tempBlock = b;
    tempBlock.coords = newCoords;

    // Verificar si el bloque rotado es válido
    if (isBlockValid(tempBlock))
    {
        b.coords = newCoords;
    }
    else
    {
        std::cout << "Rotación no válida." << std::endl;
    }
}

void MyGame::clearBlock(const block &b)
{
    for (const auto &coord : b.coords)
    {
        int y = coord.first;
        int x = coord.second;
        con.setCursor(y, x);
        con.setForecolor(CColor::DarkOliveGreen1);
        con << " ";
    }
}

// correr programa
void MyGame::run()
{
    std::srand(static_cast<unsigned>(std::time(0)));

    con.setForecolor(CColor::Grey);
    con.setBackcolor(CColor::DarkOliveGreen1);
    con.clearScreen();
    con.setForecolor(CColor::Black);

    // mapa

    // lateral izq
    for (int i = 10; i <= 35; i++)
    {
        con.setCursor(i, 15);
        con << "\x1";
    }

    // abajo
    for (int i = 15; i <= 45; i++)
    {
        con.setCursor(36, i);
        con << "\x3";
    }

    // arriba
    for (int i = 15; i <= 45; i++)
    {
        con.setCursor(9, i);
        con << "\x2";
    }

    // lateral derecha
    for (int i = 10; i <= 35; i++)
    {
        con.setCursor(i, 45);
        con << "\x1";
    }

    int random = genRandom();
    int nextRandom = genRandom();

    getBlock(random, bloque);
    getBlock(nextRandom, nextBlock);

    printBlock();
    printNextBlock(nextRandom);

    // Creacion de hilo
    std::thread fallThread(&MyGame::falling, this);

    // Puntaje
    con.setCursor(9, 72);
    con << "Puntos";
    con.setCursor(10, 72);
    con << 0;

    // Lineas completadas
    con.setCursor(13, 72);
    con << "Lineas";
    con.setCursor(14, 72);
    con << 0;

    con.setForecolor(CColor::White);

    // key press logic
    con.flushKeybuffer();
    while (con.isActive())
    {
        uint32_t key = con.getKey();

        if (key != SDLK_UNKNOWN)
        {

            bool valido = true;
            switch (key)
            {
            case SDLK_LEFT:

                valido = true;

                for (auto &coord : bloque.coords)
                {
                    if (coord.second - 1 < 16)
                    {
                        valido = false;
                        break;
                    }
                }

                if (valido)
                {
                    clearBlock(bloque);
                    for (auto &coord : bloque.coords)
                    {
                        coord.second -= 1;
                    }
                    printBlock();
                }

                break;

            case SDLK_RIGHT:

                valido = true;

                for (auto &coord : bloque.coords)
                {
                    if (coord.second + 1 > 44)
                    {
                        valido = false;
                        break;
                    }
                }

                if (valido)
                {
                    clearBlock(bloque);
                    for (auto &coord : bloque.coords)
                    {
                        coord.second += 1;
                    }
                    printBlock();
                }

                break;
            case SDLK_UP:

                clearBlock(bloque);
                rotate(bloque);
                printBlock();

                break;
            case SDLK_DOWN:
                valido = true;

                for (auto &coord : bloque.coords)
                {
                    if (coord.first + 1 > 35)
                    {
                        valido = false;
                        break;
                    }
                }

                if (valido)
                {
                    clearBlock(bloque);
                    for (auto &coord : bloque.coords)
                    {
                        coord.first += 1;
                    }
                    printBlock();
                }
                break;
            case SDLK_F1:
                con << "F1 pressed\n";
                break;
            default:
                con << "Other key pressed\n";
            }
            con.refresh();
        }
    }
    // fallThread.join();
}
