#include "renderer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

int main()
{
	Renderer app(2);
	app.initRenderer();
	app.draw();

	return 0;
}
