#include <iostream>

#include "Simplex.h"
#include "simplex_utils.h"
<<<<<<< HEAD
//#include "tests.h"
=======
>>>>>>> de4461640900ba756d47739ea0cc98e4b251fece

int main(int argc, char** argv) {
    auto answer = perform_simplex_method();

<<<<<<< HEAD
    return 0;
    //return run_tests(argc, argv);
=======
    if (answer.has_value())
        std::cout << answer.value();

    return 0;
>>>>>>> de4461640900ba756d47739ea0cc98e4b251fece
}