#pragma once
#include "Wall.h"

class Tree :
    public Wall
{
    public:
        std::unique_ptr<Tree>* selfptr;

        Tree();
        void init(GameManager GM, std::unique_ptr<Tree>* sp);
        void draw_self();
        void update();
};

