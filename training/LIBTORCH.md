# LibTorch: выполнение модели в C++

В этой ветке NeuralArtificialGamer не вычисляет слои сети вручную. Он передаёт 103 признака в LibTorch, получает четыре оценки и отсеивает недопустимые ходы. Модель в PyTorch можно менять, сохраняя интерфейс: 103 входных float и четыре выходных оценки. При изменении входа согласованно обновите NeuralFeatures.h и сбор обучающих данных.

models/neural_checkpoint.pth — checkpoint для продолжения обучения. models/neural_policy.pt — экспортированный модуль для LibTorch. Переименование файла .pth в .pt не заменяет экспорт.

После обучения запустите из корня проекта:

    py training\export_libtorch.py models\neural_checkpoint.pth models\neural_policy.pt

## Сборка в Visual Studio 2022

Потребуется CPU LibTorch той же версии, что PyTorch для экспорта. Скачать: https://pytorch.org/get-started/locally/. Для первого запуска выберите x64 Release. Windows Debug требует отдельную Debug сборку LibTorch; Debug и Release бинарники несовместимы.

Рекомендуемый способ — CMake из Developer PowerShell:

    cmake -S . -B build-libtorch -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="C:\libtorch"
    cmake --build build-libtorch --config Release
    .\build-libtorch\Release\SnakeGame.exe

Замените C:\libtorch на путь к распакованному LibTorch. CMake скопирует DLL и модель рядом с игрой. Для прежнего pyton.sln задайте LIBTORCH_ROOT с этим путём перед запуском Visual Studio из того же PowerShell. Проект настроен на C++20, include/lib директории и копирование DLL; CMake предпочтителен, поскольку получает список библиотек из пакета Torch.

Локально проверены экспорт .pth → .pt, совпадение выходов PyTorch и модуля, сборка CMake с LibTorch и запуск 20 партий. Сборка Visual Studio на Windows пока не проверена.

Сейчас экспорт использует TorchScript, так как torch::jit::load даёт прямой путь из PyTorch в LibTorch. PyTorch помечает TorchScript как устаревающий. При обновлении формата экспорта может понадобиться другой загрузчик. Checkpoint .pth хранится отдельно.
