#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
//#include <string>

using namespace std;

const double MIN_COUT = 0.05;

const double ZERO = 0;

const int NUMBERS_AFTER_DOT = 1;


//Class for matrix from AGLA (you can delete functions that we don't need to use)
class Matrix {
public:
    int n;
    int m;
    vector<vector<double> > table;

    //Only for Simplex method
    /////////////////////////
    vector<string> list_of_all_vars;
    vector<string> list_of_basic_vars;
    ///////////////////


    Matrix(int first, int second) {
        n = first;
        m = second;

        table.resize(n);
        for (int i = 0; i < n; ++i) {
            table[i].resize(m, 0);
        }
    }
    friend istream& operator>>(istream& in, Matrix& matrix) {
        for (int i = 0; i < matrix.n; ++i) {
            for (int j = 0; j < matrix.m; ++j) {
                in >> matrix.table[i][j];
            }
        }
        return in;
    }

    friend ostream& operator<<(ostream& out, Matrix& matrix) {
        for (auto& i : matrix.list_of_all_vars) {
            out << i << " " << "\t";
        }
        out << endl;
        int counter = 0;
        for (auto& i : matrix.table) {
            out << matrix.list_of_basic_vars[counter++] << "\t";
            for (int j = 0; j < i.size(); ++j) {
                if (abs(i[j]) < MIN_COUT) {
                    out << setprecision(NUMBERS_AFTER_DOT) << fixed << ZERO << "\t";
                }
                else {
                    out << setprecision(NUMBERS_AFTER_DOT) << fixed << i[j] << "\t";
                }
                //if (j != i.size() - 1) {
                //    out << " ";
               // }
            }
            out << endl << endl;
        }
        return out;
    }

    bool checkSumSub(Matrix& A, Matrix& B) {
        return A.n == B.n && A.m == B.m;
    }

    bool checkMul(Matrix& A, Matrix& B) {
        return A.m == B.n;
    }

    bool checkForDeterminant(Matrix& A) {
        return A.n == A.m;
    }
    double findDeterminant(Matrix& A) {
        if (A.n == 1) {
            return A.table[0][0];
        }
        if (A.n == 2) {
            return A.table[0][0] * A.table[1][1] - A.table[0][1] * A.table[1][0];
        }
        bool sign = true;
        double result = 0;

        for (int column = 0; column < A.n; ++column) {
            Matrix matrix(A.n - 1, A.n - 1);
            for (int i = 1; i < A.n; ++i) {
                int secondPosition = 0;
                for (int j = 0; j < A.n; ++j) {
                    if (j != column) {
                        matrix.table[i - 1][secondPosition] = A.table[i][j];
                        secondPosition++;
                    }
                }
            }
            if (sign) {
                result += A.table[0][column] * findDeterminant(matrix);
                sign = false;
            }
            else {
                result -= A.table[0][column] * findDeterminant(matrix);
                sign = true;
            }

        }
        return result;
    }

    Matrix& operator= (Matrix const& matrix) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                this->table[i][j] = matrix.table[i][j];
            }
        }
        return *this;
    }
    friend Matrix operator+ (Matrix& first, Matrix& second) {
        Matrix res(first.n, first.m);
        for (int i = 0; i < first.n; ++i) {
            for (int j = 0; j < first.m; ++j) {
                res.table[i][j] = first.table[i][j] + second.table[i][j];
            }
        }
        return res;
    }
    friend Matrix operator- (Matrix& first, Matrix& second) {
        Matrix res(first.n, first.m);
        for (int i = 0; i < first.n; ++i) {
            for (int j = 0; j < first.m; ++j) {
                res.table[i][j] = first.table[i][j] - second.table[i][j];
            }
        }
        return res;
    }

    friend Matrix operator* (Matrix& first, Matrix& second) {
        Matrix res(first.n, second.m);

        for (int i = 0; i < res.n; ++i) {
            for (int j = 0; j < res.m; ++j) {
                res.table[i][j] = 0;
                for (int k = 0; k < first.m; ++k) {
                    res.table[i][j] += first.table[i][k] * second.table[k][j];
                }
            }
        }
        return res;
    }

    Matrix findTransposeMatrix() {
        Matrix transposed(m, n);

        for (int i = 0; i < transposed.n; ++i) {
            for (int j = 0; j < transposed.m; ++j) {
                transposed.table[i][j] = this->table[j][i];
            }
        }
        return transposed;
    }
};


// Class where we can store an answer for a task
class Answer {
public:
    double z;
    vector<double> variables;

    Answer(int size) {
        z = 0;
        variables.resize(size, 0);
    }

    friend ostream& operator<<(ostream& out, Answer& answer) {
        out << "Value of z: " << answer.z << endl;

        for (int i = 0; i < answer.variables.size(); ++i) {
            out << "x" << i << " = " << answer.variables[i] << endl;
        }
        out << endl;
        return out;
    }
};


// Function for printing errors
void impossible_case(string& msg) {
    cout << "It is impossible to use Simlex method!\n";
    cout << msg << endl;
    exit(0);
}

// If we have min problem we can easily move to max problem
void swap_to_max_problem(vector<double>& z_row) {
    for (auto& i : z_row) {
        i = -1 * i;
    }
}

void input_for_SM(Matrix& matrix, int number_of_vars, int number_of_equations) {
    cout << "Is it maximization (enter 'max') or minimization (enter 'min') problem?\n";
    string problem;
    cin >> problem;

    if (problem != "max" && problem != "min") {
        string msg = "Only 'max' and 'min'!";
        impossible_case(msg);
    }

    cout << "Enter coefficients c(i-th) in z function (0 if absent)\n";

    vector<double> z_row;
    z_row.resize(number_of_vars + number_of_equations, 0);
    for (int i = 0; i < number_of_vars; ++i) {
        cout << "c" << i + 1 << "=";
        cin >> z_row[i];
    }

    //!!! Please, check the condition for this one more time
    if (problem == "max") {
        swap_to_max_problem(z_row);
    }

    for (int i = 0; i < number_of_vars; ++i) {
        matrix.table[matrix.n - 1][i] = z_row[i];
    }

    cout << "Enter coefficients (0 if absent) for all constraints (left hand side)\n";

    for (int i = 0; i < number_of_equations; ++i) {
        cout << i + 1 << " constraint\n";
        for (int j = 0; j < number_of_vars; ++j) {
            cout << "c" << j + 1 << "=";
            cin >> matrix.table[i][j];
        }

        cout << "Enter sign of inequality\n";
        string sign;
        cin >> sign;
        if (sign != "<=") {
            string msg = "Sign " + sign + " is not allowed for this method";
            impossible_case(msg);
        }

        cout << "Enter right hand side of inequality\n";
        double RHS;
        cin >> RHS;
        if (RHS < 0) {
            string msg = to_string(RHS) + " is less than zero";
            impossible_case(msg);
        }
        matrix.table[i][matrix.m - 2] = RHS;
    }

    //Filling basic variables in the table
    for (int i = number_of_vars; i < matrix.m - 2; ++i) {
        matrix.table[i - number_of_vars][i] = 1;
    }


    //Extra info for table
    matrix.list_of_all_vars.push_back("");
    for (int i = 0; i < number_of_vars; ++i) {
        matrix.list_of_all_vars.push_back("x" + to_string(i));
    }
    for (int i = 0; i < number_of_equations; ++i) {
        matrix.list_of_all_vars.push_back("s" + to_string(i));
    }
    matrix.list_of_all_vars.push_back("RHS");
    matrix.list_of_all_vars.push_back("Ratio");

    for (int i = 0; i < number_of_equations; ++i) {
        matrix.list_of_basic_vars.push_back("s" + to_string(i));
    }
    matrix.list_of_basic_vars.push_back("z");

    cout << endl;
}

//Here are the functions for the main stage you need to implement
////////////////////////////

// Function for finding min coefficients in z-row (must be negative) and in 'ratio' column
int find_min_coeff(Matrix& matrix, bool in_row) {
    // int min_item = ...
    if (in_row) {
        // find min coefficient in z row ( matrix.table[matrix.n - 1][i] )
    }
    else {
        // find min in 'ratio' column (matrix.table[i][matrix.m - 1])
    }

    return 0;
}

// Function for filling the last column
void calculate_ratio(Matrix& matrix, int min_var) {
    // 'ratio' is matrix.table[i][matrix.m - 1]
    //////////
}

// Divide current row by matrix.table[row][column] and subtract it from others rows
void make_column_basic(Matrix& matrix, int row, int column) {
    ///////////
}

// Writing the answer
void substitute_into_answer(Answer& answer, Matrix& table) {
    answer.z = table.table[table.n - 1][table.m - 2];

    for (int i = 0; i < answer.variables.size(); i++) {
        answer.variables[i] = table.table[table.n - answer.variables.size()][table.m - 2];
    }
    // answer.variables = ...
    /////////////
}

bool condition_for_exit(Matrix& matrix) {
    bool exists = true;
    for (int i = 0; i < matrix.m - 2; i++) {
        if (matrix.table[matrix.n - 1][i] < 0) {
            exists = false;
        }
    }
    // z-row is matrix.table[matrix.n - 1][i]
    ////////
    return exists;
}

// At the end of each iteration
void swap_basic_var(Matrix& matrix, int old_var_pos, int new_var_pos) {
    cout << matrix.list_of_all_vars[old_var_pos] << " leaves" << endl;
    cout << matrix.list_of_all_vars[new_var_pos] << " enters" << endl;

    matrix.list_of_basic_vars[old_var_pos] = matrix.list_of_all_vars[new_var_pos];
}


//////////////////////////

Answer simlexMethod() {
    cout << "How many variables are in the task?\n";
    int number_of_vars;
    cin >> number_of_vars;


    cout << "How many constraints are in the task (excluding x >= 0)?\n";
    int number_of_equations;
    cin >> number_of_equations;

    //n: slack variables and z-row; m: all variables, solution to constraint and ratio
    Matrix table(number_of_equations + 1, number_of_vars + number_of_equations + 2);

    //Input stage and considering impossible cases
    input_for_SM(table, number_of_vars, number_of_equations);


    //Main stage of a function
    int iteration = 0;
    while (true) {
        cout << "Iteration " << iteration << endl;
        iteration++;
        cout << table;

        // remove it!
        exit(0);
        ////////

        int min_var1 = find_min_coeff(table, false);
        calculate_ratio(table, min_var1);

        int min_var2 = find_min_coeff(table, true);
        make_column_basic(table, min_var2, min_var1);

        swap_basic_var(table, min_var2, min_var1);

        bool condition = condition_for_exit(table);
        if (condition) {
            break;
        }
    }

    Answer answer(number_of_vars);
    substitute_into_answer(answer, table);

    return answer;
}

int main() {
    Answer answer = simlexMethod();
    cout << answer;
    return 0;
}
