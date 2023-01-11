#include <string>
#include <iostream>
#include <algorithm>
#include <exception>

enum rounding_mode {
    round_random,
    round_up,
    round_down
};

enum Operation {
    add,
    sub,
    mul,
    div
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

UnaryNumber operation(UnaryNumber a, UnaryNumber b, Operation x) {
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

    std::cout << "A: " << a.number << std::endl;
    std::cout << "B: " << b.number << std::endl << std::endl;

    int i = 0;
    while (i < len || result_mid <= result_low_interval || result_mid >= result_high_interval) {
        if (a.number[i] == '0' || a.number[i] == '1') {
            double change = a.number[i] - '0';
            a_total += 1;
            a_ones += change;
            if (change) {
                a_low_interval = std::max(a_low_interval, (((a_ones)/a_total) + ((a_ones - 1)/a_total)) / 2.0);
            } else {
                a_high_interval = std::min(a_high_interval, (((a_ones)/a_total) + ((a_ones + 1)/a_total)) / 2.0);
            }
        }
        if (b.number[i] == '0' || b.number[i] == '1') {
            double change = b.number[i] - '0';
            b_total += 1;
            b_ones += change;
            if (change) {
                b_low_interval = std::max(b_low_interval, (((b_ones)/b_total) + ((b_ones - 1)/b_total)) / 2.0);
            } else {
                b_high_interval = std::min(b_high_interval, (((b_ones)/b_total) + ((b_ones + 1)/b_total)) / 2.0);
            }
        }

        switch (x) {
            case add:
                result_low_interval  = a_low_interval + b_low_interval;
                result_high_interval = a_high_interval + b_high_interval;
                break;
            case mul:
                result_low_interval  = a_low_interval * b_low_interval;
                result_high_interval = a_high_interval * b_high_interval;
                break;
            default:
                throw std::exception();
        }

        if (result_mid <= result_low_interval) {
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

        result_mid = result_total == 0 ? 0.5 : (1/(result_total + 1)) * (((result_ones) / (result_total)) + 0.5);

        std::cout << "Cycle: " << i << std::endl;
        std::cout << "A_Low_Interval: " << a_low_interval << std::endl;
        std::cout << "A_High_Interval: " << a_high_interval << std::endl;
        std::cout << "B_Low_Interval: " << b_low_interval << std::endl;
        std::cout << "B_High_Interval: " << b_high_interval << std::endl;
        std::cout << "Calc_Result_Low_Interval: " << result_low_interval << std::endl;
        std::cout << "Calc_Result_High_Interval: " << result_high_interval << std::endl;
        std::cout << "Current_Result: " << result << std::endl;
        std::cout << "Midpoint: " << result_mid << std::endl << std::endl;

        i++;
    }

    UnaryNumber output = {result, delay};
    std::cout << "Output: " << result << std::endl << std::endl;
    return output;
}

int main() {
    srand(0);
    double a = 1.0/4.0;
    double b = 2.0/4.0;
    double c = 1.0/4.0;

    UnaryNumber ab = operation(toUnary(a), toUnary(b), add);
    UnaryNumber ac = operation(toUnary(a), toUnary(c), add);
    UnaryNumber abc = operation(ab, ac, mul);
    std::cout << "Unary: " << ab.number << std::endl;
    std::cout << "Float: " << toDouble(ab) << std::endl;
    std::cout << "Delay: " << ab.num_delay << std::endl;
    std::cout << "Unary: " << ac.number << std::endl;
    std::cout << "Float: " << toDouble(ac) << std::endl;
    std::cout << "Delay: " << ac.num_delay << std::endl;
    std::cout << "Unary: " << abc.number << std::endl;
    std::cout << "Float: " << toDouble(abc) << std::endl;
    std::cout << "Delay: " << abc.num_delay << std::endl;
    return 0;
}

// Online Arithmetic
// Stochaistic Computing
// Julie's Paper


// Look at a set of experimental results that would be meaningful to look at to investigate delays
// Looking at the delay distribution for different combinations of input values
// Looking at how the stacking of inputs can be interesting to look at
// Best representation in hardware
// Relax the strictness of the number system or the information we are getting from the inputs.
// Uniformally distributed through 0 - 1 floating points that are converted. Three converters:
// one that converts always by rounding up, rounding down and one that breaks the tie randomly
// Plotting the distribution of the delays
// Think about the hardware side of things.

// Work on trying to get new algorith to work with midpoints