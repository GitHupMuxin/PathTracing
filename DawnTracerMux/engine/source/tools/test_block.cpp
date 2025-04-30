#include "engine/tools/test_block.h"

namespace tool
{
    TestBlock::TestBlock() 
    {
        this->data.rayCount = 0;
        this->data.rayTimeCount = 0;
    }

    TestBlock::~TestBlock() { }

    TotalTestBlock::TotalTestBlock() { }

    TotalTestBlock::~TotalTestBlock() { }

    TotalTestBlock* TotalTestBlock::GetInstance()
    {
        static TotalTestBlock instance;
        return &instance;
    }
}





