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

void lstm::setParams(int hidden_size, int conv1d_kernel_size, int conv1d_1_kernel_size, int conv1d_num_channels, int conv1d_1_num_channels,
                    nc::NdArray<float> conv1d_bias_nc, nc::NdArray<float> conv1d_1_bias_nc, 
                    std::vector<nc::NdArray<float>> conv1d_kernel_nc, std::vector<nc::NdArray<float>> conv1d_1_kernel_nc,
                    nc::NdArray<float> lstm_bias_nc, nc::NdArray<float> lstm_kernel_nc, 
                    nc::NdArray<float> dense_bias_nc, nc::NdArray<float> dense_kernel_nc, int input_size_loader, 
                    int conv1d_stride_loader, int conv1d_1_stride_loader)
//====================================================================
// Description: Sets the parameters for the lstm class from the 
//   model loader class. These are set once when the default model 
//   is loaded, and are reset when a new model is loaded. Certain
//   arrays are initialized here based on the model params.
//
//  TODO: Handle a better way than having a function with all these arguments
//====================================================================
{
    input_size = input_size_loader;
    HS = hidden_size;

    conv1d_Kernel_Size = conv1d_kernel_size;
    conv1d_Num_Channels = conv1d_num_channels;
    conv1d_1_Kernel_Size = conv1d_1_kernel_size;
    conv1d_1_Num_Channels = conv1d_1_num_channels;
    conv1d_stride = conv1d_stride_loader;
    conv1d_1_stride = conv1d_1_stride_loader;

    nc::NdArray<float> conv1d_bias_temp = conv1d_bias_nc;
    conv1d_kernel = conv1d_kernel_nc;

    nc::NdArray<float> conv1d_1_bias_temp = conv1d_1_bias_nc;
    conv1d_1_kernel = conv1d_1_kernel_nc;

    W = lstm_kernel_nc;
    bias = lstm_bias_nc;

    dense_kernel = dense_kernel_nc;
    dense_bias = dense_bias_nc;

    h_t = nc::zeros<float>(1, HS);

    gates = nc::zeros<float>(1, HS * 4);

    nc::NdArray<float> dummy_input = nc::zeros<float>(nc::Shape(input_size, 1));

    // Set up bias matrices for calculation 
    pad_init(dummy_input);
    nc::NdArray<float> padded_dummy = pad(dummy_input);  // TODO handle different strides
    int bias_shape = padded_dummy.shape().rows / conv1d_stride; // TODO handle different strides
    conv1d_bias = nc::zeros<float>(nc::Shape(bias_shape, conv1d_bias_temp.shape().cols));;
    nc::NdArray<float> new_bias = conv1d_bias_temp;
    for (int i = 0; i < bias_shape - 1; i++)
    {
        new_bias = nc::append(new_bias, conv1d_bias_temp, nc::Axis::ROW);
    }
    conv1d_bias = new_bias;

    
    bias_shape = 1; //TODO fix having to hardcode
    conv1d_1_bias = nc::zeros<float>(nc::Shape(bias_shape, conv1d_1_bias_temp.shape().cols));;
    nc::NdArray<float> new_bias2 = conv1d_1_bias_temp;
    for (int i = 0; i < bias_shape - 1; i++)
    {
        new_bias2 = nc::append(new_bias2, conv1d_1_bias_temp, nc::Axis::ROW);
    }
    conv1d_1_bias = new_bias2;
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