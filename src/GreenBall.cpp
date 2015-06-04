/*
 * GreenBall.cpp
 * Copyright (C) 2015 Marcin Natanek <natanek.marcin@gmail.com>
 *
 * Distributed under terms of the GPL license.
 */

#include "GreenBall.h"

bool GreenBall::Events()
{
    SDL_Event ev;

    memset(&ev, 0, sizeof(ev));

    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
        case SDL_KEYUP:
            keys[ev.key.keysym.sym] = false;
            break;

        case SDL_KEYDOWN:
            if (ev.key.keysym.sym == SDLK_ESCAPE)
                return false;

            keys[ev.key.keysym.sym] = true;
            break;

        case SDL_QUIT:
            return false;
        }
    }

    if (win && win_countdown < current_time)
    {
        win = false;

        if (load_next_level())
        {
            game_complete = true;
        }
    }

    return true;
}

void GreenBall::Logic()
{
    Vector3D zmiana = none;

    // TODO this needs to be moved into player.cpp
    //printf("player\n");

    if (main_player->animating)
    {
        //printf("access\n");

        if (main_map->access(main_player->pos + down).canFall())
        {
            fail = true;
            main_player->setAnimation( main_player->pos,
                                       Vector3D (main_player->pos.x, main_player->pos.y, -21.0f),
                                       ANIM_TIME_FALL);
        }

        if (!fail)
        {
            if (keys[SDLK_UP])
            {
                zmiana = top;
                keys[SDLK_UP] = false;
            }
            else if (keys[SDLK_DOWN])
            {
                zmiana = bot;
                keys[SDLK_DOWN] = false;
            }
            else if (keys[SDLK_LEFT])
            {
                zmiana = left;
                keys[SDLK_LEFT] = false;
            }
            else if (keys[SDLK_RIGHT])
            {
                zmiana = right;
                keys[SDLK_RIGHT] = false;
            }
        }
    }

    //printf("other\n");

    if (zmiana != zero)
    {
        Vector3D nowaPozycja = main_player->pos + zmiana;

        if (main_map->access(nowaPozycja).canEnter(*main_map, zmiana))
        {
            main_map->access(nowaPozycja).playerEnters(*main_map, zmiana);
            main_player->setAnimation( main_player->pos,
                                       nowaPozycja,
                                       ANIM_PLAYER_TIME);
        }
    }

    if (main_player->animating)
    {
        main_player->animating =
            main_player->UpdateAnimation();
    }
}

void GreenBall::Scene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -14.0f);
    glScalef(1.0f, -1.0f, 1.0f);

    // animacja tła - czasowo lewo prawo
    if (current_time - background_start_time > ANIM_BACKG_TIME)
    {
        if (backdir == 1.0f)
            backdir = -1.0f;
        else
            backdir = 1.0f;

        background_start_time = current_time;
    }

    glPushMatrix();
    static float trans;

    glTranslatef(trans += ANIM_BACKG_SPEED * ratio * backdir, 0.0f, 0.0f);
    float skalar = 16.5f;

    DrawQuadTexture(Vector3D(0.0f, 0.0f, -20.1f),
                    4.0f * skalar, 3.0f * skalar, TEX_BACKG);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-main_player->pos.x, -main_player->pos.y, -0.0f);

    // Draw map
    // TODO FIXME opuszczamy globale
    //engine.main_map->draw_map();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // draw player
    DrawQuadTexture(main_player->pos + Vector3D(0, 0, 0.5f), 0.8f, 0.8f, TEX_PLAYER);

    if (win)
    {
        /*
         *DrawQuadTexture(
         *    (float)engine.main_player->pos.x - 7.5f, (float)engine.main_player->pos.y - 7.5f, 2.5f,
         *    7.0f, 2.0f,
         *    textures[TEX_LVLCMP]
         *);
         */
    }

    if (fail)
    {
        /*
         *DrawQuadTexture(
         *    (float)engine.main_player->pos.x - 7.5f, (float)engine.main_player->pos.y - 7.5f, 2.6f,
         *    7.0f, 2.0f,
         *    textures[TEX_FAIL]
         *);
         */
    }

    if (game_complete)
    {
        /*
         *DrawQuadTexture(
         *    (float)engine.main_player->pos.x - 7.5f, (float)engine.main_player->pos.y - 7.5f, 2.7f,
         *    7.0f, 2.0f,
         *    textures[TEX_WIN]
         *);
         */
    }

    glPopMatrix();

    //std::string scores = "Score: ";
    //std::stringstream ss;
    //ss << scores << score;
    std::string scores = "(";
    std::stringstream ss;
    ss << scores << main_player->pos.x << "," << main_player-> pos.y << "," << main_player->pos.z << ")";
    std::string result = ss.str();
    //TODO score display
    //scoresurf = TTF_RenderText_Solid(fontKomoda, result.c_str(), blueFont);
    //textures[TEX_SCORE] = SurfaceToTexture(scoresurf, TEX_SCORE);
    /*
     *DrawQuadRGBA(
     *    7.0f, -6.0f, 2.1f,
     *    3.5f, 1.5f,
     *    78, 158, 116, 0.6
     *);
     *DrawQuadTexture(
     *    7.0f, -6.0f, 2.2f,
     *    3.0f, 1.0f,
     *    textures[TEX_SCORE]
     *);
     */

    glPushMatrix();
    gluPerspective(0.0, (float)screen_width / (float)screen_height, 0.0,
                   1024.0);
    static float rotat;

    glTranslatef(7.0f, 5.0f, 3.0f);
    glRotatef(rotat += 50.0f * ratio, 0.5f, 1.0f, 0.6f);
    glTranslatef(-7.0f, -5.0f, -3.0f);
    DrawCubeTexture(Vector3D(7.0f, 5.0f, 3.0f), 0.8f, TEX_KUCYK);
    glPopMatrix();

    glDisable(GL_BLEND);

    SDL_GL_SwapBuffers();
}
