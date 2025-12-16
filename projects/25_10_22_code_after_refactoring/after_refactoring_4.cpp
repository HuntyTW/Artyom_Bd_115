#include <cmath>
#include <iostream>

int main(int argc, char* argv[]) {
    double angle;
    std::cout << "введите угол" << std::endl;
    std::cin >> angle;

    double cos2x = std::cos(double(2 * angle));
    if (cos2x != 1) {
        double sin2x = std::sin(double(2 * angle));
        double result = (1 + sin2x) / (1 - cos2x);
        std::cout << "F(x1) = " << result << std::endl;
    } else if (cos2x == 1) {
        std::cout << "неверный ввод для F(x1)" << std::endl;
    }

    double tang = std::tan(angle);
    if (tang != 1) {
        double squaretan = tang * tang;
        double result2 = (1 + squaretan) / (1 - squaretan);
        std::cout << "F(x2) = " << result2 << std::endl;
    } else if (std::tan(angle) == 1){
        std::cout << "неверный ввод для F(x2)" << std::endl;
    }

    std::system("pause");
}
