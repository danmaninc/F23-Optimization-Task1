#ifndef OPTIMIZATION_SIMPLEX_H
#define OPTIMIZATION_SIMPLEX_H

#include <vector>
#include <ostream>

/** Class where we can store an answer for a task */

struct Simplex {
    double z;
    std::vector<double> variables;

    explicit Simplex(const std::size_t size) {
        z = 0;
        variables.resize(size, 0);
    }

    friend std::ostream& operator<<(std::ostream& out, Simplex& answer) {
        out << "Value of z: " << answer.z << std::endl;

        for (int i = 0; i < answer.variables.size(); ++i)
            out << "x" << i + 1 << " = " << answer.variables[i] << std::endl;

        out << std::endl;
        return out;
    }
};


#endif //OPTIMIZATION_SIMPLEX_H
