#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <tuple>
#include <algorithm>
#include <random>
#include <chrono>
#include <string>
#include <string.h>

using namespace std;

void algorithm_simple(vector<unsigned int> *BOOKS_SCORES, // Input: Puntuación de cada libro
                      vector<set<unsigned int>> *BOOKS_IN_LIBRARY, // Input: Libros en cada librería
                      vector<unsigned int> *LIBRARIES_SIGN_UP_TIME, // Input: Tiempo de signup de cada libreria
                      vector<unsigned int> *LIBRARIES_SHIP_TIME, // Input: Tiempo de ship de cada libreria
                      unsigned int DAYS_FOR_SCAN, // Input: Dias para escanear
                      vector<set<unsigned int>> *BOOKS_TO_PROCESS_IN_EACH_LIBRARY // Output: Libros a procesar por cada libreria
                      ) {
//    unsigned int N = slices_in_each_type_of_pizza->size();
//    unsigned int number_of_slices = 0;
//
//    for (unsigned int i = 0; i < N && number_of_slices <= M; i++) {
//        number_of_slices += slices_in_each_type_of_pizza->at(i);
//        if (number_of_slices <= M) {
//            types_of_pizza_to_order->insert(i);
//        }
//    }
}

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        cout << "Bad number of parameters" << endl;
        return -1;
    }

    // Load data
    unsigned int NUM_BOOKS;
    unsigned int NUM_LIBRARIES;
    unsigned int DAYS_FOR_SCAN;
    char trash, books_in_this_library;
    ifstream input_file;

    input_file.open(argv[1]);
    if (!input_file.is_open()) {
        cout << "Can't read the file" << endl;
        return -1;
    }
    input_file >> NUM_BOOKS;
    input_file >> NUM_LIBRARIES;
    input_file >> DAYS_FOR_SCAN;
    input_file >> trash; // end of line

    vector<unsigned int> BOOKS_SCORES(NUM_BOOKS, 0);
    vector<set<unsigned int>> BOOKS_IN_LIBRARY(NUM_LIBRARIES, set<unsigned int>());
    vector<unsigned int> LIBRARIES_SIGN_UP_TIME(NUM_LIBRARIES, 0);
    vector<unsigned int> LIBRARIES_SHIP_TIME(NUM_LIBRARIES, 0);

    // 2nd line
    for (int i = 0; i < NUM_BOOKS; i++) {
        input_file >> BOOKS_SCORES[i];
    }
    input_file >> trash;
    for(int j=0; !input_file.eof(); j++){
	    // 3rd line
	    input_file >> books_in_this_library;
	    input_file >> LIBRARIES_SIGN_UP_TIME[j];
	    input_file >> LIBRARIES_SHIP_TIME[j];

	    input_file >> trash; // end of line

	    // 4rd line
	    int book;
	    for (int i = 0; i < books_in_this_library; i++) {
	        input_file >> book;
	        BOOKS_IN_LIBRARY[j].insert(book);
	    }
	    input_file >> trash;
	}

    input_file.close();


    // OUTPUT
    vector<set<unsigned int>> BOOKS_TO_PROCESS_IN_EACH_LIBRARY(NUM_LIBRARIES, set<unsigned int>());

    // Execute algorithm
    clock_t tStart = clock();
    algorithm_simple(&slices_in_each_type_of_pizza, &types_of_pizza_to_order, M);
    cout << "Time taken: " << clock() - tStart << endl;

    // Print score
    unsigned int total_score = 0;
    for (auto i : types_of_pizza_to_order) {
        total_score += slices_in_each_type_of_pizza[i];
    }

    if (total_score <= M) {
        cout << "Score: " << total_score << endl;
    } else {
        cout << "Score: " << "Fail" << endl;
    }

    // Save output
    ofstream output_file;
    output_file.open(argv[2]);
    if (output_file.is_open()) {
        output_file << types_of_pizza_to_order.size() << endl;
        for (auto i: types_of_pizza_to_order) {
            output_file << i << " ";
        }
        output_file << endl;
    } else {
        cout << "Can't save the file" << endl;
        return -1;
    }
    output_file.close();

    return 0;