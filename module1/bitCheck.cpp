// Если в числе содержится только один бит со значением 1, записать в выходной поток OK.
// Иначе записать FAIL.
// Необходимо использование битовых операций.
// Использование арифметических операций запрещено.

// Формат ввода
// Входное число лежит в диапазоне 0..232-1 и вводится в десятичном виде.

// Формат вывода
// Строка OK или FAIL


#include <iostream>
#include <cstdint>
#include <assert.h>
#include <sstream>


u_int8_t countNumberOfBits(u_int32_t number) {
    u_int8_t counterBit = 0;
    while (number) {
        if (number & 1) {
            ++counterBit;
        }
        number = number >> 1;
    }
    return counterBit;
}

std::string bitCheck(u_int32_t number) {
    if (countNumberOfBits(number) == 1) {
        return "OK";
    }

    return "FAIL";
}

uint32_t userInput(std::istream& input) {
    u_int32_t number;
    input >> number;
    return number;
}

void run(std::istream& input, std::ostream& output) {
    output << bitCheck(userInput(input));
}

void testBaseCheckBits() {
    std::cout << "Success test: ";
    {
        std::stringstream sInput;
        sInput << "1";

        std::stringstream sOutput;
        run(sInput, sOutput);

        assert(sOutput.str() == "OK");
    }
    std::cout << "OK\n";
    std::cout << "Success test: ";
    {
        std::stringstream sInput;
        sInput << "8";

        std::stringstream sOutput;
        run(sInput, sOutput);

        assert(sOutput.str() == "OK");
    }
    std::cout << "OK\n";
    std::cout << "Failed test: ";
    {
        std::stringstream sInput;
        sInput << "3";

        std::stringstream sOutput;
        run(sInput, sOutput);

        assert(sOutput.str() == "FAIL");
    }
    std::cout << "OK\n";
}

int main(int argc, char *argv[]) {
    testBaseCheckBits();
    run(std::cin, std::cout);
    return 0;
}
