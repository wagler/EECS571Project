#include <random>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <functional>

using namespace std;

// Generates a set (size n) of task utilizations, 
// using Bini's UUniFast algorithm
 void UUniFast( const size_t num_tasks, 
                const size_t num_sets, 
                const float U_mean,
                float* utils   ) {

    float sumU, nextSumU, r;
    bool pass = true;

    size_t set_count = 0;
    
    while (set_count < num_sets) {
        sumU = U_mean;
        for (size_t i = 1; i < num_tasks; i++) {
            // Generates a random float in range [0.0,1.0]
            r = (float) rand() / (float) RAND_MAX;
            nextSumU = sumU * pow(r, (1.0/(num_tasks-i)) );
            
            utils[set_count*num_tasks + i] = sumU - nextSumU;
            sumU = nextSumU;
        }
        utils[set_count*num_tasks] = sumU; // set element 0

        // Make sure all utilizations in the set are <=1 before adding list to the output list
        pass = true;
        for (size_t j = 0; j < num_tasks; j++) {
            if (utils[set_count*num_tasks + j] > 1) {
                pass = false;
            }
        }
        if (pass) set_count++;
    }
}

// Randomly generates (uniform distribution) task periods for task sets
void gen_periods(   const size_t num_tasks, // number of tasks per set
                    const size_t num_sets, 
                    const unsigned int min, // range's min task period
                    const unsigned int max, // range's max task period
                    unsigned int* periods  ) {

    unsigned int r;

    for (size_t s = 0; s < num_sets; s++) {
        for (size_t t = 0; t < num_tasks; t++) {
            // Generates a random float in range [min,max]
            r = (unsigned int) (rand() % max + min);
            periods[s*num_tasks + t] = r;
        }
    }
}

// Calculates the runtimes of each task, in each set, 
// using the pre-generated periods and utilizations
// The period and utilization vectors are assumed to be the same size
void gen_runtimes(  const size_t num_sets,
                    const size_t num_tasks,
                    const float* utils, 
                    const unsigned int* periods,
                    float* runtimes   ) {

    // Multiply corresponding elements of utils and periods
    size_t idx;
    for (size_t s = 0; s < num_sets; s++) {
        for (size_t t = 0; t < num_tasks; t++) {
            idx = s*num_tasks + t;
            runtimes[idx] = utils[idx] * periods[idx];
        }
    }
}

int main(int argv, char** argc) {

    if (argv != 7) {
        cout << "Must provide number of task sets, number of tasks per set, mean task set utilization, minimum task period, maximum task period, and an output file path." << endl;
        cout << "UUniFast [num sets] [num tasks] [mean U] [min period] [max period] [output path]" << endl;

        return 1;
    }

    const unsigned int num_sets = atoi(argc[1]);
    const unsigned int num_tasks = atoi(argc[2]);
    const float U_mean = atof(argc[3]);
    const unsigned int min_period = atoi(argc[4]);
    const unsigned int max_period = atoi(argc[5]);
    const string filepath(argc[6]);

    unsigned int* periods = new unsigned int[num_sets*num_tasks];
    float* runtimes = new float[num_sets*num_tasks];
    float* utilizations = new float[num_sets*num_tasks];

    // Generate seed for random number generator
    srand(static_cast<unsigned int> (time(nullptr)));

    UUniFast(num_tasks, num_sets, U_mean, utilizations);
    gen_periods(num_tasks, num_sets, min_period, max_period, periods);
    gen_runtimes(num_sets, num_tasks, utilizations, periods, runtimes);

    // Write data to file. 
    // Each line will represent a task with format "runtime,period\n"
    // Blank lines will seperate task sets
    size_t idx;
    ofstream outfile (filepath);
    if (outfile.is_open()) {
        outfile << "runtime,period,utilization" << endl;
        for (size_t i = 0; i < num_sets; i++) {
            for (size_t j = 0; j < num_tasks; j++) {
                idx = i*num_tasks + j;
                outfile << runtimes[idx] << "," << periods[idx] << "," << utilizations[idx] << "\n";
            }
            outfile << "\n";
        }
        outfile.close();
    }

    delete[] utilizations;
    delete[] runtimes;
    delete[] periods;
    return 0;
}