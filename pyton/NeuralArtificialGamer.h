#pragma once
#include "IGamer.h"
#include "Field.h"
#include "Snake.h"
#include "HardArtificialGamer.h"
#include <array>
#include <string>
#include <vector>

class NeuralArtificialGamer : public IGamer
{
public:
    NeuralArtificialGamer(Field& field, Snake& snake);
    Direction Command() override;
    bool Loaded() const { return loaded_; }
private:
    Field& field_;
    Snake& snake_;
    HardArtificialGamer fallback_;
    bool loaded_ = false;
    std::array<float, 32*21> w1_{};
    std::array<float, 32> b1_{};
    std::array<float, 4*32> w2_{};
    std::array<float, 4> b2_{};
};
