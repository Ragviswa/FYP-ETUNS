#include "Unary.h"
#include <iostream>
#include <algorithm>
#include <cmath>

UnaryNumber toUnary(double n, rounding_mode mode, int delay, int len) {
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
            return ceil(length*((ones)/length))/length;
            break;
        case high:
            return floor(length*((ones + (length - total))/length))/length;
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

double indeterminant_result_calc(double ones, double total, double length, double epsilon) {
    return (1/(total + 1)) * (ones + 0.5 + epsilon/2.0);
}
double determinant_result_calc(double ones, double total, double length, double epsilon) {
    return (floor(length*((ones + (length - total))/length)) + ceil(length*((ones)/length)) + epsilon)/(length * 2.0);
}

UnaryNumber operation1(UnaryNumber a, Operation x, OperationType mode, double epsilon) {
std::string result;
    double a_total              = 0;
    double a_ones               = 0;
    double a_low_interval       = 0;
    double a_high_interval      = 1;

    double result_total         = 0;
    double result_ones          = 0;
    double result_mid           = ((result_ones + 1) / (result_total + 2));
    double result_mid_lo        = ((result_ones + 1) / (result_total + 2));
    double result_mid_hi        = ((result_ones + 1) / (result_total + 2));
    double result_low_interval;
    double result_high_interval;

    int len = a.number.length();
    int len_stream = a.number.length() - a.num_delay;
    int delay = 0;

    double (*interval_calc)(double, double, double, Interval);
    double (*result_calc)(double, double, double, double);
    switch (mode) {
        case indeterminant:
            interval_calc = indeterminant_interval_calc;
            result_calc = indeterminant_result_calc;
            break;
        case determinant_2_fixed:
            interval_calc = determinant_interval_calc;
            result_calc = determinant_result_calc;
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

    int i = len - 1;
    while (i >= 0 || result_mid <= result_low_interval || result_mid >= result_high_interval) {
        if (i >= 0) {
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
        }

        switch (x) {
            case root:
                result_low_interval  = std::sqrt(a_low_interval);
                result_high_interval = std::sqrt(a_high_interval);
                break;
            case mul2:
                result_low_interval  = a_low_interval * 2.0;
                result_high_interval = a_high_interval * 2.0;
                break;
            case div2:
                result_low_interval  = a_low_interval / 2.0;
                result_high_interval = a_high_interval / 2.0;
                break;
            default:
                throw std::exception();
        }
        
        result_mid_lo = result_calc(result_ones, result_total, len, -epsilon);
        result_mid    = result_calc(result_ones, result_total, len, 0.0);
        result_mid_hi = result_calc(result_ones, result_total, len, +epsilon);

        if (result_mid <= result_low_interval) {
            result += '1';
            result_ones += 1;
            result_total += 1;
        } else if (result_mid >= result_high_interval) {
            result += '0';
            result_total += 1;
        } else if (result_mid_lo <= result_low_interval && result_mid_hi < result_high_interval) {
            result += '1';
            result_ones += 1;
            result_total += 1;
        } else if (result_mid_hi >= result_high_interval && result_mid_lo > result_low_interval) {
            result += '0';
            result_total += 1;
        } else if (result_mid_hi >= result_high_interval && result_mid_lo <= result_low_interval && abs(result_mid - result_low_interval) <= abs(result_mid - result_high_interval)) {
            result += '1';
            result_ones += 1;
            result_total += 1;
        } else if (result_mid_hi >= result_high_interval && result_mid_lo <= result_low_interval && abs(result_mid - result_low_interval) >= abs(result_mid - result_high_interval)) {
            result += '0';
            result_total += 1;
        } else {
            result += '*';
            delay += 1;
        }

        std::cout << "A: " << a.number << std::endl;
        std::cout << "Cycle: " << i << std::endl;
        std::cout << "A_Low_Interval: " << a_low_interval << std::endl;
        std::cout << "A_High_Interval: " << a_high_interval << std::endl;
        std::cout << "Calc_Result_Low_Interval: " << result_low_interval << std::endl;
        std::cout << "Calc_Result_High_Interval: " << result_high_interval << std::endl;
        std::cout << "Current_Result: " << result << std::endl;
        std::cout << "Midpoint: " << result_mid << std::endl;
        std::cout << "Midpoint+Eps: " << result_mid_hi << std::endl;
        std::cout << "Midpoint-Eps: " << result_mid_lo << std::endl << std::endl;

        if (mode == determinant_fixed || mode == determinant_2_fixed) {
            if (len_stream == result_total) {
                break;
            }
        }

        i--;
    }

    UnaryNumber output = {result, delay};

    std::cout << "A: " << a.number << std::endl;
    std::cout << "Output: " << output.number << std::endl << std::endl;

    return output;
}

UnaryNumber operation2(UnaryNumber a, UnaryNumber b, Operation x, OperationType mode, double epsilon) {
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
    double result_mid_lo        = ((result_ones + 1) / (result_total + 2));
    double result_mid_hi        = ((result_ones + 1) / (result_total + 2));
    double result_low_interval;
    double result_high_interval;

    int len = std::max(a.number.length(), b.number.length());
    int len_stream = std::max(a.number.length() - a.num_delay, b.number.length() - b.num_delay);
    int delay = 0;

    double (*interval_calc)(double, double, double, Interval);
    double (*result_calc)(double, double, double, double);
    switch (mode) {
        case indeterminant:
            interval_calc = indeterminant_interval_calc;
            result_calc = indeterminant_result_calc;
            break;
        case determinant_2_fixed:
            interval_calc = determinant_interval_calc;
            result_calc = determinant_result_calc;
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

    int i = len - 1;
    while (i >= 0 || result_mid <= result_low_interval || result_mid >= result_high_interval) {
        if (i >= 0) {
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

        result_mid_lo = result_calc(result_ones, result_total, len, -epsilon);
        result_mid    = result_calc(result_ones, result_total, len, 0.0);
        result_mid_hi = result_calc(result_ones, result_total, len, +epsilon);

        if (result_mid <= result_low_interval) {
            result += '1';
            result_ones += 1;
            result_total += 1;
        } else if (result_mid >= result_high_interval) {
            result += '0';
            result_total += 1;
        } else if (result_mid_lo <= result_low_interval && result_mid_hi < result_high_interval) {
            result += '1';
            result_ones += 1;
            result_total += 1;
        } else if (result_mid_hi >= result_high_interval && result_mid_lo > result_low_interval) {
            result += '0';
            result_total += 1;
        } else if (result_mid_hi >= result_high_interval && result_mid_lo <= result_low_interval && abs(result_mid - result_low_interval) <= abs(result_mid - result_high_interval)) {
            result += '1';
            result_ones += 1;
            result_total += 1;
        } else if (result_mid_hi >= result_high_interval && result_mid_lo <= result_low_interval && abs(result_mid - result_low_interval) >= abs(result_mid - result_high_interval)) {
            result += '0';
            result_total += 1;
        } else {
            result += '*';
            delay += 1;
        }

        if (mode == determinant_fixed || mode == determinant_2_fixed) {
            if (len_stream == result_total) {
                break;
            }
        }

        // std::cout << "A: " << a.number << std::endl;
        // std::cout << "B: " << b.number << std::endl;
        // std::cout << "Eps: " << epsilon << std::endl << std::endl;
        // std::cout << "Cycle: " << i << std::endl;
        // std::cout << "A_Low_Interval: " << a_low_interval << std::endl;
        // std::cout << "A_High_Interval: " << a_high_interval << std::endl;
        // std::cout << "B_Low_Interval: " << b_low_interval << std::endl;
        // std::cout << "B_High_Interval: " << b_high_interval << std::endl;
        // std::cout << "Calc_Result_Low_Interval: " << result_low_interval << std::endl;
        // std::cout << "Calc_Result_High_Interval: " << result_high_interval << std::endl;
        // std::cout << "Current_Result: " << result << std::endl;
        // std::cout << "Midpoint: " << result_mid << std::endl;
        // std::cout << "Midpoint+Eps: " << result_mid_hi << std::endl;
        // std::cout << "Midpoint-Eps: " << result_mid_lo << std::endl << std::endl;

        i--;
    }

    UnaryNumber output = {result, delay};

    // std::cout << "A: " << a.number << std::endl;
    // std::cout << "B: " << b.number << std::endl;
    // std::cout << "Output: " << output.number << std::endl << std::endl;

    return output;
}

bool unaryVerification(UnaryNumber a, UnaryNumber b, UnaryNumber Output, Operation x, double epsilon) {
    UnaryNumber Calc_Output;
    switch (x) {
        case add:
        case mul:
            Calc_Output = operation2(a, b, x, determinant_2_fixed, epsilon);
            break;
        case root:
        case mul2:
        case div2:
            Calc_Output = operation1(a, x, determinant_2_fixed, epsilon);
            break;
        default:
            throw std::exception();
            break;
    }
    return (toDouble(Calc_Output) == toDouble(Output));
}