#include <vector>
#include <random>
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

// Generates a set (size n) of task utilizations, 
// using Bini's UUniFast algorithm
vector<vector<float> > UUniFast(const size_t n, const size_t num_sets, const float U_mean) {

    vector<vector<float> > out;
    float sumU = U_mean;
    float nextSumU;
    float r, exp;
    vector<float> curr_set;
    size_t s = 0;
    
    while (s < num_sets) {

        curr_set.clear();

        for (int i = 1; i < n; i++) {
            // Generates a random float in range [0.0,1.0]
            r = (float) rand() / (float) RAND_MAX;
            exp = (1.0/(n-i));
            nextSumU = sumU * pow(r,exp);
            curr_set.push_back(sumU-nextSumU);
            sumU = nextSumU;
        }
        curr_set.push_back(sumU);

        // Make sure all utilizations in the set are <=1 before adding list to the output list
        if (all_of(curr_set.begin(), curr_set.end(), [](float f){return f<=1;})){
            out.push_back(curr_set);
        }
    }

    return out;
}

// Randomly generates task periods for num_sets task sets, each with n tasks
vector<vector<unsigned int> > gen_periods(const size_t n, const size_t num_sets, const unsigned int min, const unsigned int max) {

    vector<vector<unsigned int> > out;
    vector<unsigned int> curr_set;
    float r;

    for (size_t s = 0; s < num_sets; s++) {
        for (size_t i = 0; i < n; i++) {
            // Generates a random float in range [min,max]
            r = ( min + (float) rand() ) / ((float) (RAND_MAX/(max-min))); 
            curr_set.push_back((int) r);
        }
    }
}

int main() {
    srand(static_cast<unsigned int> (time(nullptr)));
    return 0;
}