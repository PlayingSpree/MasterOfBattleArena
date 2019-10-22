#include "Game.h"

int main()
{
	srand(time(0));
	GameResources resources;

	//Create Window
	sf::RenderWindow Window;

	//Manage Time
	sf::Clock clock;

	//Game Class
	Game game(&clock, &Window, &resources);
	resources.CreateWindow(&Window);

	// run the program as long as the window is open
	while (game.window->isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (game.window->pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed) {
				game.window->close();
			}
			else if (event.type == sf::Event::TextEntered) {
				game.CharEnter(event.text.unicode);
			}
		}
		game.loop();
	}
	return 0;
}