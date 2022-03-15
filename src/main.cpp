// SPDX-License-Identifier: MIT
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "io.h"
#include "lca.h"

/**
 * Uses input from stdin to configure the lca::Automaton object and the
 * requested number of generations.
 * Input specification:
 * @code
 * [AB] \d+ \d+ init_start[ \d]{1,} init_end
 * or
 * U \d+ \d+ init_start[ \d]{1,} init_end[ \d]{8}]
 * @endcode
 * @param[in,out] lca_array lca::Automaton object
 * @param[in,out] n_gens    number of generations
 */
void parseInput(lca::Automaton &lca_array, long &n_gens);

int main() {
    lca::Automaton lca_array{};
    long n_gens{0};
    try {
        // Read input from stdin
        parseInput(lca_array, n_gens);
    } catch (const std::exception &ex) {
        std::cerr << "Invalid argument: " << ex.what() << '\n';
        std::exit(1);
    }

    // Testing shows execution is faster without synchronization
    // between cin and cout
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

#ifndef DISABLE_OUTPUT
    // Print generation 0
    std::cout << lca_array;
#endif

    // Loop over number of requested generations
    for (long n{1}; n < n_gens; ++n) {
        // Move array to next generation
        lca_array.evolve();
#ifndef DISABLE_OUTPUT
        std::cout << lca_array;
#endif
    }

    return 0;
}

void parseInput(lca::Automaton &lca_array, long &n_gens) {
    // Fetch automaton type and set cell type
    std::string buffer = io::readString(std::cin);
    if (buffer != "A" && buffer != "B" && buffer != "U") {
        throw std::invalid_argument("Invalid automaton type");
    }
    lca_array.setType(buffer);

    // Fetch number of cells
    // Resize container to specified size
    size_t n_cells = io::readULong(std::cin);
    lca_array.resize(n_cells);

    // Fetch number of generations
    n_gens = io::readInt(std::cin);
    if (n_gens < 1) {
        throw std::invalid_argument("Invalid number of generations");
    }

    // Start cell initialization
    // This starts with init_start
    buffer = io::readString(std::cin);
    if (buffer != "init_start") {
        throw std::invalid_argument("Expected init_start");
    }

    // Read occupied cells until init_end from stdin
    int enabled_cells{0};
    for (;;) {
        buffer = io::readString(std::cin);
        if (buffer == "init_end") {
            break;
        }
        if (!std::all_of(buffer.begin(), buffer.end(), ::isdigit)) {
            throw std::invalid_argument("Invalid cell inside init section");
        }
        size_t idx = std::stoul(buffer);
        // Cell indexing in input starts at 1
        if (idx < 1 || idx > n_cells) {
            // Spec says to ignore value
            continue;
        }
        lca_array[idx - 1] = 1;
        enabled_cells++;
    }

    // Fail if no cells occupied
    if (enabled_cells == 0) {
        throw std::invalid_argument("No cells occupied");
    }

    // If Automaton type U, start reading definition
    // Read 8 bools from stdin into lca_array.rules[]
    if (lca_array.getType() == lca::Automaton::TYPE::U) {
        for (auto &rule : lca_array.rules) {
            buffer = io::readString(std::cin);
            if (buffer != "0" && buffer != "1") {
                throw std::invalid_argument("Invalid automaton definition");
            }
            // Buffer is guaranteed to be a 0 or 1: cast to bool
            rule = static_cast<bool>(std::stoi(buffer));
        }
    }
}
