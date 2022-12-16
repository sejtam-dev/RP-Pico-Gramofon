#ifndef GRAMOFON_MATH_HPP
#define GRAMOFON_MATH_HPP

template<typename T>
T map(T value, T inputMin, T inputMax, T outputMin, T outputMax) {
    return (value - inputMin) * (outputMax - outputMin) / (inputMax - inputMin) + outputMin;
}

#endif //GRAMOFON_MATH_HPP
