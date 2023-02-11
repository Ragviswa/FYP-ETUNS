#include <string>
#include <iostream>
#include <algorithm>
#include <exception>

enum rounding_mode {
    round_random,
    round_up,
    round_down
};

enum OperationType {
    indeterminant,
    determinant_fixed,
    determinant_free
};

enum Operation {
    add,
    sub,
    mul,
    div
};

enum Interval {
    low,
    high
};

struct UnaryNumber {
    std::string number;
    int num_delay;
};


UnaryNumber toUnary(double n, rounding_mode mode = round_random, int delay = 0, int len = 0) {
    bool done = false;
    double ones = 0;
    std::string number;

    while (!done) {
        double ifone = abs(n - ((ones + 1) / (number.length() + 1)));
        double ifzero = abs(n - ((ones) / (number.length() + 1)));

        if (ifone == 0) {
            number += '1';
            done = true;
        } else if (ifzero == 0) {
            number += '0';
            ones++;
            done = true;
        } else if (ifone < ifzero) {
            number += '1';
            ones++;
        } else if (ifzero < ifone) {
            number += '0';
        } else {
            int digit;
            switch (mode) {
                case round_random:
                    digit = rand() % 2;
                    break;
                case round_up:
                    digit = 1;
                    break;
                case round_down:
                    digit = 0;
                    break;
                default:
                    digit = rand() % 2;
                    break;
            }
            number += std::to_string(digit);
            ones += digit;
        }

        if (number.length() == len) {
            done = true;
        }
    }

    if (delay > 0) {
        int total_len = number.length() + delay;
        int num_delays = delay / (number.length() + 1);
        int extra_delays = delay % (number.length() + 1);
        std::string delay_number = std::string(num_delays, '*');

        int i = 0;
        while (i < total_len) {
            if (extra_delays > 0) {
                number.insert(i, "*");
                i++;
                extra_delays--;
            }
            number.insert(i, delay_number);
            i += num_delays;
            i++;
        }
    }

    UnaryNumber result = {number, delay};
    return result;
}

double toDouble(UnaryNumber n) {
    std::string number = n.number;
    double result;
    double ones = 0;
    for (int i = 0; i < number.length(); i++) {
        if (number[i] == '1' || number[i] == '0') {
            if (number[i] - '0') {
                ones++;
            }
        }
    }
    result = ones / (number.length() - n.num_delay);
    return result;
}

double determinant_interval_calc(double ones, double total, double length, Interval interval) {
    switch (interval) {
        case low:
            return ((ones)/length);
            break;
        case high:
            return ((ones + (length - total))/length);
            break;
        default:
            throw std::exception();
    }
}

double indeterminant_interval_calc(double ones, double total, double length, Interval interval) {
    switch (interval) {
        case low:
            return (((ones)/total) + ((ones - 1)/total)) / 2.0;
            break;
        case high:
            return (((ones)/total) + ((ones + 1)/total)) / 2.0;
            break;
        default:
            throw std::exception();
    }
}

double indeterminant_result_calc(double ones, double total) {
    return (1/(total + 1)) * (ones + 0.5);
}

UnaryNumber operation(UnaryNumber a, UnaryNumber b, Operation x, OperationType mode) {
    std::string result;
    double a_total              = 0;
    double a_ones               = 0;
    double a_low_interval       = 0;
    double a_high_interval      = 1;

    double b_total              = 0;
    double b_ones               = 0;
    double b_low_interval       = 0;
    double b_high_interval      = 1;

    double result_total         = 0;
    double result_ones          = 0;
    double result_mid           = ((result_ones + 1) / (result_total + 2));
    double result_low_interval;
    double result_high_interval;

    int len = std::max(a.number.length(), b.number.length());
    int delay = 0;

    double (*interval_calc)(double, double, double, Interval);
    double (*result_calc)(double, double);
    switch (mode) {
        case indeterminant:
            interval_calc = indeterminant_interval_calc;
            result_calc = indeterminant_result_calc;
            break;
        case determinant_fixed:
            interval_calc = determinant_interval_calc;
            result_calc = indeterminant_result_calc;
            break;
        case determinant_free:
            interval_calc = determinant_interval_calc;
            result_calc = indeterminant_result_calc;
            break;
        default:
            throw std::exception();
            break;
    }

    int i = 0;
    while (i < len || result_mid <= result_low_interval || result_mid >= result_high_interval) {
        if (i < len) {
            if (a.number[i] == '0' || a.number[i] == '1') {
                double change = a.number[i] - '0';
                a_total += 1;
                a_ones += change;
                if (change) {
                    a_low_interval = std::max(a_low_interval, interval_calc(a_ones, a_total, (a.number.length() - a.num_delay), low));
                } else {
                    a_high_interval = std::min(a_high_interval, interval_calc(a_ones, a_total, (a.number.length() - a.num_delay), high));
                }
            }
            if (b.number[i] == '0' || b.number[i] == '1') {
                double change = b.number[i] - '0';
                b_total += 1;
                b_ones += change;
                if (change) {
                    b_low_interval = std::max(b_low_interval, interval_calc(b_ones, b_total, (b.number.length() - b.num_delay), low));
                } else {
                    b_high_interval = std::min(b_high_interval, interval_calc(b_ones, b_total, (b.number.length() - b.num_delay), high));
                }
            }
        }

        switch (x) {
            case add:
                result_low_interval  = (a_low_interval + b_low_interval)/2.0;
                result_high_interval = (a_high_interval + b_high_interval)/2.0;
                break;
            case mul:
                result_low_interval  = a_low_interval * b_low_interval;
                result_high_interval = a_high_interval * b_high_interval;
                break;
            default:
                throw std::exception();
        }

        if ((result_ones/result_total) == result_low_interval && (result_ones/result_total) == result_high_interval) {
            break;
        } else if (result_mid <= result_low_interval) {
            result += '1';
            result_ones += 1;
            result_total += 1;
        } else if (result_mid >= result_high_interval) {
            result += '0';
            result_total += 1;
        } else {
            result += '*';
            delay += 1;
        }

        if (mode == determinant_fixed) {
            if (len == result_total) {
                break;
            }
        }

        result_mid = result_total == 0 ? 0.5 : result_calc(result_ones, result_total);

        // std::cout << "A: " << a.number << std::endl;
        // std::cout << "B: " << b.number << std::endl << std::endl;
        // std::cout << "Cycle: " << i << std::endl;
        // std::cout << "A_Low_Interval: " << a_low_interval << std::endl;
        // std::cout << "A_High_Interval: " << a_high_interval << std::endl;
        // std::cout << "B_Low_Interval: " << b_low_interval << std::endl;
        // std::cout << "B_High_Interval: " << b_high_interval << std::endl;
        // std::cout << "Calc_Result_Low_Interval: " << result_low_interval << std::endl;
        // std::cout << "Calc_Result_High_Interval: " << result_high_interval << std::endl;
        // std::cout << "Current_Result: " << result << std::endl;
        // std::cout << "Midpoint: " << result_mid << std::endl << std::endl;

        i++;
    }

    UnaryNumber output = {result, delay};

    // std::cout << "A: " << a.number << std::endl;
    // std::cout << "B: " << b.number << std::endl;
    // std::cout << "Output: " << output.number << std::endl << std::endl;

    return output;
}

// int main() {
//     srand(0);
//     double a = 0.0/100.0;
//     double b = 0/100.0;
//     double c = 2.0/4.0;

//     double test = 3.0/32.0;

//     // UnaryNumber detab = operation(toUnary(a, round_random, 0, 2), toUnary(b, round_random, 0, 2), add, determinant);
//     UnaryNumber detac = operation(toUnary(a, round_random, 0, 2), toUnary(b, round_random, 0, 2), mul, indeterminant);
//     // UnaryNumber detabc = operation(detab, detac, mul, determinant);

//     // UnaryNumber inab = operation(toUnary(a, round_random), toUnary(b, round_random, 10), add, indeterminant);
//     // UnaryNumber inac = operation(toUnary(a, round_random), toUnary(c, round_random, 10), add, indeterminant);
//     // UnaryNumber inabc = operation(inab, inac, mul, indeterminant);
//     // std::cout << "-----------------Indeterminant Mode-----------------" << std::endl;
//     // std::cout << "Unary: " << inab.number << std::endl;
//     // std::cout << "Float: " << toDouble(inab) << std::endl;
//     // std::cout << "Delay: " << inab.num_delay << std::endl;
//     // std::cout << "Unary: " << inac.number << std::endl;
//     // std::cout << "Float: " << toDouble(inac) << std::endl;
//     // std::cout << "Delay: " << inac.num_delay << std::endl;
//     // std::cout << "Unary: " << inabc.number << std::endl;
//     // std::cout << "Float: " << toDouble(inabc) << std::endl;
//     // std::cout << "Delay: " << inabc.num_delay << std::endl;
//     // std::cout << "----------------------------------------------------" << std::endl << std::endl;
//     // std::cout << "------------------Determinant Mode------------------" << std::endl;
//     // std::cout << "Unary: " << detab.number << std::endl;
//     // std::cout << "Float: " << toDouble(detab) << std::endl;
//     // std::cout << "Delay: " << detab.num_delay << std::endl;
//     // std::cout << "Unary: " << detac.number << std::endl;
//     // std::cout << "Float: " << toDouble(detac) << std::endl;
//     // std::cout << "Delay: " << detac.num_delay << std::endl;
//     // std::cout << "Unary: " << detabc.number << std::endl;
//     // std::cout << "Float: " << toDouble(detabc) << std::endl;
//     // std::cout << "Delay: " << detabc.num_delay << std::endl;
//     // std::cout << "----------------------------------------------------" << std::endl << std::endl;
//     return 0;
// }

int main() {
    srand((unsigned)time(NULL));
    std::cout << "Length,Delay,TotalError,CalcError" << std::endl;
    for (int i = 1; i <= 64; i*=2) {
        for (int j = 0 ; j < 1000; j++) {
            double DoubleA = (double) rand()/RAND_MAX;
            double DoubleB = (double) rand()/RAND_MAX;
            double DoubleC = (double) rand()/RAND_MAX;
            double DoubleD = (double) rand()/RAND_MAX;
            UnaryNumber InputA = toUnary(DoubleA, round_random, 0, i);
            UnaryNumber InputB = toUnary(DoubleB, round_random, 0, i);
            UnaryNumber InputC = toUnary(DoubleC, round_random, 0, i);
            UnaryNumber InputD = toUnary(DoubleD, round_random, 0, i);
            UnaryNumber OutputAB = operation(InputA, InputB, mul, determinant_fixed);
            UnaryNumber OutputCD = operation(InputC, InputD, mul, determinant_fixed);
            UnaryNumber Output  = operation(OutputAB, OutputCD, mul, determinant_fixed);
            double total_error  = abs(toDouble(Output) - ((DoubleA + DoubleB)/2.0))/((DoubleA + DoubleB)/2.0);
            double calc_error   = ((toDouble(InputA) + toDouble(InputB))/2.0) ? abs(toDouble(Output) - ((toDouble(InputA) + toDouble(InputB))/2.0))/((toDouble(InputA) + toDouble(InputB))/2.0) : 0;
            //assert(toDouble(Output) == (((toDouble(InputA) + toDouble(InputB))/2.0 + (toDouble(InputC) + toDouble(InputD))/2.0)/2.0));
            std::cout << i << "," << Output.num_delay << "," << total_error << "," << calc_error << "," << std::endl;
        }
    }
    return 0;
}