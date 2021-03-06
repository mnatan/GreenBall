/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) Marcin Natanek, 2015
 */

#include "GreenBall.cpp"

int main()
{
    GreenBall main_engine;

    if (!main_engine.OK)
    {
        std::cout << "Error creating window!" << std::endl;
        return 1;
    }

    main_engine.SetTitle("<<GreenEngine>> - GreenBall");

    main_engine.Run();

    return 0;
}
