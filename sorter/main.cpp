#include "utils.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <unistd.h>

using namespace std;


string tmp_filename(size_t index) {
    return "/tmp/chunk_" + to_string(index) + ".bin";
}


int main(int argc, char * argv[]) {
    auto option = getopt(argc, argv, "m:");

    size_t m = 0;
    if (option == 'm') {
        m = static_cast<size_t>(atoi(optarg));
    }
    else {
        cerr << "Usage: " << argv[0] << " -m <max number of integers to store in RAM>" << endl;
        exit(E_INVALID_ARGS);
    }

    size_t temporary_file_count = 0;

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

        size_t file_index = 0;
        vector<integer> buffer(m);
        while (in->good()) {
            utils::binary_read(in, buffer);
            buffer.resize(in->gcount() / sizeof(integer));
            sort(buffer.begin(), buffer.end());
            ofstream temp_file(tmp_filename(file_index++), ios::out | ios::binary);
            utils::binary_write(temp_file, buffer);
        }
        temporary_file_count = file_index;
    }

    {   /** merge sorted data **/
        bool in_pipe = isatty(fileno(stdout)) == 0;

        ofstream file;
        ostream * out = nullptr;
        if (in_pipe) {
            out = &cout;
        } else {
            file.open("data_sorted.bin", ios::out | ios::binary);
            if (!file) {
                cerr << "Error while opening file" << endl;
                exit(E_FAILURE);
            }
            out = &file;
        }

        vector<ifstream> temp_files(temporary_file_count);
        for (size_t index = 0; index < temp_files.size(); ++index) {
            auto f = tmp_filename(index);
            temp_files[index].open(tmp_filename(index), ios::in | ios::binary);
            if (!temp_files[index]) {
                cerr << "Error while opening file" << endl;
                exit(E_FAILURE);
            }
        }

        using file_iterator = utils::binary_iterator<integer, ifstream>;

        vector<file_iterator> iterators;
        iterators.reserve(temporary_file_count);
        for (auto & temp_file : temp_files) {
            iterators.push_back(move(file_iterator(temp_file)));
        }

        bool done = false;
        while (!done) {
            integer min = numeric_limits<integer>::max();
            size_t index_of_min = iterators.size();
            for (size_t iterator_index = 0; iterator_index < iterators.size(); ++iterator_index) {
                auto & iterator = iterators[iterator_index];
                if (!iterator) {
                    continue;
                }
                auto value = *iterator;
                if (value <= min) {
                    min = value;
                    index_of_min = iterator_index;
                }
            }
            if (index_of_min < iterators.size()) {
                ++iterators[index_of_min];
                utils::binary_write(out, min);
            }
            else {
                done = true;
            }
        }
    }

    return E_OK;
}