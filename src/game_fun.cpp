#include "game.h"

// Functional part of game

#include "error.h"

void set_clear_color(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{
    glClearColor(r, g, b, a);
}
