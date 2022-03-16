// SPDX-License-Identifier: MIT
#include "lca.h"

#include <algorithm>
#include <array>
#include <stdexcept>

using namespace lca;

void Automaton::evolve() {
    if (this->array.empty()) {
        return;
    }
    // 3 LSB of sliding_window are the index for rule lookup inside rules[]
    // Every iteration we add the next element to the sliding window
    // To accomplish this, we use a left shift 1 and do an OR operation with the
    // next element
    const unsigned char BITMASK = 0b111;
    unsigned char sliding_window{this->array.front()};

    for (auto curr = begin(this->array); curr != end(this->array); ++curr) {
        sliding_window = static_cast<unsigned char>(sliding_window << 1U);
        // End boundary condition: array[array.size()] == 0
        if (next(curr) != end(this->array)) {
            [[likely]] sliding_window |= *(next(curr));
        }

        // Update current cell to new value using the three least significant
        // bits of the sliding window
        *curr = this->rules[static_cast<size_t>(sliding_window & BITMASK)];
    }
}

std::ostream &lca::operator<<(std::ostream &out, const Automaton &c) {
    // Lookup instead of expensive branch. See README.md
    const std::array<char, 2> char_map = {' ', '*'};
    for (auto const &i : c.array) {
        std::cout.put(char_map[i]);
    }

    // Don't use endl, we don't need to flush every line
    std::cout.put('\n');
    return out;
}

// cppcheck-suppress passedByValue
void Automaton::setType(const std::string_view new_type) {
    if (new_type == "A") {
        this->type = TYPE::A;
    } else if (new_type == "B") {
        this->type = TYPE::B;
    } else if (new_type == "U") {
        this->type = TYPE::U;
    } else {
        throw std::invalid_argument("Invalid automaton type");
    }

    initRules();
}

void Automaton::initRules() {
    // Copy rules for type from default_rules to this->rules
    std::copy(cbegin(this->default_rules[static_cast<size_t>(this->type)]),
              cend(this->default_rules[static_cast<size_t>(this->type)]),
              begin(this->rules));
}

Automaton::TYPE Automaton::getType() const { return this->type; }

void Automaton::resize(size_t n_cells) {
    // Resize is faster when resizing an empty vector
    if (this->array.empty()) {
        this->array.resize(n_cells);
    } else {
        this->array.assign(n_cells, 0U);
    }
}

unsigned char &Automaton::operator[](size_t idx) { return this->array.at(idx); }
