#include "vec3.h"
#include "color.h"

#include <iostream>

int main() {
    std::cerr << "Started" << std::endl;

    const int side = 100;

    // Image
    const int image_width = side;
    const int image_height = side;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n100\n";

    for (int j = 0; j < image_height; ++j) {
        if (j % 10 == 0) {
            int percent = int(100 * float(j) / (image_height - 1));
            std::cerr << percent << "\% done.\n";
            // std::cerr << j << " out of " << image_height << " rows rendered.\n";
        }
        for (int i = 0; i < image_width; ++i) {
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0.25;

            int ir = static_cast<int>(side * r);
            int ig = static_cast<int>(side * g);
            int ib = static_cast<int>(side * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    std::cerr << "Done!" << std::endl;
    return 0;
}