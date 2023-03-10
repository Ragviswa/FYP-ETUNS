#include <string>
#ifndef UNARY_H // include guard
#define UNARY_H

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

UnaryNumber toUnary(double n, rounding_mode mode = round_random, int delay = 0, int len = 0);

double toDouble(UnaryNumber n);

double determinant_interval_calc(double ones, double total, double length, Interval interval);

double indeterminant_interval_calc(double ones, double total, double length, Interval interval);

double indeterminant_result_calc(double ones, double total, double epsilon);

UnaryNumber operation(UnaryNumber a, UnaryNumber b, Operation x, OperationType mode, double epsilon = 0);

#endif