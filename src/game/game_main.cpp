#define SFML_STATIC
#include <SFML/Graphics.hpp>

int mainLoop() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				return 0;
			}
		}

		window.clear(sf::Color::Black);
		window.display();
	}

	return EXIT_SUCCESS;
}

int main() {
	return mainLoop();
}
