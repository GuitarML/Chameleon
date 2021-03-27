/*
  ==============================================================================

  lstm

  ==============================================================================
*/

#include "lstm.h"
#include <boost/range/irange.hpp>
#include <iostream>


lstm::lstm()
//====================================================================
// Description: Default constructor for lstm class
//
//====================================================================
{

}

float lstm::tanh(float x)
{
    return tanhf(x);
}

float lstm::sigmoid(float x)
{
    return 1.0f / (1.0f + expf(-x));
}

void lstm::setParams(int hidden_size, nc::NdArray<float> lstm_bias_ih_nc,
                     nc::NdArray<float> lstm_weights_ih_nc, nc::NdArray<float> lstm_bias_hh_nc,
                     nc::NdArray<float> lstm_weights_hh_nc, nc::NdArray<float> lstm_bias_nc,
                     nc::NdArray<float> dense_bias_nc, nc::NdArray<float> dense_weights_nc)
//====================================================================
// Description: Sets the parameters for the lstm class from the 
//   model loader class. These are set once when the default model 
//   is loaded, and are reset when a new model is loaded. Certain
//   arrays are initialized here based on the model params.
//
//  TODO: Handle a better way than having a function with all these arguments
//====================================================================
{
    HS = hidden_size;

    lstm_bias = lstm_bias_nc;
    lstm_weights_ih = lstm_weights_ih_nc;
    lstm_weights_hh = lstm_weights_hh_nc;
    lstm_bias = lstm_bias_nc;
    dense_bias = dense_bias_nc;
    dense_weights = dense_weights_nc);
}




//==============================================================================
void lstm::lstm_layer()
//====================================================================
// Description: This is the lstm layer. It has been simplified from
//   the full implementation to use stateful=False (the default setting
//   in Keras). This is the third layer in the stack.
//
//====================================================================
{
    gates = nc::dot(conv1d_1_out, W) + bias;
    for (int i = 0; i < HS; i++) {
        h_t[i] = sigmoid(gates[3 * HS + i]) * tanh(sigmoid(gates[i]) * tanh(gates[2 * HS + i]));
    }
    lstm_out = h_t;
}

void lstm::dense_layer()
//====================================================================
// Description: This is the dense layer, and the 4th and last layer
//   in the stack. It takes the output from the lstm layer and
//   performs a dot product to reduce the network output to a single
//   predicted sample of audio.
//
//====================================================================
{
    dense_out = nc::dot(lstm_out, dense_kernel) + dense_bias;
}



void lstm::process(const float* inData, float* outData, int numSamples)
//====================================================================
// Description: This processes each block (buffer) of audio data. 
//   It calls the initial data preparation functions, and then 
//   runs each sample of audio through the deep learning network.
//   The output is written to the write buffer. 
//
//====================================================================
{
    for (int i = 0; i < numSamples; i++)
    {
        // Set the current sample input to LSTM
        for (int j = 0; j < input_size; j++) {
            input[j] = data[i][j];
        }

        lstm_layer();
        dense_layer();
        outData[i] = dense_out[0];
    }
}