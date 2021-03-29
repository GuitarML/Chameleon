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


void lstm::setParams(int hidden_size, 
                     nc::NdArray<float> lstm_weights_ih_nc, 
                     nc::NdArray<float> lstm_weights_hh_nc, nc::NdArray<float> lstm_bias_nc,
                     nc::NdArray<float> dense_bias_nc, nc::NdArray<float> dense_weights_nc)
//====================================================================
// Description: Sets the parameters for the lstm class from the 
//   model loader class. These are set once when the default model 
//   is loaded, and are reset when a new model is loaded. Certain
//   arrays are initialized here based on the model params.

//====================================================================
{
    HS = hidden_size;

    lstm_bias = lstm_bias_nc;
    lstm_weights_ih = lstm_weights_ih_nc;
    lstm_weights_hh = lstm_weights_hh_nc;
    dense_bias = dense_bias_nc;
    dense_weights = dense_weights_nc;
	
	// Initialize the lstm arrays
	gates = nc::zeros<float>(1, HS * 4);
    h_t = nc::zeros<float>(1, HS);
    c_t = nc::zeros<float>(1, HS);
}


//==============================================================================
void lstm::lstm_layer()
//====================================================================
// Description: This is the stateful lstm inference layer. 
//
//====================================================================
{
    gates = nc::dot(xt, lstm_weights_ih) + nc::dot(h_t, lstm_weights_hh) + lstm_bias;
    for (int i = 0; i < HS; i++) {
        c_t[i] = sigmoid(gates[HS + i]) * c_t[i] + sigmoid(gates[i]) * tanh(gates[2 * HS + i]);
        h_t[i] = sigmoid(gates[3 * HS + i]) * tanh(c_t[i]);
    }
    lstm_out = h_t;
}

void lstm::dense_layer()
//====================================================================
// Description: This is the dense (or linear) layer. It takes the 
//   output from lstm layer and performs a dot product to reduce the
//   network output to a single predicted sample of audio.
//
//====================================================================
{
    dense_out = nc::dot(lstm_out, dense_weights) + dense_bias;
}


void lstm::process(const float* inData, float* outData, int numSamples)
//====================================================================
// Description: This processes each block (buffer) of audio data. 
//   It runs each sample of audio through the deep learning network.
//   The output is written to the write buffer. 
//
//====================================================================
{
    for (int i = 0; i < numSamples; i++)
    {
        xt = inData[i];
        lstm_layer();
        dense_layer();
		
		// Set output and add residual skip
        outData[i] = dense_out[0] + inData[i]; 
    }
}