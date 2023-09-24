#ifndef OPTIMIZATION_MATRIX_H
#define OPTIMIZATION_MATRIX_H

#include <vector>
#include <string>
#include <istream>
#include <iomanip>

const double MIN_COUT = 0.05;

const double ZERO = 0;

const int NUMBERS_AFTER_DOT = 1;

struct Matrix {
    std::size_t n;
    std::size_t m;
    std::vector<std::vector<double>> table;

    std::vector<std::string> list_of_all_vars;
    std::vector<std::string> list_of_basic_vars;

    Matrix(const std::size_t n, const std::size_t m) {
        this->n = n;
        this->m = m;

        table.resize(n);

        for (auto& r : table)
            r.resize(m, 0);
    }

    friend std::istream& operator>>(std::istream& in, Matrix& matrix) {
        for (auto& r : matrix.table)
            for (auto& e : r)
                in >> e;

        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
        for (auto& i : matrix.list_of_all_vars)
            out << i << " " << "\t";

        out << std::endl;
        int counter = 0;

        for (auto& i : matrix.table) {
            out << matrix.list_of_basic_vars[counter++] << "\t";

            for (double j : i)
                out
                << std::setprecision(NUMBERS_AFTER_DOT)
                << std::fixed
                << (std::abs(j) < MIN_COUT ? ZERO : j)
                << "\t";

            out << std::endl << std::endl;
        }

        return out;
    }

    Matrix& operator= (const Matrix& matrix) {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                table[i][j] = matrix.table[i][j];

        return *this;
    }
};


#endif //OPTIMIZATION_MATRIX_H
