/*
 * GreenBall.h
 * Copyright (C) 2015 Marcin Natanek <natanek.marcin@gmail.com>
 *
 * Distributed under terms of the GPL license.
 */

#ifndef GREENBALL_H
#define GREENBALL_H

#include "src/engine/GreenEngine.cpp"

class GreenBall : public GreenEngine
{
    public:
        bool Events();
        void Logic();
        void Scene();
        bool LoadGraphics();
};

#endif /* !GREENBALL_H */
