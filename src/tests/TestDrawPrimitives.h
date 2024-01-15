#pragma once
#include "Test.h"
#include "../system/graphics/Line.h"
#include "../system/graphics/LineThickness.h"

namespace test
{
    class TestDrawPrimitives :
        public Test
    {
        public:
            Line test_line;
            float line_colour[4] = {1.0, 1.0, 1.0, 1.0};
            float positions[4] = {32, 32, 64, 128};

            LineThickness* test_line_thickness;

        public:

        TestDrawPrimitives(GameManager* GM);
        ~TestDrawPrimitives();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnGUIRender() override;

        private:
    };
}

