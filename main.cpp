#include <iostream>

#include "Simplex.h"
#include "simplex_utils.h"

int main(int argc, char** argv) {
    auto answer = perform_simplex_method();

    if (answer.has_value())
        std::cout << answer.value();

    return 0;
}