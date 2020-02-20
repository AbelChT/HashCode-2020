#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include <tuple>
#include <algorithm>
#include <random>
#include <chrono>
#include <string>
#include <string.h>

using namespace std;

void fits(int nB, int signup, int shiptime){

    
}

void algorithm_simple(vector<unsigned int> *BOOKS_SCORES, // Input: Puntuación de cada libro
                      vector<set<unsigned int>> *BOOKS_IN_LIBRARY, // Input: Libros en cada librería
                      vector<unsigned int> *LIBRARIES_SIGN_UP_TIME, // Input: Tiempo de signup de cada libreria
                      vector<unsigned int> *LIBRARIES_SHIP_TIME, // Input: Tiempo de ship de cada libreria
                      unsigned int DAYS_FOR_SCAN, // Input: Dias para escanear
                      vector<list<unsigned int>> *BOOKS_TO_PROCESS_IN_EACH_LIBRARY, // Output: Libros a procesar por cada libreria y orden,
                      list<unsigned int> *LIBRARIES_PROCESSING_ORDER // Output: Orden de sign up de cada libreria
) {

    vector<unsigned int> fitness;
    vector<unsigned int> LIBRARIES_ORDERED;

    for (size_t i = 0; i < BOOKS_IN_LIBRARY->size(); i++)
    {
        LIBRARIES_ORDERED.push_front(i);
    }

    for (size_t i = 0; i < BOOKS_IN_LIBRARY->size(); i++)
    {
       fitness.push_front(fit(BOOKS_IN_LIBRARY[i], LIBRARIES_SIGN_UP_TIME[i], LIBRARIES_SHIP_TIME[i]));
    }



  int x, y, min, tmp1, tmp2;
  for(x = 0; x < n; x++) {

  }

    for (size_t i = 0; i < BOOKS_IN_LIBRARY->size(); i++){

    min = i;
    for(y = i + 1; y < BOOKS_IN_LIBRARY->size(); y++) {
      if(fitness[min] > fitness[y]) {
        min = y;
      }
    }
    tmp1 = fitness[min];
    tmp2 = LIBRARIES_ORDERED[min];
    fitness[min] = fitness[i];
    fitness[i] = tmp1;
    LIBRARIES_ORDERED[min] = LIBRARIES_ORDERED[i];
    LIBRARIES_ORDERED[i] = tmp2;
    }

    for (size_t i = 0; i < BOOKS_IN_LIBRARY->size(); i++) {
        LIBRARIES_PROCESSING_ORDER->push_back(LIBRARIES_ORDERED[i]);
        for (unsigned int x : BOOKS_IN_LIBRARY->at(LIBRARIES_ORDERED[i])) {
            BOOKS_TO_PROCESS_IN_EACH_LIBRARY->at(LIBRARIES_ORDERED[i]).push_back(x);
        }

    }
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
    ifstream input_file;

    input_file.open(argv[1]);
    if (!input_file.is_open()) {
        cout << "Can't read the file" << endl;
        return -1;
    }
    input_file >> NUM_BOOKS;
    input_file >> NUM_LIBRARIES;
    input_file >> DAYS_FOR_SCAN;
    //input_file >> trash; // end of line

    vector<unsigned int> BOOKS_SCORES(NUM_BOOKS, 0);
    vector<set<unsigned int>> BOOKS_IN_LIBRARY(NUM_LIBRARIES, set<unsigned int>());
    vector<unsigned int> LIBRARIES_SIGN_UP_TIME(NUM_LIBRARIES, 0);
    vector<unsigned int> LIBRARIES_SHIP_TIME(NUM_LIBRARIES, 0);

    // 2nd line
    for (int i = 0; i < NUM_BOOKS; i++) {
        input_file >> BOOKS_SCORES[i];
    }

    //input_file >> trash;

    for (int i = 0; i < NUM_LIBRARIES; i++) {
        // 3rd line
        int books_in_this_library;
        input_file >> books_in_this_library;
        input_file >> LIBRARIES_SIGN_UP_TIME[i];
        input_file >> LIBRARIES_SHIP_TIME[i];

        //input_file >> trash; // end of line

        // 4rd line
        int book;
        for (int j = 0; j < books_in_this_library; j++) {
            input_file >> book;
            BOOKS_IN_LIBRARY[i].insert(book);
        }
    }

    input_file.close();


    // OUTPUT
    vector<list<unsigned int>> BOOKS_TO_PROCESS_IN_EACH_LIBRARY(NUM_LIBRARIES, list<unsigned int>());
    list<unsigned int> LIBRARIES_PROCESSING_ORDER;

    // Execute algorithm
    clock_t tStart = clock();

    algorithm_simple(&BOOKS_SCORES, // Input: Puntuación de cada libro
                     &BOOKS_IN_LIBRARY, // Input: Libros en cada librería
                     &LIBRARIES_SIGN_UP_TIME, // Input: Tiempo de signup de cada libreria
                     &LIBRARIES_SHIP_TIME, // Input: Tiempo de ship de cada libreria
                     DAYS_FOR_SCAN, // Input: Dias para escanear
                     &BOOKS_TO_PROCESS_IN_EACH_LIBRARY, // Output: Libros a procesar por cada libreria y orden,
                     &LIBRARIES_PROCESSING_ORDER);// Output: Orden de sign up de cada libreria);

    cout << "Time taken: " << clock() - tStart << endl;

    // Print score
//    unsigned int total_score = 0;
//    set<unsigned int> books_signed;
//
//    for (auto i: BOOKS_TO_PROCESS_IN_EACH_LIBRARY) {
//        for (auto j: i) {
//            books_signed.insert(j);
//        }
//    }
//    for (auto i : books_signed) {
//        total_score += BOOKS_SCORES[i];
//    }
//
//    cout << "Score: " << total_score << endl;

    // Save output
    ofstream output_file;
    output_file.open(argv[2]);
    if (output_file.is_open()) {
        output_file << LIBRARIES_PROCESSING_ORDER.size() << endl;
        for (auto i: LIBRARIES_PROCESSING_ORDER) {
            output_file << i << " " << BOOKS_TO_PROCESS_IN_EACH_LIBRARY[i].size() << endl;
            for (auto j: BOOKS_TO_PROCESS_IN_EACH_LIBRARY[i]) {
                output_file << j << " ";
            }
            output_file << endl;
        }

    } else {
        cout << "Can't save the file" << endl;
        return -1;
    }

    output_file.close();

    return 0;

}