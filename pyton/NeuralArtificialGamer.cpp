#include "NeuralArtificialGamer.h"
#include "NeuralFeatures.h"
#include <algorithm>
#include <fstream>
#include <limits>

NeuralArtificialGamer::NeuralArtificialGamer(Field& field, Snake& snake)
    : field_(field), snake_(snake), fallback_(field, snake)
{
    std::ifstream input("models/neural_weights.txt");
    if (!input) input.open("../models/neural_weights.txt");
    int features = 0, hidden = 0, outputs = 0;
    if (!(input >> features >> hidden >> outputs)
        || features != NeuralInputSize || hidden != NeuralHiddenSize || outputs != 4) return;
    for (float& v : w1_) if (!(input >> v)) return;
    for (float& v : b1_) if (!(input >> v)) return;
    for (float& v : w2_) if (!(input >> v)) return;
    for (float& v : b2_) if (!(input >> v)) return;
    loaded_ = true;
}

Direction NeuralArtificialGamer::Command()
{
    if (!loaded_) return fallback_.Command();
    const auto x = NeuralFeatures(field_, snake_);
    std::array<float, NeuralHiddenSize> hidden{};
    for (int i = 0; i < NeuralHiddenSize; ++i)
    {
        float value = b1_[i];
        for (int j = 0; j < NeuralInputSize; ++j) value += w1_[i*NeuralInputSize+j] * x[j];
        hidden[i] = (std::max)(0.f, value);
    }
    const Direction dirs[4] = {left, right, up, down};
    Direction best = nothing;
    float bestScore = -std::numeric_limits<float>::infinity();
    for (int i = 0; i < 4; ++i)
    {
        if (x[4*i] == 0.f) continue;
        const Direction current = snake_.GetDirection();
        if (!snake_.GetTail().empty() && ((current == left && dirs[i] == right)
            || (current == right && dirs[i] == left)
            || (current == up && dirs[i] == down)
            || (current == down && dirs[i] == up))) continue;
        float score = b2_[i];
        for (int j = 0; j < NeuralHiddenSize; ++j) score += w2_[i*NeuralHiddenSize+j] * hidden[j];
        if (score > bestScore) { bestScore = score; best = dirs[i]; }
    }
    return best == nothing ? fallback_.Command() : best;
}
