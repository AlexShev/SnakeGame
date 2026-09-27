#pragma once
#include "IGamer.h"
#include "Field.h"
#include "Snake.h"
#include "HardArtificialGamer.h"
#include "NeuralFeatures.h"
#include <torch/script.h>

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
    torch::jit::Module model_;
    bool loaded_ = false;
};
