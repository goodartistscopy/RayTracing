#include "vec3.h"
#include <iostream>

int main(int argc, char *argv[])
{
    Vec3 a(1, 2, 2);
    Vec3 b(9, 0, 1);
    Vec3 c(a);

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = a = " << c << std::endl;

    std::cout << "-a = "  << -a << std::endl;
    std::cout << "a+b = "  << a + b << std::endl;
    std::cout << "a-b = "  << a - b << std::endl;
    std::cout << "a*b = "  << a * b << std::endl;
    std::cout << "b/a = "  << b / a << std::endl;
    std::cout << "3 * a = "  << 3 * a << std::endl;

    std::cout << "dot(a,b) = "  << dot(a,b) << std::endl;
    std::cout << "cross(a,b) = "  << cross(a,b) << std::endl;

    std::cout << "a length = " << a.length() << std::endl;
    std::cout << "b length = " << b.length() << std::endl;
    b.make_unit_vector();
    std::cout << "normalized b length = " << b.length() << std::endl; 

    return 0;
}
