//Подсчитать кол-во единичных бит в входном числе , стоящих на четных позициях.
//Позиции битов нумеруются с 0.
//
//Необходимо использование битовых операций.
//Использование арифметических операций запрещено.
//
//Формат ввода
//Входное число лежит в диапазоне 0..232-1 и вводится в десятичном виде.
//
//Формат вывода
//Целое число - количество бит


#include <iostream>
#include <cstdint>

u_int32_t countNumberOfBits(u_int32_t number) {
    u_int32_t counterBit = 0;
    u_int32_t i = 0;
    while (number) {
        if (i % 2 == 0 && number & 1) {
            ++counterBit;
        }
        number = number >> 1;
        ++i;
    }
    return counterBit;
}

void run(std::istream& input, std::ostream& output) {
    u_int32_t number;
    input >> number;
    output << countNumberOfBits(number);
}

int main() {
    run(std::cin, std::cout);
    return 0;
}