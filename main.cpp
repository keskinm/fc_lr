#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

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


int main()
{
    GenParabolaData parabola_data = GenParabolaData(-5, 5, 500);
}
