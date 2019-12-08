#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <math.h>

class GenParabolaData
{
public:
    GenParabolaData(int low, int high, int size)
    : random_engine_{std::random_device{}()}
    , distribution_{low, high}

    {
        for (int j=0 ; j < size; j++)
        {
            numbers.push_back(std::vector<int>{j, j*j+distribution_(random_engine_)});
        }
    }

    std::vector<std::vector<int>> operator()()
    {
        return numbers;
    }
private:
    std::mt19937 random_engine_;
    std::uniform_int_distribution<int> distribution_;
    std::vector<std::vector<int>> numbers;
};


class FcNn
{
public:
    FcNn(int epochs, int n_neurons, std::vector<std::vector<int>> array)
    : epochs{epochs}
    , array{array}
    , weights{std::vector<double>{0.1, 0.1}}
    , lr{0.1}
    , n_neurons{n_neurons}
    , neurons{construct_weights()}
    , out_bias{0.1}


    {
    }

    std::vector<std::vector<double>> construct_weights()
        {
        std::vector<std::vector<double>> neurons;
        for (int j=0 ; j < n_neurons; j++)
            {
            neurons.push_back(std::vector<double>{0.1, 0.1, 0.1});
            }
        return neurons;

        }

    void fit()
    {
    for (int epoch=1; epoch<=epochs; epoch++)
         {
         for (std::vector<int> X : array)
             {
             std::vector<double> forwards = forwards_(X[0]);
             double pred = forwards.back();
             double sig_end = (X[1]-pred)*pred*(1-pred);
             out_bias = out_bias + lr*sig_end;
             for (ulong i=0; i<neurons.size(); i++)
                 {
                 double delta_end = lr*sig_end*forwards[i];
                 neurons[i][2] = neurons[i][2] + delta_end;

                 double sig_hidden = forwards[i]*(1-forwards[i])*sig_end*neurons[i][2];
                 double delta_hidden = lr*sig_hidden;
                 neurons[i][0] = neurons[i][0] + delta_hidden;
                 neurons[i][1] = neurons[i][1] +delta_hidden;
                 }
              }
         }
    }

    std::vector<double> forwards_(double input)
    {
        std::vector<double> forwards;
        for (std::vector<double> neuron : neurons)
           {
            double output = 1/(1+exp(input*neuron[0]+neuron[1]));
            forwards.push_back(output);
           }
        double pred = 0;
        for (ulong i=0 ; i < forwards.size(); i++)
            {pred += neurons[i][2]*forwards[i]+out_bias;}
        pred = 1/(1+exp(pred));
        forwards.push_back(pred);
        return forwards;
    }

private:
    int epochs;
    std::vector<std::vector<int>> array;
    std::vector<double> weights;
    double lr;
    int n_neurons;
    std::vector<std::vector<double>> neurons;
    double out_bias;
};

int main()
{
    GenParabolaData parabola_data = GenParabolaData(-5, 5, 500);
    FcNn fc_nn = FcNn(1, 5, parabola_data());
    fc_nn.fit();

    for (std::vector<int> X : parabola_data())
    {std::vector<double> forwards = fc_nn.forwards_(X[0]);
     double pred = forwards.back();
     std::cout << pred << std::endl;
    }
}
