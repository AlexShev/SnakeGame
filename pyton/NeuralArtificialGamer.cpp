#include "NeuralArtificialGamer.h"
#include <array>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

NeuralArtificialGamer::NeuralArtificialGamer(Field& field, Snake& snake)
    : field_(field), snake_(snake), fallback_(field, snake)
{
    for (const char* path : {"models/neural_policy.pt", "../models/neural_policy.pt"})
    {
        try
        {
            model_ = torch::jit::load(path, torch::kCPU);
            model_.eval();
            loaded_ = true;
            break;
        }
        catch (const c10::Error&)
        {
            // Try the other working directory; retain the algorithmic fallback.
        }
    }
}

Direction NeuralArtificialGamer::Command()
{
    if (!loaded_) return fallback_.Command();
    const auto features = NeuralFeatures(field_, snake_);
    torch::NoGradGuard noGrad;
    const auto input = torch::from_blob(
        const_cast<float*>(features.data()), {1, NeuralInputSize},
        torch::TensorOptions().dtype(torch::kFloat32)).clone();
    torch::Tensor output;
    try
    {
        output = model_.forward({input}).toTensor().to(torch::kCPU).contiguous().view({-1});
        if (output.numel() != 4) return fallback_.Command();
    }
    catch (const c10::Error&)
    {
        return fallback_.Command();
    }
    const Direction dirs[4] = {left, right, up, down};
    const Direction current = snake_.GetDirection();
    Direction best = nothing;
    float bestScore = -std::numeric_limits<float>::infinity();
    for (int i = 0; i < 4; ++i)
    {
        if (features[4*i] == 0.f) continue;
        if (!snake_.GetTail().empty() && ((current == left && dirs[i] == right)
            || (current == right && dirs[i] == left)
            || (current == up && dirs[i] == down)
            || (current == down && dirs[i] == up))) continue;
        const float score = output[i].item<float>();
        if (score > bestScore) { bestScore = score; best = dirs[i]; }
    }
    return best == nothing ? fallback_.Command() : best;
}
