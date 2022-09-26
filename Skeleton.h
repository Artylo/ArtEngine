#pragma once
#include "Entity.h"
class Skeleton : public Entity
{
public:

    SDL_Color hairCol;
    SDL_Color shirtCol;
    SDL_Color pantsCol;

    std::shared_ptr<SDL_Surface> surf_hair = nullptr;
    std::shared_ptr<SDL_Surface> surf_shirt = nullptr;
    std::shared_ptr<SDL_Surface> surf_pants = nullptr;

    std::shared_ptr<SDL_Texture> tex_hair = nullptr;
    std::shared_ptr<SDL_Texture> tex_shirt = nullptr;
    std::shared_ptr<SDL_Texture> tex_pants = nullptr;

    Skeleton();
    void update();
    void draw_self();
    void generate_clothes_textures();
};

