#include "utils.h"

#include <iostream>
#include <fstream>
#include <random>
#include <unistd.h>

using namespace std;


int main(int argc, char * argv[]) {
    auto option = getopt(argc, argv, "n:");

    int n = 0;
    if (option == 'n') {
        n = atoi(optarg);
    }
    else {
        cerr << "Usage: " << argv[0] << " -n <number of integers to generate>" << endl;
        exit(E_INVALID_ARGS);
    }

    bool in_pipe = isatty(fileno(stdout)) == 0;

    mt19937 mt_rand(static_cast<unsigned long long>(time(0)));
    {
        ofstream file;
        ostream * out = nullptr;
        if (in_pipe) {
            out = &cout;
        }
        else {
            file.open("data.bin", ios::out | ios::binary);
            if (!file) {
                cerr << "Error while opening file" << endl;
                exit(E_FAILURE);
            }
            out = &file;
        }

        while (n) {
            auto random_number = static_cast<uint32_t>(mt_rand());
            utils::binary_write(out, random_number);
            --n;
        }

    } // file closed

    return E_OK;
}