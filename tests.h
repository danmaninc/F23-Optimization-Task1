#include <fstream>
#include <string>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "json.hpp"
#include "simplex_utils.h"

using json = nlohmann::json;

const double CMP_PRECISION = 0.5;

void set_matrix_table(
        Matrix& matrix,
        std::vector<std::vector<double>>&& table,
        const int number_of_equations,
        const int number_of_vars
) {
    for (int i = 0; i < number_of_equations; ++i) {
        for (int j = 0; j < number_of_vars; ++j)
            matrix.table[i][j] = table[i][j];
        matrix.table[i][matrix.m - 2] = table[i].back();
    }
}

bool check_double_vector_equality(
        const std::vector<double>& first,
        const std::vector<double>& second
) {
    for (std::size_t i = 0; i < first.size(); ++i)
        if (std::abs(first[i] - second[i]) > CMP_PRECISION)
            return false;
    return true;
}

void print_vec(const std::vector<double>& first) {
    for (auto& e : first)
        std::cout << e << " ";
    std::cout << std::endl;
}

inline void perform_test(
        const char* const test_sample_path,
        const double expected_z,
        std::vector<double>&& expected_vars
) {
    std::ifstream in(test_sample_path);
    const auto data = json::parse(in);

    const int number_of_vars = data["number_of_vars"].get<int>();
    const int number_of_equations = data["number_of_equations"].get<int>();
    const auto problem = data["problem"].get<std::string>();

    auto z_row = data["z_row"].get<std::vector<double>>();
    auto table = data["matrix"].get<std::vector<std::vector<double>>>();

    Matrix mtx(number_of_equations + 1, number_of_vars + number_of_equations + 2);

    const bool is_max_problem = problem == "max";

    if (is_max_problem)
        swap_to_max_problem(z_row);

    set_z_row(mtx, std::move(z_row), number_of_vars);
    set_matrix_table(mtx, std::move(table), number_of_equations, number_of_vars);
    set_basic_vars(mtx, number_of_vars);
    set_presentation(mtx, number_of_vars, number_of_equations);

    auto ans = calculate_answer(std::move(mtx), number_of_vars, is_max_problem, false).value();

    std::cout << ans.z << std::endl;
    ASSERT_TRUE(std::abs(ans.z - expected_z) < CMP_PRECISION);

    print_vec(ans.variables);
    ASSERT_TRUE(check_double_vector_equality(ans.variables, expected_vars));
}

TEST(SimplexTests, Test_1) {
    perform_test(
            "../test_samples/1.json",
            16.94,
            std::vector<double> { 0, 6.11765, 1.41176 }
    );
}

TEST(SimplexTests, Test_2) {
    perform_test(
            "../test_samples/2.json",
            -6,
            std::vector<double> { 0, 0, 6 }
    );
}

TEST(SimplexTests, Test_3) {
    perform_test(
            "../test_samples/3.json",
            19.615,
            std::vector<double> { 1.154, 7.308 }
    );
}

TEST(SimplexTests, Test_4) {
    perform_test(
            "../test_samples/4.json",
            -20.9,
            std::vector<double> { 11.82, 0.9 }
    );
}

TEST(SimplexTests, Test_5) {
    perform_test(
            "../test_samples/5.json",
            34.96,
            std::vector<double> { 0.38, 0, 0, 1.63, 2.33 }
    );
}

TEST(SimplexTests, Test_6) {
    perform_test(
            "../test_samples/6.json",
            -6.19,
            std::vector<double> { 0, 0, 1.24, 0, 0 }
    );
}

int run_tests(int argc, char** const argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}