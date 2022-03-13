// SPDX-License-Identifier: MIT
#pragma once

#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace lca {

    /**
     * Class used to simulate a Linear Cellular Automaton.
     * Default constructed Automaton is of type U with all rules = false.
     *
     * Basic usage:
     * @code
     * // construct and configure
     * Automaton lca_array(Automaton::TYPE::A, 10);
     * lca_array[6] = 1;
     * // move to next generation
     * lca_array.evolve();
     * // output to std::cout
     * std::cout << lca_array;
     * @endcode
     */
    class Automaton {
       public:
        /** Enum describing the three types of Automaton. */
        enum class TYPE {
            A = 0,     /**< Automaton type A */
            B = 1,     /**< Automaton type B */
            U = 2,     /**< Automaton type U */
            COUNT = 3, /**< Number of types (size of default_rules) */
        };

        // Constructor with type and size
        Automaton(TYPE autom_type, size_t n_cells)
            : type(autom_type), array(n_cells, 0) {}
        Automaton() = default;

        /** Number of rules neccesary to describe a Automaton. */
        static constexpr size_t RULES_SIZE{8};

        /** Array containing the rules of the Automaton. */
        std::array<bool, RULES_SIZE> rules{0};

        /**
         * Method to resize the array containing the cells. Also sets all values
         * to zero.
         * @param[in] n_cells   number of cells
         */
        void resize(size_t n_cells);

        /**
         * Change the Automaton type.
         * Rules are initialized based on the default_rules array.
         * @param[in] new_type   New automaton type. ("A", "B", "U")
         */
        // cppcheck-suppress passedByValue
        void setType(const std::string_view new_type);

        /**
         * @brief Fetch type of the Automaton.
         * @return Automaton::TYPE
         */
        [[nodiscard]] TYPE getType() const;

        /**
         * Move the Automaton to the next generation by applying the rules of
         * the Automaton to the array.
         */
        void evolve();

        /** Indexing operator to modify or access the cell array */
        unsigned char &operator[](size_t);

        friend std::ostream &operator<<(std::ostream &out, const Automaton &c);

       private:
        /** Automaton Type. */
        TYPE type{TYPE::U};

        /** Vector containing the cells. */
        std::vector<unsigned char> array{};

        /** Array conaining the default rules for the Automaton types. */
        const std::array<std::array<bool, RULES_SIZE>,
                         static_cast<size_t>(TYPE::COUNT)>
            default_rules = {
                {{0, 1, 0, 1, 1, 1, 1, 0},  // Rules for Automaton type A
                 {0, 1, 1, 0, 1, 0, 1, 0},  // Rules for Automaton type B
                 {0}}};  // Default rules for Automaton type U (user defined)
    };

    /** Friend function to output the Automaton to std::ostream. */
    std::ostream &operator<<(std::ostream &out, const Automaton &c);

}  // namespace lca
