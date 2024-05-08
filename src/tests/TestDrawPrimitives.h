#pragma once
#include <vector>
#include "Test.h"
#include "../system/graphics/Line.h"
#include "../system/debug.h"

#define IM Test::testGM->input_manager

namespace test
{
    class TestDrawPrimitives :
        public Test
    {
        public:
            Line test_line;
            std::vector<float> line_colour = {1.0, 1.0, 1.0, 1.0};
            std::vector<float> positions = {0, 0, 32, 32};

            //@TEST:
            std::vector<Line> lines;

        public:

        TestDrawPrimitives(GameManager* GM);
        ~TestDrawPrimitives();

        void OnUpdate() override;
        void OnRender() override;
        void OnGUIRender() override;

        private:
    };
}

