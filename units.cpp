#include "Unary.h"
#include <exception>
#include <iostream>
#include <cmath>

int singleAdd(int emax = 1, int maxops = 1000) {
    std::cout << "Length,Delay,TotalError,CalcError" << std::endl;
    for (int e = 0; e < emax; e++) {
        double epsilon = e * 0.05;
        for (int i = 1; i <= 64; i*=2) {
            for (int j = 0 ; j < maxops; j++) {
                double DoubleA = (double) rand()/RAND_MAX;
                double DoubleB = (double) rand()/RAND_MAX;
                UnaryNumber InputA = toUnary(DoubleA, round_random, 0, i);
                UnaryNumber InputB = toUnary(DoubleB, round_random, 0, i);
                UnaryNumber Output = operation(InputA, InputB, add, determinant_fixed, epsilon);
                double total_error = abs(toDouble(Output) - ((DoubleA+DoubleB)/2.0))/((DoubleA+DoubleB)/2.0);
                double calc_error  = ((toDouble(InputA)+toDouble(InputB))/2.0) ? abs(toDouble(Output) - ((toDouble(InputA)+toDouble(InputB))/2.0)) : 0;
                //assert(toDouble(Output) == ((toDouble(InputA)+toDouble(InputB))/2.0));
                std::cout << i << "," << Output.num_delay << "," << total_error << "," << calc_error << std::endl;
            }
        }
    }
    return 0;
}

int singleMul(int emax = 1, int maxops = 1000) {
    std::cout << "Length,Delay,TotalError,CalcError" << std::endl;
    for (int e = 0; e < emax; e++) {
        double epsilon = e * 0.05;
        for (int i = 1; i <= 64; i*=2) {
            for (int j = 0 ; j < maxops; j++) {
                double DoubleA = (double) rand()/RAND_MAX;
                double DoubleB = (double) rand()/RAND_MAX;
                UnaryNumber InputA = toUnary(DoubleA, round_random, 0, i);
                UnaryNumber InputB = toUnary(DoubleB, round_random, 0, i);
                UnaryNumber Output = operation(InputA, InputB, mul, determinant_fixed, epsilon);
                double total_error = abs(toDouble(Output) - (DoubleA*DoubleB))/(DoubleA*DoubleB);
                double calc_error  = (toDouble(InputA)*toDouble(InputB)) ? abs(toDouble(Output) - (toDouble(InputA)*toDouble(InputB))) : 0;
                //assert(toDouble(Output) == (toDouble(InputA)*toDouble(InputB)));
                std::cout << i << "," << Output.num_delay << "," << total_error << "," << calc_error << std::endl;
            }
        }
    }
    return 0;
}

int doubleAdd (int emax = 1, int maxops = 1000) {
    std::cout << "Length,Delay,TotalError,CalcError" << std::endl;
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
                UnaryNumber OutputAB    = operation(InputA, InputB, add, determinant_fixed, epsilon);
                UnaryNumber OutputCD    = operation(InputC, InputD, add, determinant_fixed, epsilon);
                UnaryNumber Output      = operation(OutputAB, OutputCD, add, determinant_fixed, epsilon);
                double total_error      = abs(toDouble(Output) - (((DoubleA+DoubleB)/2.0)+((DoubleC+DoubleD)/2.0)))/(((DoubleA+DoubleB)/2.0)+((DoubleC+DoubleD)/2.0));
                double calc_error       = (((toDouble(InputA)+toDouble(InputB))/2.0)+((toDouble(InputC)+toDouble(InputD))/2.0)) ? abs(toDouble(Output) - (((toDouble(InputA)+toDouble(InputB))/2.0)+((toDouble(InputC)+toDouble(InputD))/2.0))) : 0;
                //assert(toDouble(Output) == (((toDouble(InputA) + toDouble(InputB))/2.0 + (toDouble(InputC) + toDouble(InputD))/2.0)/2.0));
                std::cout << i << "," << Output.num_delay << "," << total_error << "," << calc_error << std::endl;
            }
        }
    }
    return 0;
}

int doubleMul (int emax = 1, int maxops = 1000) {
    std::cout << "Length,Delay,TotalError,CalcError" << std::endl;
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
                UnaryNumber OutputAB    = operation(InputA, InputB, mul, determinant_fixed, epsilon);
                UnaryNumber OutputCD    = operation(InputC, InputD, mul, determinant_fixed, epsilon);
                UnaryNumber Output      = operation(OutputAB, OutputCD, mul, determinant_fixed, epsilon);
                double total_error  = abs(toDouble(Output) - ((DoubleA*DoubleB)*(DoubleC*DoubleD)))/((DoubleA*DoubleB)*(DoubleC*DoubleD));
                double calc_error   = ((toDouble(InputA)*toDouble(InputB))*(toDouble(InputC)*toDouble(InputD))) ? abs(toDouble(Output) - ((toDouble(InputA)*toDouble(InputB))*(toDouble(InputC)*toDouble(InputD)))) : 0;
                //assert(toDouble(Output) == (((toDouble(InputA) + toDouble(InputB))/2.0 + (toDouble(InputC) + toDouble(InputD))/2.0)/2.0));
                std::cout << i << "," << Output.num_delay << "," << total_error << "," << calc_error << std::endl;
            }
        }
    }
    return 0;
}

int test() {
    double a = 0.0/100.0;
    double b = 0/100.0;
    double c = 2.0/4.0;
    double d = 2.0/4.0;

    UnaryNumber InputA = toUnary(a, round_random, 0, 2);
    UnaryNumber InputB = toUnary(b, round_random, 0, 2);
    UnaryNumber InputC = toUnary(c, round_random, 0, 2);
    UnaryNumber InputD = toUnary(d, round_random, 0, 2);

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

    UnaryNumber ab = operation(InputA, InputB, add, determinant_free);
    UnaryNumber cd = operation(InputC, InputD, add, determinant_free);
    
    std::cout << "----------------------Outputs-----------------------" << std::endl;
    std::cout << "UnaryAB: " << ab.number << std::endl;
    std::cout << "FloatAB: " << toDouble(ab) << std::endl;
    std::cout << "DelayAB: " << ab.num_delay << std::endl;
    std::cout << "UnaryCD: " << cd.number << std::endl;
    std::cout << "FloatCD: " << toDouble(cd) << std::endl;
    std::cout << "DelayCD: " << cd.num_delay << std::endl;
    std::cout << "----------------------------------------------------" << std::endl << std::endl;
    assert(toDouble(ab) == ((toDouble(InputA)+toDouble(InputB))/2.0));
    assert(toDouble(cd) == ((toDouble(InputC)+toDouble(InputD))/2.0));
    return 0;
}

int main() {
    srand((unsigned)time(NULL));
    test();
}