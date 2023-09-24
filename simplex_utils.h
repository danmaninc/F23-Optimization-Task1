#ifndef OPTIMIZATION_SIMPLEX_UTILS_H
#define OPTIMIZATION_SIMPLEX_UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <cfloat>

#include <optional>

#include "Matrix.h"
#include "Simplex.h"

/** Function for printing errors */

void impossible_case(std::string& msg) {
    std::cout << "It is impossible to use Simplex method!\n";
    std::cout << msg << std::endl;
}

/** If we have min problem we can easily move to max problem */

void swap_to_max_problem(std::vector<double>& z_row) {
    for (auto& i : z_row)
        i = -1 * i;
}

bool input_for_SM(
        Matrix& matrix,
        const std::size_t number_of_vars,
        const std::size_t number_of_equations
) {
    std::cout << "Is it maximization (enter 'max') or minimization (enter 'min') problem?\n";
    std::string problem;
    std::cin >> problem;

    if (problem != "max" && problem != "min") {
        std::string msg = "Only 'max' and 'min'!";
        impossible_case(msg);
        return false;
    }

    std::cout << "Enter coefficients c(i-th) in z function (0 if absent)\n";
    std::vector<double> z_row;

    z_row.resize(number_of_vars + number_of_equations, 0);

    for (std::size_t i = 0; i < number_of_vars; ++i) {
        std::cout << "c" << i + 1 << "=";
        std::cin >> z_row[i];
    }

    // !!! Please, check the condition for this one more time

    if (problem == "max")
        swap_to_max_problem(z_row);

    for (std::size_t i = 0; i < number_of_vars; ++i)
        matrix.table[matrix.n - 1][i] = z_row[i];

    std::cout << "Enter coefficients (0 if absent) for all constraints (left hand side)\n";

    for (std::size_t i = 0; i < number_of_equations; ++i) {
        std::cout << i + 1 << " constraint\n";

        for (std::size_t j = 0; j < number_of_vars; ++j) {
            std::cout << "c" << j + 1 << "=";
            std::cin >> matrix.table[i][j];
        }

        std::cout << "Enter sign of inequality\n";
        std::string sign;
        std::cin >> sign;

        if (sign != "<=") {
            std::string msg = "Sign " + sign + " is not allowed for this method";
            impossible_case(msg);
            return false;
        }

        std::cout << "Enter right hand side of inequality\n";

        double RHS;
        std::cin >> RHS;

        if (RHS < 0) {
            std::string msg = std::to_string(RHS) + " is less than zero";
            impossible_case(msg);
            return false;
        }

        matrix.table[i][matrix.m - 2] = RHS;
    }

    // Filling basic variables in the table

    for (std::size_t i = number_of_vars; i < matrix.m - 2; ++i)
        matrix.table[i - number_of_vars][i] = 1;

    // Extra info for table
    matrix.list_of_all_vars.push_back("");

    for (std::size_t i = 0; i < number_of_vars; ++i)
        matrix.list_of_all_vars.push_back("x" + std::to_string(i));

    for (std::size_t i = 0; i < number_of_equations; ++i)
        matrix.list_of_all_vars.push_back("s" + std::to_string(i));

    matrix.list_of_all_vars.push_back("RHS");
    matrix.list_of_all_vars.push_back("Ratio");

    for (std::size_t i = 0; i < number_of_equations; ++i)
        matrix.list_of_basic_vars.push_back("s" + std::to_string(i));

    matrix.list_of_basic_vars.push_back("z");

    std::cout << std::endl;
    return true;
}

/**
 * Function for finding min coefficients in z-row
 * (must be negative) and in 'ratio' column
 */

int find_min_coeff(Matrix& matrix, bool in_row) {
    auto min_item = DBL_MAX;
    int index = -1;

    if (in_row) {
        for (std::size_t j = 0; j < matrix.m - 2; j++) {
            double c = matrix.table[matrix.n - 1][j];

            if (min_item > c) {
                index = j;
                min_item = c;
            }
        }
        // find min coefficient in z row ( matrix.table[matrix.n - 1][i] )
    } else {
        for (std::size_t j = 0; j < matrix.n; j++) {
            double c = matrix.table[j][matrix.m - 1];

            if (min_item > c) {
                index = j;
                min_item = c;
            }
        }
    }

    return index;
}

/** Function for filling the last column (ratio) */

void calculate_ratio(Matrix& matrix, int min_var) {
    // 'ratio' is matrix.table[i][matrix.m - 1]
    // 'solution' is matrix.table[i][matrix.m - 2]

    for (std::size_t i = 0; i < matrix.n; i++)
        matrix.table[i][matrix.m - 1] = matrix.table[i][matrix.m - 2] / matrix.table[i][min_var];
}

/**
 * Divide current row by matrix.table[row][column]
 * and subtract it from others rows
 */

void make_column_basic(Matrix& matrix, int row, int column) {
    double pivot = matrix.table[row][column];

    for (std::size_t i = 0; i < matrix.m - 1; i++)
        matrix.table[row][i] /= pivot;

    for (std::size_t k = 0; k < matrix.n; k++) {
        int factor = matrix.table[k][column];

        if (k != row)
            for (int t = 0; t < matrix.m - 1; t++)
                matrix.table[k][t] = matrix.table[k][t] - factor * matrix.table[row][t];
    }
}

void substitute_into_answer(Simplex& answer, Matrix& table) {
    answer.z = table.table[table.n - 1][table.m - 2];

    for (std::size_t i = 0; i < answer.variables.size(); i++)
        answer.variables[i] = table.table[table.n - answer.variables.size()][table.m - 2];..
}

bool condition_for_exit(Matrix& matrix) {
    for (std::size_t i = 0; i < matrix.m - 2; i++)
        if (matrix.table[matrix.n - 1][i] < 0)
            return false;

    return true;
}


// At the end of each iteration
void swap_basic_var(Matrix& matrix, int old_var_pos, int new_var_pos) {
    std::cout << matrix.list_of_all_vars[old_var_pos] << " leaves" << std::endl;
    std::cout << matrix.list_of_all_vars[new_var_pos] << " enters" << std::endl;
    matrix.list_of_basic_vars[old_var_pos] = matrix.list_of_all_vars[new_var_pos];
}

std::optional<Simplex> perform_simplex_method() {
    std::cout << "How many variables are in the task?\n";
    std::size_t number_of_vars;
    std::cin >> number_of_vars;

    std::cout << "How many constraints are in the task (excluding x >= 0)?\n";
    std::size_t number_of_equations;
    std::cin >> number_of_equations;

    // n: slack variables and z-row; m: all variables, solution to constraint and ratio
    Matrix table(number_of_equations + 1, number_of_vars + number_of_equations + 2);

    // Input stage and considering impossible cases
    if (!input_for_SM(table, number_of_vars, number_of_equations))
        return std::nullopt;

    // Main stage of a function

    std::size_t iteration = 0;

    while (true) {
        std::cout << "Iteration " << iteration << std::endl;
        iteration++;
        std::cout << table;

        int min_var1 = find_min_coeff(table, false);
        calculate_ratio(table, min_var1);

        int min_var2 = find_min_coeff(table, true);
        make_column_basic(table, min_var2, min_var1);

        swap_basic_var(table, min_var2, min_var1);

        if (condition_for_exit(table))
            break;
    }

    Simplex answer(number_of_vars);
    substitute_into_answer(answer, table);

    return std::make_optional(answer);
}

#endif //OPTIMIZATION_SIMPLEX_UTILS_H
