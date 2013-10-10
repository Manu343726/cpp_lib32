
#include "../code/headers/math_2d.h"
#include "../code/headers/utils/console.h"

#include <iostream>

namespace console = dl32::console;

int main()
{
	dl32::vector_2d<float> a(0.0f,0.0f) , b(0.0f,1.0f) , c(1.0f,0.0f) , d(1.0f,1.0f);

	c = a + (b*=2.0f) + b;

	std::cout << console::push_style << console::style_change::FOREGROUND << console::color::GREEN;

	std::cout << c.to_string() << std::endl;
	std::cout << b.to_string() << std::endl;
	std::cout << std::boolalpha << (c != a) << std::endl;

	std::cin.sync();
	std::cin.get();
}