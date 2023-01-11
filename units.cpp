#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

enum rounding_mode {
    round_random,
    round_up,
    round_down
};

struct UnaryNumber {
    string number;
    int num_delay;
};


UnaryNumber toUnary(double n, rounding_mode mode = round_random, int delay = 0, int len = 0) {
    bool done = false;
    double ones = 0;
    string number;

    while (!done) {
        double ifone = abs(n - ((ones + 1) / (number.length() + 1)));
        double ifzero = abs(n - ((ones) / (number.length() + 1)));

        if (ifone == 0) {
            number += '1';
            done = true;
        } else if (ifzero == 0) {
            number += '0';
            done = true;
        } else if (ifone < ifzero) {
            ones++;
            number += '1';
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
            ones += digit;
            number += to_string(digit);
        }

        if (number.length() == len) {
            done = true;
        }
    }

    if (delay > 0) {
        int total_len = number.length() + delay;
        int num_delays = delay / (number.length() + 1);
        int extra_delays = delay % (number.length() + 1);
        string delay_number = string(num_delays, '*');

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
    string number = n.number;
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

UnaryNumber add(UnaryNumber a, UnaryNumber b) {
    string result;
    double a_total              = 0;
    double a_ones               = 0;
    double a_low_interval       = 0;
    double a_high_interval      = 1;

    double b_total              = 0;
    double b_ones               = 0;
    double b_low_interval       = 0;
    double b_high_interval      = 1;

    double last_a               = a_total;
    double last_b               = b_total;
    double result_total         = 0;
    double result_ones          = 0;
    double result_mid           = ((result_ones + 1) / (result_total + 2));
    double result_low_interval  = a_low_interval + b_low_interval;
    double result_high_interval = a_high_interval + b_high_interval;

    int len = max(a.number.length(), b.number.length());
    int delay = 0;

    cout << "A: " << a.number << endl;
    cout << "B: " << b.number << endl << endl;

    for (int i = 0; i < len; i++) {
        if (a.number[i] == '0' || a.number[i] == '1') {
            double change = a.number[i] - '0';
            a_total += 1;
            a_ones += change;
            if (change) {
                a_low_interval = max(a_low_interval, (((a_ones)/a_total) + ((a_ones - 1)/a_total)) / 2.0);
            } else {
                a_high_interval = min(a_high_interval, (((a_ones)/a_total) + ((a_ones + 1)/a_total)) / 2.0);
            }
        }
        if (b.number[i] == '0' || b.number[i] == '1') {
            double change = b.number[i] - '0';
            b_total += 1;
            b_ones += change;
            if (change) {
                b_low_interval = max(b_low_interval, (((b_ones)/b_total) + ((b_ones - 1)/b_total)) / 2.0);
            } else {
                b_high_interval = min(b_high_interval, (((b_ones)/b_total) + ((b_ones + 1)/b_total)) / 2.0);
            }
        }
        if (last_a != a_total || last_b != b_total) {
            last_a = a_total;
            last_b = b_total;
            result_low_interval  = a_low_interval + b_low_interval;
            result_high_interval = a_high_interval + b_high_interval;

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

            cout << "Digit: " << i << endl;
            cout << "A_Low_Interval: " << a_low_interval << endl;
            cout << "A_High_Interval: " << a_high_interval << endl;
            cout << "B_Low_Interval: " << b_low_interval << endl;
            cout << "B_High_Interval: " << b_high_interval << endl;
            cout << "Calc_Result_Low_Interval: " << result_low_interval << endl;
            cout << "Calc_Result_High_Interval: " << result_high_interval << endl;
            cout << "Current_Result: " << result << endl;
            cout << "Midpoint: " << result_mid << endl << endl;

            result_mid = result_total == 0 ? 0.5 : (1/(result_total + 1)) * (((result_ones) / (result_total)) + 0.5);

        } else {
            result += '*';
            delay += 1;
        }
    }
    cout << "Input Stream Finished" << endl << endl;

    bool done = false;
    while (!done) {

        if (result_mid <= result_low_interval) {
            result += '1';
            result_ones += 1;
            result_total += 1;
        } else if (result_mid >= result_high_interval) {
            result += '0';
            result_total += 1;
        } else {
            done = true;
        }

        cout << "Calc_Result_Low_Interval: " << result_low_interval << endl;
        cout << "Calc_Result_High_Interval: " << result_high_interval << endl;
        cout << "Current_Result: " << result << endl;
        cout << "Midpoint: " << result_mid << endl << endl;

        result_mid = result_total == 0 ? 0.5 : (1/(result_total + 1)) * (((result_ones) / (result_total)) + 0.5);
    }

    UnaryNumber output = {result, delay};
    cout << "Output: " << result << endl << endl;
    return output;
}

UnaryNumber mul(UnaryNumber a, UnaryNumber b) {
    string result;
    double a_total              = 0;
    double a_ones               = 0;
    double a_low_interval       = 0;
    double a_high_interval      = 1;

    double b_total              = 0;
    double b_ones               = 0;
    double b_low_interval       = 0;
    double b_high_interval      = 1;

    double last_a               = a_total;
    double last_b               = b_total;
    double result_total         = 0;
    double result_ones          = 0;
    double result_mid           = ((result_ones + 1) / (result_total + 2));
    double result_low_interval  = a_low_interval * b_low_interval;
    double result_high_interval = a_high_interval * b_high_interval;

    int len = max(a.number.length(), b.number.length());
    int delay = 0;

    cout << "A: " << a.number << endl;
    cout << "B: " << b.number << endl << endl;

    for (int i = 0; i < len; i++) {
        if (a.number[i] == '0' || a.number[i] == '1') {
            double change = a.number[i] - '0';
            a_total += 1;
            a_ones += change;
            if (change) {
                a_low_interval = max(a_low_interval, (((a_ones)/a_total) + ((a_ones - 1)/a_total)) / 2.0);
            } else {
                a_high_interval = min(a_high_interval, (((a_ones)/a_total) + ((a_ones + 1)/a_total)) / 2.0);
            }
        }
        if (b.number[i] == '0' || b.number[i] == '1') {
            double change = b.number[i] - '0';
            b_total += 1;
            b_ones += change;
            if (change) {
                b_low_interval = max(b_low_interval, (((b_ones)/b_total) + ((b_ones - 1)/b_total)) / 2.0);
            } else {
                b_high_interval = min(b_high_interval, (((b_ones)/b_total) + ((b_ones + 1)/b_total)) / 2.0);
            }
        }
        if (last_a != a_total || last_b != b_total) {
            last_a = a_total;
            last_b = b_total;
            result_low_interval  = a_low_interval * b_low_interval;
            result_high_interval = a_high_interval * b_high_interval;

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
            
            cout << "Digit: " << i << endl;
            cout << "A_Low_Interval: " << a_low_interval << endl;
            cout << "A_High_Interval: " << a_high_interval << endl;
            cout << "B_Low_Interval: " << b_low_interval << endl;
            cout << "B_High_Interval: " << b_high_interval << endl;
            cout << "Calc_Result_Low_Interval: " << result_low_interval << endl;
            cout << "Calc_Result_High_Interval: " << result_high_interval << endl;
            cout << "Current_Result: " << result << endl;
            cout << "Midpoint: " << result_mid << endl << endl;

            result_mid = result_total == 0 ? 0.5 : (1/(result_total + 1)) * (((result_ones) / (result_total)) + 0.5);
            
        } else {
            result += '*';
            delay += 1;
        }
    }
    cout << "Input Stream Finished" << result_mid << endl << endl;

    bool done = false;
    while (!done) {

        if (result_mid <= result_low_interval) {
            result += '1';
            result_ones += 1;
            result_total += 1;
        } else if (result_mid >= result_high_interval) {
            result += '0';
            result_total += 1;
        } else {
            done = true;
        }

        cout << "Calc_Result_Low_Interval: " << result_low_interval << endl;
        cout << "Calc_Result_High_Interval: " << result_high_interval << endl;
        cout << "Current_Result: " << result << endl;
        cout << "Midpoint: " << result_mid << endl << endl;

        result_mid = result_total == 0 ? 0.5 : (1/(result_total + 1)) * (((result_ones) / (result_total)) + 0.5);
    }

    UnaryNumber output = {result, delay};
    cout << "Output: " << result << endl << endl;
    return output;
}

int main() {
    srand(0);
    double a = 1.0/4.0;
    double b = 2.0/4.0;
    double c = 1.0/4.0;

    UnaryNumber ab = add(toUnary(a), toUnary(b));
    UnaryNumber ac = add(toUnary(a), toUnary(c));
    UnaryNumber abc = mul(ab, ac);
    cout << "Unary: " << ab.number << endl;
    cout << "Float: " << toDouble(ab) << endl;
    cout << "Delay: " << ab.num_delay << endl;
    cout << "Unary: " << ac.number << endl;
    cout << "Float: " << toDouble(ac) << endl;
    cout << "Delay: " << ac.num_delay << endl;
    cout << "Unary: " << abc.number << endl;
    cout << "Float: " << toDouble(abc) << endl;
    cout << "Delay: " << abc.num_delay << endl;
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