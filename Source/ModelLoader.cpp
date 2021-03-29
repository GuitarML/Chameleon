#include "ModelLoader.h"
#include <iostream>

#include <nlohmann/json.hpp>

//==============================================================================
ModelLoader::ModelLoader()
{
}

nc::NdArray<float> ModelLoader::vector_to_nc(std::vector<float> in_vec)
//====================================================================
// Description: Converts a std::vector to a numcpp array.
//====================================================================
{
    nc::NdArray<float> out_vec = nc::random::rand<float>(nc::Shape(1, in_vec.size()));;

    for (int i = 0; i < in_vec.size(); i++)
    {
        out_vec[i] = in_vec[i]; 
    }
    return out_vec;
}

nc::NdArray<float> ModelLoader::matrix_to_nc(std::vector<std::vector<float>> in_mat)
//====================================================================
// Description: Converts a std::vector<std::vector> (a 2d matrix) 
//              to a numcpp array.
//====================================================================
{
    nc::NdArray<float> out_mat = nc::random::rand<float>(nc::Shape(in_mat.size(), in_mat[0].size()));

    for (int i = 0; i < in_mat.size(); i++)
    {
        for (int j = 0; j < in_mat[0].size(); j++)
        {
            out_mat(i, j) = in_mat[i][j]; 
        }
    }
    return out_mat;
}


void ModelLoader::load_json(const char *filename)
{
    // read a JSON file
    std::ifstream i2(filename);
    nlohmann::json weights_json;
    i2 >> weights_json;

    hidden_size = weights_json["/model_data/hidden_size"_json_pointer];

    std::vector<float> lstm_bias_ih = weights_json["/state_dict/rec.bias_ih_l0"_json_pointer]; 
    std::vector<std::vector<float>> lstm_weights_ih = weights_json["/state_dict/rec.weight_ih_l0"_json_pointer];
    lstm_bias_ih_nc = vector_to_nc(lstm_bias_ih);
    lstm_weights_ih_nc = matrix_to_nc(lstm_weights_ih).transpose();


    std::vector<float> lstm_bias_hh = weights_json["/state_dict/rec.bias_hh_l0"_json_pointer];
    std::vector<std::vector<float>> lstm_weights_hh = weights_json["/state_dict/rec.weight_hh_l0"_json_pointer];
    lstm_bias_hh_nc = vector_to_nc(lstm_bias_hh);
    lstm_weights_hh_nc = matrix_to_nc(lstm_weights_hh).transpose();

    std::vector<float> dense_bias = weights_json["/state_dict/lin.bias"_json_pointer];
    std::vector<std::vector<float>> dense_weights = weights_json["/state_dict/lin.weight"_json_pointer];
    dense_bias_nc = vector_to_nc(dense_bias).transpose();
    dense_weights_nc = matrix_to_nc(dense_weights);

    // Add lstm weights for later use
    lstm_bias_nc = lstm_bias_hh_nc + lstm_bias_ih_nc;

}