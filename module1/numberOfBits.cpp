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
    return number;
}

//uint32_t userInput(std::istream& input) {
//    u_int32_t number;
//    input >> number;
//    return number;
//}

void run(std::istream& input, std::ostream& output) {
    u_int32_t number;
    input >> number;
    output << countNumberOfBits(number) << std::endl;
}

int main() {
    // testBaseCheckBits();
    run(std::cin, std::cout);
    return 0;
}