#include <graphics/Window.hpp>
#include <iostream>

namespace graphics
{
Window::Window(const char* title, int width, int height, SDL_WindowFlags flags) : width(width), height(height)
{
	window = SDL_CreateWindow(title, width, height, flags);

	std::cout << "Window was created" << std::endl;

	SDL_RaiseWindow(window);
}

Window::~Window()
{
	SDL_DestroyWindow(window);

	std::cout << "Window was deleted" << std::endl;
}

bool Window::pollEvent(SDL_Event& event)
{
	bool return_value = SDL_PollEvent(&event);

	if (return_value)
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			running = false;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (event.key.scancode == SDL_SCANCODE_F11)
			{
				SDL_SetWindowFullscreen(window, fullscreen);
				fullscreen = !fullscreen;
			}
			break;
		}
	}

	return return_value;
}

glm::ivec2 Window::getWindowSize() const
{
	return { width, height };
}

bool Window::isOpen() const
{
	return running;
}
} //namespace graphics