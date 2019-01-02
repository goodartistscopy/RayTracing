#include "vec3.h"
#include "mat4.h"

int main(int argc, char *argv[])
{
    Mat4 m;
    Vec4 a(1, 2, 3, 4);
    Vec3 c(2.0);

    m.setIdentity();
    std::cout << "m = " << m << std::endl;
    std::cout << "m[2] = " << m[2] << std::endl;

    Vec4 b = m * a;
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";

    Vec4 d = m * c;
    std::cout << "d = " << d << "\n";

    std::cout << "a to vec3 = " << a.to_vec3() << "\n";

    return 0;
}
