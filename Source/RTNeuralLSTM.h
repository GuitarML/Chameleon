#pragma once

#include <RTNeural/RTNeural.h>
//#include <RTNeural.h>

class RT_LSTM
{
public:
    RT_LSTM() = default;

    void reset();
    void load_json(const nlohmann::json& weights_json);

    void process(const float* inData, float* outData, int numSamples);

private:
    RTNeural::ModelT<float, 1, 1,
        RTNeural::LSTMLayerT<float, 1, 20>,
        RTNeural::DenseT<float, 20, 1>> model;
};
