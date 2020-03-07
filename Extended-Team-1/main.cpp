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
#include <utility>

using namespace std;

int get_score(const vector<int> &BOOKS_SCORES, // Input: Puntuación de cada libro
              const vector<set<int>> &BOOKS_IN_LIBRARY, // Input: Libros en cada librería
              const vector<int> &LIBRARIES_SIGN_UP_TIME, // Input: Tiempo de signup de cada libreria
              const vector<int> &LIBRARIES_SHIP_TIME, // Input: Tiempo de ship de cada libreria
              const int DAYS_FOR_SCAN, // Input: Dias para escanear
              const vector<vector<int>> &BOOKS_TO_PROCESS_IN_EACH_LIBRARY, // Output: Libros a procesarporcadalibreriayorden,
              const vector<int> &LIBRARIES_PROCESSING_ORDER // Output: Orden de sign up de cada libreria
) {
    int total_score = 0;
    int init_time = 0;
    set<int> books_signed;

    for (auto i : LIBRARIES_PROCESSING_ORDER) {
        init_time += LIBRARIES_SIGN_UP_TIME[i];
        int remaining_time = DAYS_FOR_SCAN - init_time;

        if (remaining_time > 0) {
            int books_to_ship = LIBRARIES_SHIP_TIME[i] * remaining_time;

            for (int j = 0; j < books_to_ship && j < BOOKS_TO_PROCESS_IN_EACH_LIBRARY[i].size(); ++j) {
                unsigned int actual_book = BOOKS_TO_PROCESS_IN_EACH_LIBRARY[i][j];

                if (books_signed.find(actual_book) == books_signed.end()) {
                    total_score += BOOKS_SCORES[actual_book];
                    books_signed.insert(actual_book);
                }
            }
        }
    }

    return total_score;
}

pair<vector<vector<int>>, vector<int>> algorithm_random_executor(const vector<int> &BOOKS_SCORES,
                                                                 const vector<set<int>> &BOOKS_IN_LIBRARY,
                                                                 const vector<int> &LIBRARIES_SIGN_UP_TIME,
                                                                 const vector<int> &LIBRARIES_PARALLEL_CAPACITIES) {
    vector<vector<int>> BOOKS_TO_PROCESS_IN_EACH_LIBRARY; // Output: Libros a procesar por cada libreria y orden
    vector<int> LIBRARIES_PROCESSING_ORDER; // Output: Orden de sign up de cada libreria

    for (int i = 0; i < BOOKS_IN_LIBRARY.size(); i++) {
        LIBRARIES_PROCESSING_ORDER.push_back(i);

        BOOKS_TO_PROCESS_IN_EACH_LIBRARY.emplace_back(vector<int>());

        for (int x : BOOKS_IN_LIBRARY[i]) {
            BOOKS_TO_PROCESS_IN_EACH_LIBRARY[i].emplace_back(x);
        }

        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(BOOKS_TO_PROCESS_IN_EACH_LIBRARY[i].begin(), BOOKS_TO_PROCESS_IN_EACH_LIBRARY[i].end(),
                default_random_engine(seed));
    }

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(LIBRARIES_PROCESSING_ORDER.begin(), LIBRARIES_PROCESSING_ORDER.end(),
            default_random_engine(seed));

    return pair<vector<vector<int>>, vector<int>>(BOOKS_TO_PROCESS_IN_EACH_LIBRARY, LIBRARIES_PROCESSING_ORDER);
}

// Return vector<vector<unsigned int>> BOOKS_TO_PROCESS_IN_EACH_LIBRARY, // Output: Libros a procesar por cada libreria
// y orden,
//        list<unsigned int> LIBRARIES_PROCESSING_ORDER // Output: Orden de sign up de cada libreria
pair<vector<vector<int>>, vector<int>> algorithm_random(const vector<int> &BOOKS_SCORES,
                                                        const vector<set<int>> &BOOKS_IN_LIBRARY,
                                                        const vector<int> &LIBRARIES_SIGN_UP_TIME,
                                                        const vector<int> &LIBRARIES_PARALLEL_CAPACITIES,
                                                        const int DAYS_FOR_SCAN
) {
    int best_score = 0;
    pair<vector<vector<int>>, vector<int>> best_result;

    for (int actual_iteration = 0; actual_iteration < 10000; actual_iteration++) {

        pair<vector<vector<int>>, vector<int>> actual_result = algorithm_random_executor(BOOKS_SCORES, BOOKS_IN_LIBRARY,
                                                                                         LIBRARIES_SIGN_UP_TIME,
                                                                                         LIBRARIES_PARALLEL_CAPACITIES);

        int total_score_local = get_score(BOOKS_SCORES,
                                          BOOKS_IN_LIBRARY,
                                          LIBRARIES_SIGN_UP_TIME,
                                          LIBRARIES_PARALLEL_CAPACITIES,
                                          DAYS_FOR_SCAN,
                                          actual_result.first,
                                          actual_result.second);

        if (total_score_local > best_score) {
            best_score = total_score_local;
            best_result = actual_result;
        }


    }
    cout << "Score: " << best_score << endl;

    return best_result;
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

    vector<int> BOOKS_SCORES(NUM_BOOKS, 0);
    vector<set<int> > BOOKS_IN_LIBRARY(NUM_LIBRARIES, set<int>());
    vector<int> LIBRARIES_SIGN_UP_TIME(NUM_LIBRARIES, 0);
    vector<int> LIBRARIES_SHIP_TIME(NUM_LIBRARIES, 0);

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

    // Execute algorithm
    clock_t tStart = clock();

    auto algorithm_result = algorithm_random(BOOKS_SCORES, // Input: Puntuación de cada libro
                                             BOOKS_IN_LIBRARY, // Input: Libros en cada librería
                                             LIBRARIES_SIGN_UP_TIME, // Input: Tiempo de signup de cada libreria
                                             LIBRARIES_SHIP_TIME, // Input: Tiempo de ship de cada libreria
                                             DAYS_FOR_SCAN);// Output: Orden de sign up de cada libreria);

    cout << "Time taken: " << clock() - tStart << endl;

    // Save output
    ofstream output_file;
    output_file.open(argv[2]);
    if (output_file.is_open()) {
        output_file << algorithm_result.second.size() << endl;
        for (auto i: algorithm_result.second) {
            output_file << i << " " << algorithm_result.first[i].size() << endl;
            for (auto j: algorithm_result.first[i]) {
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