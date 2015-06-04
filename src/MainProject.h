/*
 * MainProject.h
 * Copyright (C) 2015 Marcin Natanek <natanek.marcin@gmail.com>
 *
 * Distributed under terms of the GPL license.
 */

#ifndef MAINPROJECT_H
#define MAINPROJECT_H

#include "src/engine/GreenEngine.cpp"

class MainProject : public GreenEngine
{
    public:
        bool Events();
        void Logic();
        void Scene();
        bool LoadGraphics();
};

#endif /* !MAINPROJECT_H */
