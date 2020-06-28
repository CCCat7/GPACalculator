#include "GPACalculator.hpp"

int main(int argc, char *argv[])
{
    if(argc != 2) {
        std::cout << "Usage: GPACalculator score.txt rule.txt" << std::endl;
        exit(0);
    }

    GPACalculator GPA_Proc(argv[1], argv[2]);
    GPA_Proc.Run();
    return 0;
}