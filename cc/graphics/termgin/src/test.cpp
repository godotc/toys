#include <chrono>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <random>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <vector>


#include "terminal_mode.h"



int                       nScreenWidth  = 80; // Console Screen Size X (columns)
int                       nScreenHeight = 30; // Console Screen Size Y (rows)
std::vector<std::wstring> tetromino;
int                       nFieldWidth  = 12;
int                       nFieldHeight = 18;
unsigned char            *pField       = nullptr;

int Rotate(int px, int py, int r)
{
    int pi = 0;
    switch (r % 4)
    {
    case 0: // 0 degrees
        pi = py * 4 + px;
        break;

    case 1: // 90 degrees
        pi = 12 + py - (px * 4);
        break;

    case 2: // 180 degrees
        pi = 15 - (py * 4) - px;
        break;

    case 3: // 270 degrees
        pi = 3 - py + (px * 4);
    }

    return pi;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            int pi = Rotate(px, py, nRotation);
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

            if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
            {
                if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
                {
                    if (tetromino[nTetromino][pi] != L'.' && pField[fi] != 0)
                        return false;
                }
            }
        }
    }

    return true;
}

void DrawScreen(wchar_t *screen, int nScreenWidth, int nScreenHeight)
{
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
        screen[i] = L' ';

    // Draw Field
    for (int x = 0; x < nFieldWidth; x++)
    {
        for (int y = 0; y < nFieldHeight; y++)
        {
            screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];
        }
    }
}

int main()
{

    TerminalMode mode;

    // Set up tetromino shapes
    tetromino.push_back(L"..X...X...X...X."); // Tetrominos 4x4
    tetromino.push_back(L"..X..XX...X.....");
    tetromino.push_back(L".....XX..XX.....");
    tetromino.push_back(L"..X..XX..X......");
    tetromino.push_back(L".X...XX...X.....");
    tetromino.push_back(L".X...X...XX.....");
    tetromino.push_back(L"..X...X..XX.....");

    // Create play field buffer
    pField = new unsigned char[nFieldWidth * nFieldHeight];
    for (int x = 0; x < nFieldWidth; x++)
    {
        for (int y = 0; y < nFieldHeight; y++)
        {
            pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
        }
    }

    // Game variables
    bool             bKey[4];
    int              nCurrentPiece    = 0;
    int              nCurrentRotation = 0;
    int              nCurrentX        = nFieldWidth / 2;
    int              nCurrentY        = 0;
    int              nSpeed           = 20;
    int              nSpeedCount      = 0;
    bool             bForceDown       = false;
    bool             bRotateHold      = true;
    int              nPieceCount      = 0;
    int              nScore           = 0;
    std::vector<int> vLines;
    bool             bGameOver = false;

    // Create Screen Buffer
    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];

    while (!bGameOver)
    {
        // Timing
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        nSpeedCount++;
        bForceDown = (nSpeedCount == nSpeed);

        // Input
        for (int k = 0; k < 4; k++)
            bKey[k] = (0 != (0x01 & (getc(stdin) >> 4)));

        // Game Logic

        // Handle player movement
        nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
        nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
        nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

        // Rotate, but latch to stop wild spinning
        if (bKey[3])
        {
            nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
            bRotateHold = false;
        }
        else
            bRotateHold = true;

        // Force the piece down the playfield if it's time
        if (bForceDown)
        {
            // Update difficulty every 50 pieces
            nSpeedCount = 0;
            nPieceCount++;
            if (nPieceCount % 50 == 0)
                if (nSpeed >= 10)
                    nSpeed--;

            // Test if piece can be moved down
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
                nCurrentY++;
            else
            {
                // Lock the piece in place
                for (int px = 0; px < 4; px++)
                    for (int py = 0; py < 4; py++)
                        if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

                // Check for lines
                for (int py = 0; py < 4; py++)
                {
                    if (nCurrentY + py < nFieldHeight - 1)
                    {
                        bool bLine = true;
                        for (int px = 1; px < nFieldWidth - 1; px++)
                            bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

                        if (bLine)
                        {
                            for (int px = 1; px < nFieldWidth - 1; px++)
                                pField[(nCurrentY + py) * nFieldWidth + px] = 8;
                            vLines.push_back(nCurrentY + py);
                        }
                    }
                }

                nScore += 25;
                if (!vLines.empty())
                    nScore += (1 << vLines.size()) * 100;

                // Pick a new piece
                nCurrentX        = nFieldWidth / 2;
                nCurrentY        = 0;
                nCurrentRotation = 0;
                nCurrentPiece    = rand() % 7;

                // Game over if the piece does not fit
                bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }
        }

        // Display
        DrawScreen(screen, nScreenWidth, nScreenHeight);

        // Draw Current Piece
        for (int px = 0; px < 4; px++)
            for (int py = 0; py < 4; py++)
                if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                    screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;

        // Draw Score
        swprintf(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);

        // Animate Line Completion
        if (!vLines.empty())
        {
            // Display Frame (cheekily to draw lines)
            DrawScreen(screen, nScreenWidth, nScreenHeight);
            std::this_thread::sleep_for(std::chrono::milliseconds(400));

            for (auto &v : vLines)
                for (int px = 1; px < nFieldWidth - 1; px++)
                {
                    for (int py = v; py > 0; py--)
                        pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
                    pField[px] = 0;
                }

            vLines.clear();
        }

        // Display Frame
        for (int y = 0; y < nScreenHeight; y++)
        {
            for (int x = 0; x < nScreenWidth; x++)
            {
                putwchar(screen[y * nScreenWidth + x]);
            }
            putwchar(L'\n');
        }
    }

    // Oh Dear
    std::wcout << L"Game Over!! Score:" << nScore << std::endl;
    return 0;
}
