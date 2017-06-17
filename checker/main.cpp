#include "utils.h"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <limits>

using namespace std;

int main() {
    {   /** generate temporary files with sorted chunks **/
        bool in_pipe = isatty(fileno(stdin)) == 0;

        ifstream file;
        istream * in = nullptr;
        if (in_pipe) {
            in = &cin;
        } else {
            file.open("data.bin", ios::in | ios::binary);
            if (!file) {
                cerr << "Error while opening file" << endl;
                exit(E_FAILURE);
            }
            in = &file;
        }

        integer prev = numeric_limits<integer>::min();
        integer num;
        size_t count = 0;
        while (utils::binary_read(in, num)) {
            if (num < prev) {
                cerr << "File is not sorted" << endl;
                exit(E_FAILURE);
            }
            prev = num;
            ++count;
        }
        cout << "Successfully checked " << count << " integers" << endl;
    }
    return 0;
}