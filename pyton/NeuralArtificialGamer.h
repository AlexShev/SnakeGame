#pragma once
#include "IGamer.h"
#include "Field.h"
#include "Snake.h"
#include "HardArtificialGamer.h"
#include "NeuralFeatures.h"
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
    std::array<float, NeuralHiddenSize*NeuralInputSize> w1_{};
    std::array<float, NeuralHiddenSize> b1_{};
    std::array<float, 4*NeuralHiddenSize> w2_{};
    std::array<float, 4> b2_{};
};
