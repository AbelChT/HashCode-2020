#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <tuple>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

void algorithm_simple(vector<unsigned int> *slices_in_each_type_of_pizza, set<unsigned int> *types_of_pizza_to_order,
                      unsigned int M) {
    unsigned int N = slices_in_each_type_of_pizza->size();
    unsigned int number_of_slices = 0;

    for (unsigned int i = 0; i < N && number_of_slices <= M; i++) {
        number_of_slices += slices_in_each_type_of_pizza->at(i);
        if (number_of_slices <= M) {
            types_of_pizza_to_order->insert(i);
        }
    }
}

// Used in input d and e
void algorithm_random(vector<unsigned int> *slices_in_each_type_of_pizza, set<unsigned int> *types_of_pizza_to_order,
                      unsigned int M) {
    unsigned int N = slices_in_each_type_of_pizza->size();

    unsigned int score_max = 0;
    set<unsigned int> types_of_pizza_to_order_max;
    unsigned max_score_seed = 0;

    const unsigned int number_of_random_iterations = 10000;

    for (int j = 0; j < number_of_random_iterations; ++j) {
        unsigned int number_of_slices = 0;

        vector<unsigned int> types_of_pizza_random(N, 0);
        iota(types_of_pizza_random.begin(), types_of_pizza_random.end(), 0);

        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        shuffle(types_of_pizza_random.begin(), types_of_pizza_random.end(), default_random_engine(seed));

        set<unsigned int> types_of_pizza_to_order_local;

        for (auto i : types_of_pizza_random) {
            if (number_of_slices + (*slices_in_each_type_of_pizza)[i] <= M) {
                number_of_slices += (*slices_in_each_type_of_pizza)[i];
                types_of_pizza_to_order_local.insert(i);
            }
        }

        // cout << "Iteration score: " << number_of_slices << ", with seed: " << seed << endl;

        if (score_max < number_of_slices) {
            score_max = number_of_slices;
            types_of_pizza_to_order_max = types_of_pizza_to_order_local;
            max_score_seed = seed;
        }
        if (j % 100 == 0) {
            cout << "Iteration: " << j << endl;
        }
    }
    (*types_of_pizza_to_order) = types_of_pizza_to_order_max;
    cout << "Max score seed: " << max_score_seed << endl;
}

// Used in input a and b
void algorithm_dynamic_programing(vector<unsigned int> *slices_in_each_type_of_pizza,
                                  set<unsigned int> *types_of_pizza_to_order,
                                  unsigned int M) {
    // First element: is_final, second element: action, third element: score
    vector<tuple<bool, int, int>> values(M + 1, make_tuple(true, -1, 0));

    // Fill value vector
    for (int i = 0; i <= M; i++) {
        if (i % 1000 == 0) {
            cout << "Iteracion: " << i << endl;
        }
        int max_action = -1;
        int max_score = 0;
        bool is_final_state = true;
        for (int j = 0; j < slices_in_each_type_of_pizza->size(); j++) {
            int number_of_slices = slices_in_each_type_of_pizza->at(j);
            if (i - number_of_slices >= 0) {
                // If we don't pass the limit of slices
                // Check if slice have been taken
                bool have_been_found = false;
                int search_index = i - number_of_slices;
                while (search_index >= 0 && !have_been_found) {
                    if (get<1>(values[search_index]) == j) {
                        have_been_found = true;
                    }
                    if (get<0>(values[search_index])) {
                        search_index = -1;
                    } else {
                        search_index -= slices_in_each_type_of_pizza->at(get<1>(values[search_index]));
                    }
                }

                // Check if this is the better score
                if ((!have_been_found && max_score < get<2>(values[i - number_of_slices]) + number_of_slices) ||
                    (have_been_found && max_score < number_of_slices)) {
                    max_action = j;
                    if (have_been_found) {
                        max_score = number_of_slices;
                        is_final_state = true;
                    } else {
                        max_score = get<2>(values[i - number_of_slices]) + number_of_slices;
                        is_final_state = false;
                    }
                }
            }
        }
        values[i] = make_tuple(is_final_state, max_action, max_score);
    }

    // Find better sequence
    for (int k = M; !get<0>(values[k]); k -= slices_in_each_type_of_pizza->at(get<1>(values[k]))) {
        types_of_pizza_to_order->insert(get<1>(values[k]));
    }
}

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        cout << "Bad number of parameters" << endl;
        return -1;
    }

    // Load data
    unsigned int M; // Number of pizza slices to order
    unsigned int N; // the number of diferent types of pizza

    ifstream input_file;

    input_file.open(argv[1]);
    if (!input_file.is_open()) {
        cout << "Can't read the file" << endl;
        return -1;
    }
    input_file >> M >> N;

    vector<unsigned int> slices_in_each_type_of_pizza(N, 0);  // slices_each_type_of_pizza

    for (int i = 0; i < N; i++) {
        input_file >> slices_in_each_type_of_pizza[i];
    }

    input_file.close();

    set<unsigned int> types_of_pizza_to_order;

    // Execute algorithm
    clock_t tStart = clock();
    algorithm_random(&slices_in_each_type_of_pizza, &types_of_pizza_to_order, M);
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
}
