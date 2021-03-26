import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
import json

#from torch.nn import Sigmoid
from tensorflow.keras.activations import sigmoid


# Reference:
# https://dtransposed.github.io/blog/Under-the-hood-LSTM.html


## Define parameters ##

no_of_layers = 1        # Number of hidden LSTM layers in the network. (for this application will always be 1)
no_of_units = 32        # Number of units in every LSTM layer.

test_length = 4410    # Number of samples to test (44100 is one second of audio)

def save_wav(name, data):
    if name.endswith('.wav') == False:
        name = name + '.wav'
    wavfile.write(name, 44100, data.flatten().astype(np.float32))
    print("Predicted wav file generated: "+name)

def read_wav(wav_file):
    # Extract Audio and framerate from Wav File
    fs, signal = wavfile.read(wav_file)
    return signal, fs
    
## Define LSTM networks ##

class custom_LSTM(object):
    ''' 
    custom_LSTM creates a single LSTM layer ('custom-made' implementation)
    '''
    def __init__(self, input_size, no_of_units):
        self.input_size = input_size            
        self.hidden_units = no_of_units         # c++ code needs to handle a variable number of hidden_units (default is 32)
        self.hidden = np.zeros((self.input_size, self.hidden_units),dtype = np.float32)
        self.cell_state = np.zeros((self.input_size, self.hidden_units),dtype = np.float32)
        self.output_array = []
        self.residual = 0

    def tanh(self, x):
        return np.tanh(x)

    def sigmoid(self, z):
        return 1/(1 + np.exp(-z))
        
    def layer(self, xt, W, U, b):
        # This is the main function needed to be converted to c++ (the conv1d layer is already mostly implemented in c++ already)
        #
        # Optimized Version ( 2 bigger matrix multiplications instead of 8 smaller matrix multiplications ) 
        # Reference "Algorithm 1" on page 5 of research paper, that version
        #   might be easier to implement in c++.  It does the same thing as below, but it
        #   updates each index "for each i in [0 -> N"]) where "N" is the hidden size (self.hidden_units)
        #      Ignore line 2 from Algorithm 1 "if b_cond was given..", not implemented here
        #
        # The below matrix shapes are the default for the ts9_model.h5, the sizes are determined by the parameters listed beside the defaults
        # The c++ code will need to be able to account for variations in these parameters ("filters" and "hidden_units" can vary depending on the model parameters)
        #
        #   xt shape = (1,)        (filters,) <-- the number of filters in the conv1d_1 layer
        #   W shape =  (1, 128)    (filters, hidden_units*4) <-- the number of filters in the conv1d_1 layer, and hidden_units of LSTM * 4
        #   U shape =  (32, 128)    (hidden_units, hidden_units*4) <-- hidden_units in LSTM, and hidden_units of LSTM * 4
        #   bias shape = (128,)     (hidden_units*4) <-- hidden_units of LSTM * 4

        self.residual = xt # residual is for skip connection

        HS = self.hidden_units # Hidden size
        hidden = self.hidden
        bias = b
        gates = np.dot(xt, W) + np.dot(hidden, U) + bias
        i_t, f_t, g_t, o_t = (
            sigmoid(gates[:, :HS]), # input  
            sigmoid(gates[:, HS:HS*2]), # forget
            self.tanh(gates[:, HS*2:HS*3]), 
            sigmoid(gates[:, HS*3:]), # output
        )
        c_t = f_t * self.cell_state + i_t * g_t # Cell state
        h_t = o_t * self.tanh(c_t) # Hidden state
        self.hidden = h_t     # c_t and h_t assignments written out for clarity, can just update the self.hidden and self.cell states without using c_t and h_t
        self.cell_state = c_t

        return self.hidden

    def reset_state(self):
        self.hidden = np.zeros((self.input_size, self.hidden_units),dtype = np.float32)
        self.cell_state = np.zeros((self.input_size, self.hidden_units),dtype = np.float32)
        
    def dense(self, x, weights, bias):
        result = np.dot(x, weights)+bias
        self.result=result[0] + self.residual ## This is where I'm adding the residual skip connection
        return result[0] + self.residual  ## This is where I'm adding the residual skip connection

    def output_array_append(self):
        self.output_array.append(self.result[0]) 



## Main ##

########################################################### Load and process the audio input data
in_rate, in_data = wavfile.read('x_test1.wav')
X = in_data.astype(np.float32).flatten()[0:test_length]  #read input wav file to np array

########################################################### Load json model 
with open('ts9_model_best.json') as json_file:
    data = json.load(json_file)

# Transpose to match current custom implementation made for Keras
weight_ih_l0 = np.array(data['state_dict']['rec.weight_ih_l0']).T
weight_hh_l0 = np.array(data['state_dict']['rec.weight_hh_l0']).T
bias_ih_l0 = np.array(data['state_dict']['rec.bias_ih_l0']).T
bias_hh_l0 = np.array(data['state_dict']['rec.bias_hh_l0']).T

lin_weight = np.array(data['state_dict']['lin.weight']).T
lin_bias = np.array(data['state_dict']['lin.bias'])

print("Model Data: ", data['model_data'])
print("rec.weight_ih_l0 shape:", weight_ih_l0.shape)
print("rec.weight_hh_l0 shape:", weight_hh_l0.shape)
print("rec.bias_ih_l0 shape:", bias_ih_l0.shape)
print("rec.bias_hh_l0 shape:", bias_hh_l0.shape)
print("lin.weight shape:", lin_weight.shape)
print("lin.bias:", lin_bias.shape)

input_size =  int(data['model_data']['input_size'])
hidden_units = int(data['model_data']['hidden_size'])

print("input, hidden size:", input_size, hidden_units)

LSTM_layer_1 = custom_LSTM(input_size, hidden_units)


## Prediction step using custom-made LSTM ##
LSTM_layer_1.reset_state()  #initialize the hidden and cell states for the LSTM (set to 0's)  TODO WHEN SHOULD HIDDEN AND CELL STATES BE INITIALIZED?
for sample in X:

    # Layer 1: LSTM (Long short term memory layer)
    output_from_LSTM_1 = LSTM_layer_1.layer(sample, weight_ih_l0, weight_hh_l0, bias_ih_l0 + bias_hh_l0)  #Just add the biases at the beginning
    #print("Shape of output_from_LSTM_1", output_from_LSTM_1.shape)
    
    # Layer 2: Dense layer (Fully connected layer)
    LSTM_layer_1.dense(output_from_LSTM_1, lin_weight, lin_bias)
    LSTM_layer_1.output_array_append()

## Compare custom-made implementation  ##
    
result_custom = LSTM_layer_1.output_array
save_wav("custom_lstm.wav", np.array(result_custom))

# Load baseline wav for ts9 from pytorch model
signal, fs = read_wav('ts9_best_out.wav')
test_signal = signal[0:test_length]

test_input = in_data[0:test_length]

dummy_axis = list(range(len(result_custom)))
plt.plot(dummy_axis, result_custom, label ='Custom-made LSTM', marker='x')
plt.plot(dummy_axis, test_signal, label ='Baseline Pytorch')
plt.plot(dummy_axis, test_input, label ='Input')
plt.legend(loc='best')
plt.xlabel('Data point')
plt.ylabel('Value')
plt.title('Comparison of two methods')
plt.show()
