

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"

Game::Game(unsigned int width, unsigned int height)
    : m_State(GameState::GAME_ACTIVE), m_keys(), m_Width(width), m_Height(height)
{
}

Game::~Game()
{
}

void Game::Init()
{
}

void Game::Update(float dt)
{
}

void Game::ProcessInput(float dt)
{
}

void Game::Render()
{
}
