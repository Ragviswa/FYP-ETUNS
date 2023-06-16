#include "Unary.h"
#include <exception>
#include <iostream>
#include <cmath>

int singleAdd(int emax = 1, int maxops = 1000) {
    std::cout << "Epsilon,Length,Delay,TotalError,CalcError" << std::endl;
    for (int e = 0; e < emax; e++) {
        double epsilon = e * 0.05;
        for (int i = 1; i <= 64; i*=2) {
            for (int j = 0 ; j < maxops; j++) {
                double DoubleA = (double) rand()/RAND_MAX;
                double DoubleB = (double) rand()/RAND_MAX;
                UnaryNumber InputA = toUnary(DoubleA, round_random, 0, i);
                UnaryNumber InputB = toUnary(DoubleB, round_random, 0, i);
                UnaryNumber Output = operation2(InputA, InputB, add, determinant_fixed, epsilon);
                double total_error = abs(toDouble(Output) - ((DoubleA+DoubleB)/2.0))/((DoubleA+DoubleB)/2.0);
                double calc_error  = abs(toDouble(Output) - ((toDouble(InputA)+toDouble(InputB))/2.0));
                //assert(toDouble(Output) == ((toDouble(InputA)+toDouble(InputB))/2.0));
                std::cout << epsilon << "," << i << "," << Output.num_delay << "," << total_error << "," << calc_error << std::endl;
            }
        }
    }
    return 0;
}

int singleMul(int emax = 1, int maxops = 1000) {
    std::cout << "Epsilon,Length,Delay,TotalError,CalcError" << std::endl;
    for (int e = 0; e < emax; e++) {
        double epsilon = e * 0.05;
        for (int i = 1; i <= 64; i*=2) {
            for (int j = 0 ; j < maxops; j++) {
                double DoubleA = (double) rand()/RAND_MAX;
                double DoubleB = (double) rand()/RAND_MAX;
                UnaryNumber InputA = toUnary(DoubleA, round_random, 0, i);
                UnaryNumber InputB = toUnary(DoubleB, round_random, 0, i);
                UnaryNumber Output = operation2(InputA, InputB, mul, determinant_fixed, epsilon);
                double total_error = abs(toDouble(Output) - (DoubleA*DoubleB))/(DoubleA*DoubleB);
                double calc_error  = abs(toDouble(Output) - (toDouble(InputA)*toDouble(InputB)));
                //assert(toDouble(Output) == (toDouble(InputA)*toDouble(InputB)));
                std::cout << epsilon << "," << i << "," << Output.num_delay << "," << total_error << "," << calc_error << std::endl;
            }
        }
    }
    return 0;
}

int doubleAdd (int emax = 1, int maxops = 1000) {
    std::cout << "Epsilon,Length,Delay,TotalError,CalcError" << std::endl;
    for (int e = 0; e < emax; e++) {
        double epsilon = e * 0.05;
        for (int i = 1; i <= 64; i*=2) {
            for (int j = 0 ; j < maxops; j++) {
                double DoubleA = (double) rand()/RAND_MAX;
                double DoubleB = (double) rand()/RAND_MAX;
                double DoubleC = (double) rand()/RAND_MAX;
                double DoubleD = (double) rand()/RAND_MAX;
                UnaryNumber InputA = toUnary(DoubleA, round_random, 0, i);
                UnaryNumber InputB = toUnary(DoubleB, round_random, 0, i);
                UnaryNumber InputC = toUnary(DoubleC, round_random, 0, i);
                UnaryNumber InputD = toUnary(DoubleD, round_random, 0, i);
                UnaryNumber OutputAB    = operation2(InputA, InputB, add, determinant_fixed, epsilon);
                UnaryNumber OutputCD    = operation2(InputC, InputD, add, determinant_fixed, epsilon);
                UnaryNumber Output      = operation2(OutputAB, OutputCD, add, determinant_fixed, epsilon);
                double total_error      = abs(toDouble(Output) - ((((DoubleA+DoubleB)/2.0)+((DoubleC+DoubleD)/2.0))/2.0))/((((DoubleA+DoubleB)/2.0)+((DoubleC+DoubleD)/2.0))/2.0);
                double calc_error       = abs(toDouble(Output) - ((((toDouble(InputA)+toDouble(InputB))/2.0)+((toDouble(InputC)+toDouble(InputD))/2.0))/2.0));
                //assert(toDouble(Output) == (((toDouble(InputA) + toDouble(InputB))/2.0 + (toDouble(InputC) + toDouble(InputD))/2.0)/2.0));
                std::cout << epsilon << "," << i << "," << Output.num_delay << "," << total_error << "," << calc_error << std::endl;
            }
        }
    }
    return 0;
}

int doubleMul (int emax = 1, int maxops = 1000) {
     std::cout << "Epsilon,Length,Delay,TotalError,CalcError" << std::endl;
    for (int e = 0; e < emax; e++) {
        double epsilon = e * 0.05;
        for (int i = 1; i <= 64; i*=2) {
            for (int j = 0 ; j < maxops; j++) {
                double DoubleA = (double) rand()/RAND_MAX;
                double DoubleB = (double) rand()/RAND_MAX;
                double DoubleC = (double) rand()/RAND_MAX;
                double DoubleD = (double) rand()/RAND_MAX;
                UnaryNumber InputA = toUnary(DoubleA, round_random, 0, i);
                UnaryNumber InputB = toUnary(DoubleB, round_random, 0, i);
                UnaryNumber InputC = toUnary(DoubleC, round_random, 0, i);
                UnaryNumber InputD = toUnary(DoubleD, round_random, 0, i);
                UnaryNumber OutputAB    = operation2(InputA, InputB, mul, determinant_fixed, epsilon);
                UnaryNumber OutputCD    = operation2(InputC, InputD, mul, determinant_fixed, epsilon);
                UnaryNumber Output      = operation2(OutputAB, OutputCD, mul, determinant_fixed, epsilon);
                double total_error  = abs(toDouble(Output) - ((DoubleA*DoubleB)*(DoubleC*DoubleD)))/((DoubleA*DoubleB)*(DoubleC*DoubleD));
                double calc_error   = abs(toDouble(Output) - ((toDouble(InputA)*toDouble(InputB))*(toDouble(InputC)*toDouble(InputD))));
                //assert(toDouble(Output) == ((toDouble(InputA)*toDouble(InputB)) * (toDouble(InputC)*toDouble(InputD))));
                std::cout << epsilon << "," << i << "," << Output.num_delay << "," << total_error << "," << calc_error << std::endl;
            }
        }
    }
    return 0;
}

int doubleAddMul (int emax = 1, int maxops = 1000) {
     std::cout << "Epsilon,Length,Delay,TotalError,CalcError" << std::endl;
    for (int e = 0; e < emax; e++) {
        double epsilon = e * 0.05;
        for (int i = 1; i <= 64; i*=2) {
            for (int j = 0 ; j < maxops; j++) {
                double DoubleA = (double) rand()/RAND_MAX;
                double DoubleB = (double) rand()/RAND_MAX;
                double DoubleC = (double) rand()/RAND_MAX;
                double DoubleD = (double) rand()/RAND_MAX;
                UnaryNumber InputA = toUnary(DoubleA, round_random, 0, i);
                UnaryNumber InputB = toUnary(DoubleB, round_random, 0, i);
                UnaryNumber InputC = toUnary(DoubleC, round_random, 0, i);
                UnaryNumber InputD = toUnary(DoubleD, round_random, 0, i);
                UnaryNumber OutputAB    = operation2(InputA, InputB, add, determinant_fixed, epsilon);
                UnaryNumber OutputCD    = operation2(InputC, InputD, add, determinant_fixed, epsilon);
                UnaryNumber Output      = operation2(OutputAB, OutputCD, mul, determinant_fixed, epsilon);
                double total_error      = abs(toDouble(Output) - (((DoubleA+DoubleB)/2.0)*((DoubleC+DoubleD)/2.0)))/(((DoubleA+DoubleB)/2.0)*((DoubleC+DoubleD)/2.0));
                double calc_error       = abs(toDouble(Output) - (((toDouble(InputA)+toDouble(InputB))/2.0)*((toDouble(InputC)+toDouble(InputD))/2.0)));
                //assert(toDouble(Output) == (((toDouble(InputA) + toDouble(InputB))/2.0 + (toDouble(InputC) + toDouble(InputD))/2.0)/2.0));
                std::cout << epsilon << "," << i << "," << Output.num_delay << "," << total_error << "," << calc_error << std::endl;
            }
        }
    }
    return 0;
}

int doubleMulAdd (int emax = 1, int maxops = 1000) {
    std::cout << "Epsilon,Length,Delay,TotalError,CalcError" << std::endl;
    for (int e = 0; e < emax; e++) {
        double epsilon = e * 0.05;
        for (int i = 1; i <= 64; i*=2) {
            for (int j = 0 ; j < maxops; j++) {
                double DoubleA = (double) rand()/RAND_MAX;
                double DoubleB = (double) rand()/RAND_MAX;
                double DoubleC = (double) rand()/RAND_MAX;
                double DoubleD = (double) rand()/RAND_MAX;
                UnaryNumber InputA = toUnary(DoubleA, round_random, 0, i);
                UnaryNumber InputB = toUnary(DoubleB, round_random, 0, i);
                UnaryNumber InputC = toUnary(DoubleC, round_random, 0, i);
                UnaryNumber InputD = toUnary(DoubleD, round_random, 0, i);
                UnaryNumber OutputAB    = operation2(InputA, InputB, mul, determinant_fixed, epsilon);
                UnaryNumber OutputCD    = operation2(InputC, InputD, mul, determinant_fixed, epsilon);
                UnaryNumber Output      = operation2(OutputAB, OutputCD, add, determinant_fixed, epsilon);
                double total_error      = abs(toDouble(Output) - (((DoubleA*DoubleB)+(DoubleC*DoubleD))/2.0))/(((DoubleA*DoubleB)+(DoubleC*DoubleD))/2.0);
                double calc_error       = abs(toDouble(Output) - (((toDouble(InputA)*toDouble(InputB))+(toDouble(InputC)*toDouble(InputD)))/2.0));
                //assert(toDouble(Output) == (((toDouble(InputA) + toDouble(InputB))/2.0 + (toDouble(InputC) + toDouble(InputD))/2.0)/2.0));
                std::cout << epsilon << "," << i << "," << Output.num_delay << "," << total_error << "," << calc_error << std::endl;
            }
        }
    }
    return 0;
}

int singleSqrt (int emax = 1, int maxops = 1000) {
    std::cout << "Epsilon,Length,Delay,TotalError,CalcError" << std::endl;
    for (int e = 0; e < emax; e++) {
        double epsilon = e * 0.05;
        for (int i = 1; i <= 64; i*=2) {
            for (int j = 0 ; j < maxops; j++) {
                double DoubleA = (double) rand()/RAND_MAX;
                UnaryNumber InputA = toUnary(DoubleA, round_random, 0, i);
                UnaryNumber Output    = operation1(InputA, root, determinant_fixed, epsilon);
                double total_error      = abs(toDouble(Output) - std::sqrt(DoubleA))/(std::sqrt(DoubleA));
                double calc_error       = abs(toDouble(Output) - (std::sqrt(toDouble(InputA))));
                //assert(toDouble(Output) == std::round(i * sqrt(toDouble(InputA))) / i);
                std::cout << epsilon << "," << i << "," << Output.num_delay << "," << total_error << "," << calc_error << std::endl;
            }
        }
    }
    return 0;
}

int singleMul2 (int emax = 1, int maxops = 1000) {
    std::cout << "Epsilon,Length,Delay,TotalError,CalcError" << std::endl;
    for (int e = 0; e < emax; e++) {
        double epsilon = e * 0.05;
        for (int i = 1; i <= 64; i*=2) {
            for (int j = 0 ; j < maxops; j++) {
                double DoubleA = (double) rand()/RAND_MAX;
                UnaryNumber InputA = toUnary(DoubleA, round_random, 0, i);
                UnaryNumber Output    = operation1(InputA, mul2, determinant_fixed, epsilon);
                double total_error      = abs(toDouble(Output) - (DoubleA * 2.0))/(DoubleA * 2.0);
                double calc_error       = abs(toDouble(Output) - (toDouble(InputA) * 2.0));
                //assert(toDouble(Output) == (std::min(toDouble(InputA) * 2.0, 1.0)));
                std::cout << epsilon << "," << i << "," << Output.num_delay << "," << total_error << "," << calc_error << std::endl;
            }
        }
    }
    return 0;
}

int singleDiv2 (int emax = 1, int maxops = 1000) {
    std::cout << "Epsilon,Length,Delay,TotalError,CalcError" << std::endl;
    for (int e = 0; e < emax; e++) {
        double epsilon = e * 0.05;
        for (int i = 1; i <= 64; i*=2) {
            for (int j = 0 ; j < maxops; j++) {
                double DoubleA = (double) rand()/RAND_MAX;
                UnaryNumber InputA = toUnary(DoubleA, round_random, 0, i);
                UnaryNumber Output    = operation1(InputA, div2, determinant_fixed, epsilon);
                double total_error      = abs(toDouble(Output) - (DoubleA / 2.0))/(DoubleA / 2.0);
                double calc_error       = abs(toDouble(Output) - (toDouble(InputA) / 2.0));
                //assert(toDouble(Output) == std::round(i * (toDouble(InputA) / 2.0)) / i);
                std::cout << epsilon << "," << i << "," << Output.num_delay << "," << total_error << "," << calc_error << std::endl;
            }
        }
    }
    return 0;
}

int test() {
    double a = 1.0/2.0;
    double b = 1.0/4.0;
    double c = 3.0/4.0;
    double d = 1.0/3.0;

    UnaryNumber InputA = toUnary(a, round_random, 0);
    UnaryNumber InputB = toUnary(b, round_random, 0);
    UnaryNumber InputC = toUnary(c, round_random, 0);
    UnaryNumber InputD = toUnary(d, round_random, 0);

    std::cout << "----------------------Inputs------------------------" << std::endl;
    std::cout << "UnaryA: " << InputA.number << std::endl;
    std::cout << "FloatA: " << toDouble(InputA) << std::endl;
    std::cout << "DelayA: " << InputA.num_delay << std::endl;
    std::cout << "UnaryB: " << InputB.number << std::endl;
    std::cout << "FloatB: " << toDouble(InputB) << std::endl;
    std::cout << "DelayB: " << InputB.num_delay << std::endl;
    std::cout << "UnaryC: " << InputC.number << std::endl;
    std::cout << "FloatC: " << toDouble(InputC) << std::endl;
    std::cout << "DelayC: " << InputC.num_delay << std::endl;
    std::cout << "UnaryD: " << InputD.number << std::endl;
    std::cout << "FloatD: " << toDouble(InputD) << std::endl;
    std::cout << "DelayD: " << InputD.num_delay << std::endl;
    std::cout << "----------------------------------------------------" << std::endl << std::endl;

    UnaryNumber aUnary = operation1(InputA, mul2, determinant_fixed);
    UnaryNumber bUnary = operation1(InputB, mul2, determinant_fixed);
    UnaryNumber cUnary = operation1(InputC, mul2, determinant_fixed);
    UnaryNumber dUnary = operation1(InputD, mul2, determinant_fixed);
    
    std::cout << "----------------------Outputs-----------------------" << std::endl;
    std::cout << "UnaryA: " << aUnary.number << std::endl;
    std::cout << "FloatA: " << toDouble(aUnary) << std::endl;
    std::cout << "DelayA: " << aUnary.num_delay << std::endl;
    std::cout << "Unaryb: " << bUnary.number << std::endl;
    std::cout << "Floatb: " << toDouble(bUnary) << std::endl;
    std::cout << "Delayb: " << bUnary.num_delay << std::endl;
    std::cout << "UnaryC: " << cUnary.number << std::endl;
    std::cout << "FloatC: " << toDouble(cUnary) << std::endl;
    std::cout << "DelayC: " << cUnary.num_delay << std::endl;
    std::cout << "UnaryD: " << dUnary.number << std::endl;
    std::cout << "FloatD: " << toDouble(dUnary) << std::endl;
    std::cout << "DelayD: " << dUnary.num_delay << std::endl;
    std::cout << "----------------------------------------------------" << std::endl << std::endl;
    return 0;
}

int main() {
    srand((unsigned)time(NULL));
    singleDiv2(1);
}