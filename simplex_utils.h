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

void impossible_case(const std::string& msg) {
    std::cout << "It is impossible to use Simplex method!\n";
    std::cout << msg << std::endl;
}

/** If we have min problem we can easily move to max problem */

void swap_to_max_problem(std::vector<double>& z_row) {
    for (auto& i : z_row)
        i = -1 * i;
}

/** Filling z-row in the table */

void set_z_row(
        Matrix& matrix,
        std::vector<double>&& z_row,
        const int number_of_vars
) {
    std::move(
            z_row.begin(),
            z_row.begin() + number_of_vars,
            matrix.table[matrix.n - 1].begin()
    );
}

/** Filling basic variables in the table */

void set_basic_vars(Matrix& matrix, const int number_of_vars) {
    for (int i = number_of_vars; i < matrix.m - 2; ++i)
        matrix.table[i - number_of_vars][i] = 1;
}

/** Filling extra info for the table */

void set_presentation(Matrix& matrix, const int number_of_vars, const int number_of_equations) {
    matrix.list_of_all_vars.emplace_back("");

    for (int i = 0; i < number_of_vars; ++i)
        matrix.list_of_all_vars.emplace_back("x" + std::to_string(i + 1));

    for (int i = 0; i < number_of_equations; ++i)
        matrix.list_of_all_vars.emplace_back("s" + std::to_string(i + 1));

    matrix.list_of_all_vars.emplace_back("RHS");
    matrix.list_of_all_vars.emplace_back("Ratio");

    for (int i = 0; i < number_of_equations; ++i)
        matrix.list_of_basic_vars.emplace_back("s" + std::to_string(i + 1));

    matrix.list_of_basic_vars.emplace_back("z");
}

/** Input stage and considering impossible cases */

[[nodiscard]] std::optional<std::tuple<Matrix, int, int, bool>> read_SM() {
    std::cout << "How many variables are in the task?\n";
    int number_of_vars;
    std::cin >> number_of_vars;

    std::cout << "How many constraints are in the task (excluding x >= 0)?\n";
    int number_of_equations;
    std::cin >> number_of_equations;

    // n: slack variables and z-row; m: all variables, solution to constraint and ratio
    Matrix matrix(number_of_equations + 1, number_of_vars + number_of_equations + 2);

    std::cout << "Is it maximization (enter 'max') or minimization (enter 'min') problem?\n";
    std::string problem;
    std::cin >> problem;

    if (problem != "max" && problem != "min") {
        std::string msg = "Only 'max' and 'min'!";
        impossible_case(msg);
        return std::nullopt;
    }

    std::cout << "Enter coefficients c(i-th) in z function (0 if absent)\n";
    std::vector<double> z_row;

    z_row.resize(number_of_vars + number_of_equations, 0);

    for (int i = 0; i < number_of_vars; ++i) {
        std::cout << "c" << i + 1 << "=";
        std::cin >> z_row[i];
    }

    const bool is_max_problem = problem == "max";

    if (is_max_problem)
        swap_to_max_problem(z_row);

    set_z_row(matrix, std::move(z_row), number_of_vars);

    std::cout << "Enter coefficients (0 if absent) for all constraints (left hand side)\n";

    for (int i = 0; i < number_of_equations; ++i) {
        std::cout << i + 1 << " constraint\n";

        for (int j = 0; j < number_of_vars; ++j) {
            std::cout << "c" << j + 1 << "=";
            std::cin >> matrix.table[i][j];
        }

        std::cout << "Enter sign of inequality\n";
        std::string sign;
        std::cin >> sign;

        if (sign != "<=") {
            std::string msg = "Sign " + sign + " is not allowed for this method";
            impossible_case(msg);
            return std::nullopt;
        }

        std::cout << "Enter right hand side of inequality\n";

        double RHS;
        std::cin >> RHS;

        if (RHS < 0) {
            std::string msg = std::to_string(RHS) + " is less than zero";
            impossible_case(msg);
            return std::nullopt;
        }

        matrix.table[i][matrix.m - 2] = RHS;
    }

    set_basic_vars(matrix, number_of_vars);
    set_presentation(matrix, number_of_vars, number_of_equations);

    std::cout << std::endl;
    return std::make_optional(std::make_tuple(matrix, number_of_vars, number_of_equations, is_max_problem));
}

/**
 * Function for finding min coefficients in z-row
 * (must be negative) and in 'ratio' column
 */

[[nodiscard]] int find_min_coeff(const Matrix& matrix, const bool in_row) {
    auto min_item = DBL_MAX;
    int index = -1;

    if (in_row) {
        for (int j = 0; j < matrix.m - 2; j++) {
            double c = matrix.table[matrix.n - 1][j];

            if (min_item > c) {
                index = j;
                min_item = c;
            }
        }
    } else {
        for (int j = 0; j < matrix.n - 1; j++) {
            double c = matrix.table[j][matrix.m - 1];

            if (min_item > c && c >= 0) {
                index = j;
                min_item = c;
            }
        }
    }

    return index;
}

/** Function for filling the last column (ratio) */

void calculate_ratio(Matrix& matrix, const int min_var) {
    // 'ratio' is matrix.table[i][matrix.m - 1]
    // 'solution' is matrix.table[i][matrix.m - 2]

    for (int i = 0; i < matrix.n - 1; i++)
        matrix.table[i][matrix.m - 1] = matrix.table[i][matrix.m - 2] / matrix.table[i][min_var];
}

/**
 * Divide current row by matrix.table[row][column]
 * and subtract it from others rows
 */

void make_column_basic(Matrix& matrix, const int row, const int column) {
    double pivot = matrix.table[row][column];

    for (int i = 0; i < matrix.m - 1; i++)
        matrix.table[row][i] /= pivot;

    for (int k = 0; k < matrix.n; k++) {
        double factor = matrix.table[k][column];

        if (k != row)
            for (int t = 0; t < matrix.m - 1; t++)
                matrix.table[k][t] = matrix.table[k][t] - factor * matrix.table[row][t];
    }
}

void substitute_into_answer(Simplex& answer, const Matrix& matrix, const bool is_max_problem) {
    const auto ans = matrix.table[matrix.n - 1][matrix.m - 2];
    answer.z = is_max_problem ? ans : -ans;

    for (int i = 0; i < matrix.list_of_basic_vars.size(); i++) {
        if (matrix.list_of_basic_vars.at(i)[0] != 's' && matrix.list_of_basic_vars.at(i)[0] != 'z') {
            int index = matrix.list_of_basic_vars.at(i)[1] - '0' - 1;
            answer.variables[index] = matrix.table.at(i)[matrix.m - 2];
        }
    }
}

[[nodiscard]] bool condition_for_exit(const Matrix& matrix) {
    for (int i = 0; i < matrix.m - 2; i++)
        if (matrix.table[matrix.n - 1][i] < 0)
            return false;

    return true;
}

/** At the end of each iteration */

void swap_basic_var(Matrix& matrix, const int old_var_pos, const int new_var_pos, const bool verbose) {
    if (verbose) {
        std::cout << matrix.list_of_basic_vars[old_var_pos] << " leaves" << std::endl;
        std::cout << matrix.list_of_all_vars[new_var_pos + 1] << " enters" << std::endl;
    }

    matrix.list_of_basic_vars[old_var_pos] = matrix.list_of_all_vars[new_var_pos + 1];
}

/** Main stage of an algorithm */

[[nodiscard]] std::optional<Simplex> calculate_answer(
        Matrix&& table,
        const int number_of_vars,
        const bool is_max_problem,
        const bool verbose = true
) {
    int iteration = 0;

    if (verbose) {
        std::cout << "Iteration " << iteration++ << std::endl;
        std::cout << table;
    }

    while (true) {
        if (condition_for_exit(table))
            break;

        int min_var1 = find_min_coeff(table, true);
        if (min_var1 < 0) return std::nullopt;
        calculate_ratio(table, min_var1);

        int min_var2 = find_min_coeff(table, false);
        if (min_var2 < 0) return std::nullopt;
        make_column_basic(table, min_var2, min_var1);

        swap_basic_var(table, min_var2, min_var1, verbose);

        if (verbose) {
            std::cout << std::endl;
            std::cout << "Iteration " << iteration++ << std::endl;
            std::cout << table;
        }
    }

    Simplex answer(number_of_vars);
    substitute_into_answer(answer, table, is_max_problem);
    return std::make_optional(answer);
}

[[nodiscard]] std::optional<Simplex> perform_simplex_method() {
    auto matrix_opt = read_SM();

    if (!matrix_opt.has_value())
        return std::nullopt;

    auto [matrix, number_of_vars, number_of_equations, is_max_problem] = matrix_opt.value();
    return calculate_answer(std::move(matrix), number_of_vars, is_max_problem);
}

#endif //OPTIMIZATION_SIMPLEX_UTILS_H
