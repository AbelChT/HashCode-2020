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


int get_score(vector<unsigned int> *BOOKS_SCORES, // Input: Puntuación de cada libro
              vector<set<unsigned int>> *BOOKS_IN_LIBRARY, // Input: Libros en cada librería
              vector<unsigned int> *LIBRARIES_SIGN_UP_TIME, // Input: Tiempo de signup de cada libreria
              vector<unsigned int> *LIBRARIES_SHIP_TIME, // Input: Tiempo de ship de cada libreria
              int DAYS_FOR_SCAN, // Input: Dias para escanear
              vector<list<unsigned int>> *BOOKS_TO_PROCESS_IN_EACH_LIBRARY, // Output: Libros a procesar por cada libreria y orden,
              list<unsigned int> *LIBRARIES_PROCESSING_ORDER // Output: Orden de sign up de cada libreria
) {
    int total_score = 0;
    int init_time = 0;
    set<unsigned int> books_signed;

    for (auto i : (*LIBRARIES_PROCESSING_ORDER)) {
        init_time += LIBRARIES_SIGN_UP_TIME->at(i);
        int remaining_time = DAYS_FOR_SCAN - init_time;
        if (remaining_time > 0) {
            int books_to_ship = LIBRARIES_SHIP_TIME->at(i) * remaining_time;
            vector<unsigned int> actual_library_books(BOOKS_TO_PROCESS_IN_EACH_LIBRARY->at(i).size(), 0);
            int j = 0;
            for (auto k: BOOKS_TO_PROCESS_IN_EACH_LIBRARY->at(i)) {
                actual_library_books[j] = k;
                j = j + 1;
            }
            for (int j = 0; j < books_to_ship && j < BOOKS_TO_PROCESS_IN_EACH_LIBRARY->at(i).size(); ++j) {
                unsigned int it1 = actual_library_books[j];

                if (books_signed.find(it1) == books_signed.end()) {
                    total_score += BOOKS_SCORES->at(it1);
                    books_signed.insert(it1);
                }
            }
        }
    }

    return total_score;
}

void algorithm_simple_basic(vector<unsigned int> *BOOKS_SCORES, // Input: Puntuación de cada libro
                            vector<set<unsigned int>> *BOOKS_IN_LIBRARY, // Input: Libros en cada librería
                            vector<unsigned int> *LIBRARIES_SIGN_UP_TIME, // Input: Tiempo de signup de cada libreria
                            vector<unsigned int> *LIBRARIES_SHIP_TIME, // Input: Tiempo de ship de cada libreria
                            unsigned int DAYS_FOR_SCAN, // Input: Dias para escanear
                            vector<list<unsigned int>> *BOOKS_TO_PROCESS_IN_EACH_LIBRARY, // Output: Libros a procesar por cada libreria y orden,
                            list<unsigned int> *LIBRARIES_PROCESSING_ORDER // Output: Orden de sign up de cada libreria
) {

    for (size_t i = 0; i < BOOKS_IN_LIBRARY->size(); i++) {
        LIBRARIES_PROCESSING_ORDER->push_back(i);
        for (unsigned int x : BOOKS_IN_LIBRARY->at(i)) {
            BOOKS_TO_PROCESS_IN_EACH_LIBRARY->at(i).push_back(x);
        }

    }
}

void algorithm_random(vector<unsigned int> *BOOKS_SCORES, // Input: Puntuación de cada libro
                      vector<set<unsigned int>> *BOOKS_IN_LIBRARY, // Input: Libros en cada librería
                      vector<unsigned int> *LIBRARIES_SIGN_UP_TIME, // Input: Tiempo de signup de cada libreria
                      vector<unsigned int> *LIBRARIES_SHIP_TIME, // Input: Tiempo de ship de cada libreria
                      unsigned int DAYS_FOR_SCAN, // Input: Dias para escanear
                      vector<list<unsigned int>> *BOOKS_TO_PROCESS_IN_EACH_LIBRARY, // Output: Libros a procesar por cada libreria y orden,
                      list<unsigned int> *LIBRARIES_PROCESSING_ORDER // Output: Orden de sign up de cada libreria
) {

    int total_score = 0;

    for (int actual_iteration = 0; actual_iteration < 100; actual_iteration++) {
        vector<list<unsigned int>> BOOKS_TO_PROCESS_IN_EACH_LIBRARY_LOCAL(BOOKS_TO_PROCESS_IN_EACH_LIBRARY->size(),
                                                                          list<unsigned int>());
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();

        vector<unsigned int> LIBRARIES_PROCESSING_ORDER_LOCAL(BOOKS_IN_LIBRARY->size(), 0);
        for (size_t i = 0; i < BOOKS_IN_LIBRARY->size(); i++) {
            LIBRARIES_PROCESSING_ORDER_LOCAL[i] = i;
        }
        shuffle(LIBRARIES_PROCESSING_ORDER_LOCAL.begin(), LIBRARIES_PROCESSING_ORDER_LOCAL.end(),
                default_random_engine(seed));

        for (size_t i = 0; i < BOOKS_IN_LIBRARY->size(); i++) {
            vector<unsigned int> BOOKS_IN_LIBRARY_LOCAL(BOOKS_IN_LIBRARY->at(i).size(), 0);
            for (unsigned int x : BOOKS_IN_LIBRARY->at(i)) {
                BOOKS_IN_LIBRARY_LOCAL[i] = x;
            }
            shuffle(BOOKS_IN_LIBRARY_LOCAL.begin(), BOOKS_IN_LIBRARY_LOCAL.end(), default_random_engine(seed));
            for (auto j:BOOKS_IN_LIBRARY_LOCAL) {
                BOOKS_TO_PROCESS_IN_EACH_LIBRARY_LOCAL[i].push_back(j);
            }
        }

        // Pass to list
        list<unsigned int> LIBRARIES_PROCESSING_ORDER_LOCAL_LIST;

        for (auto j : LIBRARIES_PROCESSING_ORDER_LOCAL) {
            LIBRARIES_PROCESSING_ORDER_LOCAL_LIST.push_back(j);
        }

        int total_score_local = get_score(BOOKS_SCORES, // Input: Puntuación de cada libro
                                          BOOKS_IN_LIBRARY, // Input: Libros en cada librería
                                          LIBRARIES_SIGN_UP_TIME, // Input: Tiempo de signup de cada libreria
                                          LIBRARIES_SHIP_TIME, // Input: Tiempo de ship de cada libreria
                                          DAYS_FOR_SCAN, // Input: Dias para escanear
                                          &BOOKS_TO_PROCESS_IN_EACH_LIBRARY_LOCAL, // Output: Libros a procesar por cada libreria y orden,
                                          &LIBRARIES_PROCESSING_ORDER_LOCAL_LIST);

        if (total_score_local > total_score) {
            total_score = total_score_local;
        }


    }
    cout << "Score: " << total_score << endl;
}

unsigned int fit(unsigned int nB, unsigned int signup, unsigned int shiptime) {
    return (((1 / signup) + shiptime) / 2);
}

void algorithm_simple(vector<unsigned int> *BOOKS_SCORES, // Input: Puntuación de cada libro
                      vector<set<unsigned int> > *BOOKS_IN_LIBRARY, // Input: Libros en cada librería
                      vector<unsigned int> *LIBRARIES_SIGN_UP_TIME, // Input: Tiempo de signup de cada libreria
                      vector<unsigned int> *LIBRARIES_SHIP_TIME, // Input: Tiempo de ship de cada libreria
                      unsigned int DAYS_FOR_SCAN, // Input: Dias para escanear
                      vector<list<unsigned int> > *BOOKS_TO_PROCESS_IN_EACH_LIBRARY, // Output: Libros a procesar por cada libreria y orden,
                      list<unsigned int> *LIBRARIES_PROCESSING_ORDER // Output: Orden de sign up de cada libreria
) {

    std::vector<unsigned int>::iterator it;
    vector<unsigned int> fitness;
    vector<unsigned int> LIBRARIES_ORDERED;

    it = LIBRARIES_ORDERED.begin();
    for (size_t i = 0; i < BOOKS_IN_LIBRARY->size(); i++) {
        it = LIBRARIES_ORDERED.insert(it, i);
    }

    it = fitness.begin();
    for (size_t i = 0; i < BOOKS_IN_LIBRARY->size(); i++) {
        it = fitness.insert(it,
                            fit(BOOKS_IN_LIBRARY->size(), LIBRARIES_SIGN_UP_TIME->at(i), LIBRARIES_SHIP_TIME->at(i)));
    }


    int min, tmp1, tmp2;
    for (size_t i = 0; i < BOOKS_IN_LIBRARY->size(); i++) {
        min = i;
        for (size_t y = i + 1; y < BOOKS_IN_LIBRARY->size(); y++) {
            if (fitness[min] > fitness[y]) {
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
        LIBRARIES_PROCESSING_ORDER->push_front(LIBRARIES_ORDERED[i]);
        for (unsigned int x : BOOKS_IN_LIBRARY->at(LIBRARIES_ORDERED[i])) {
            BOOKS_TO_PROCESS_IN_EACH_LIBRARY->at(LIBRARIES_ORDERED[i]).push_front(x);
        }

    }
}


int main(int argc, const char *argv[]) {
    if (argc != 3) {
        cout << "Bad number of parameters" << endl;
        return -1;
    }

    // Load data
    int NUM_BOOKS;
    int NUM_LIBRARIES;
    int DAYS_FOR_SCAN;
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

    algorithm_random(&BOOKS_SCORES, // Input: Puntuación de cada libro
                     &BOOKS_IN_LIBRARY, // Input: Libros en cada librería
                     &LIBRARIES_SIGN_UP_TIME, // Input: Tiempo de signup de cada libreria
                     &LIBRARIES_SHIP_TIME, // Input: Tiempo de ship de cada libreria
                     DAYS_FOR_SCAN, // Input: Dias para escanear
                     &BOOKS_TO_PROCESS_IN_EACH_LIBRARY, // Output: Libros a procesar por cada libreria y orden,
                     &LIBRARIES_PROCESSING_ORDER);// Output: Orden de sign up de cada libreria);

    cout << "Time taken: " << clock() - tStart << endl;

    // Print score

    int total_score = get_score(&BOOKS_SCORES, // Input: Puntuación de cada libro
                                &BOOKS_IN_LIBRARY, // Input: Libros en cada librería
                                &LIBRARIES_SIGN_UP_TIME, // Input: Tiempo de signup de cada libreria
                                &LIBRARIES_SHIP_TIME, // Input: Tiempo de ship de cada libreria
                                DAYS_FOR_SCAN, // Input: Dias para escanear
                                &BOOKS_TO_PROCESS_IN_EACH_LIBRARY, // Output: Libros a procesar por cada libreria y orden,
                                &LIBRARIES_PROCESSING_ORDER);

    cout << "Score: " << total_score << endl;

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