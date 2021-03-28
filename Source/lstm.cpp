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
//
//  TODO: Handle a better way than having a function with all these arguments
//====================================================================
{
    HS = hidden_size;

    lstm_bias = lstm_bias_nc;
    lstm_weights_ih = lstm_weights_ih_nc; //TODO here down are empty arrays
    lstm_weights_hh = lstm_weights_hh_nc;
    dense_bias = dense_bias_nc;
    dense_weights = dense_weights_nc;
}




//==============================================================================
void lstm::lstm_layer()
//====================================================================
// Description: This is the lstm layer. It has been simplified from
//   the full implementation to use stateful=False (the default setting
//   in Keras). This is the third layer in the stack.
//
//====================================================================
/*
self.residual = xt # residual is for skip connection

HS = self.hidden_units # Hidden size
hidden = self.hidden
bias = b
gates = np.dot(xt, W) + np.dot(hidden, U) + bias
i_t, f_t, g_t, o_t = (
    sigmoid(gates[:, : HS]), # input
    sigmoid(gates[:, HS : HS * 2]), # forget
    self.tanh(gates[:, HS * 2 : HS * 3]),
    sigmoid(gates[:, HS * 3 : ]), # output
    )
    c_t = f_t * self.cell_state + i_t * g_t # Cell state
    h_t = o_t * self.tanh(c_t) # Hidden state
    self.hidden = h_t     # c_t and h_t assignments written out for clarity, can just update the self.hiddenand self.cell states without using c_tand h_t
    self.cell_state = c_t
*/

{
    // DEBUG
    //nc::NdArray<float>xt_test = nc::zeros<float>(128,1);
    //nc::NdArray<float> test = nc::dot(xt, lstm_weights_ih);  /// This throws exception, dot product of 1 not working?
    //nc::NdArray<float> test1 = nc::dot(h_t, lstm_weights_hh);
    //nc::NdArray<float> test2 = nc::dot(h_t, lstm_weights_hh) + lstm_bias;

    gates = nc::dot(xt, lstm_weights_ih) + nc::dot(h_t, lstm_weights_hh) + lstm_bias;  // TODO: Exception here, Try transposing matrices (when read in) like in pytorch code
    for (int i = 0; i < HS; i++) {
        c_t[i] = sigmoid(gates[HS + i]) * c_t[i] + sigmoid(gates[i]) * tanh(gates[2 * HS + i]);
        h_t[i] = sigmoid(gates[3 * HS + i]) * tanh(c_t[i]);
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
    dense_out = nc::dot(lstm_out, dense_weights) + dense_bias;
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
        xt = inData[i];
        lstm_layer();
        dense_layer();
        outData[i] = dense_out[0] + inData[i]; // Set output and add residual skip
    }
}