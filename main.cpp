#include <iostream>

#include "Simplex.h"
#include "simplex_utils.h"
//#include "tests.h"

int main(int argc, char** argv) {
    Simplex answer = perform_simplex_method();
    std::cout << answer;

    return 0;
    //return run_tests(argc, argv);
}