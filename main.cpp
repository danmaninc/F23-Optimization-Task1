#include <iostream>

#include "Simplex.h"
#include "simplex_utils.h"

int main() {
    Simplex answer = perform_simplex_method();
    std::cout << answer;
    return 0;
}