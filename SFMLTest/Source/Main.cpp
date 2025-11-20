#include <Book/State.hpp>
#include <Book/StateStack.hpp>
#include <Book/States.hpp>
#include <Book/TitleState.hpp>
#include <Book/MenuState.hpp>
#include <Book/PauseState.hpp>
#include <Book/GameState.hpp>
#include <Book/EndState.hpp>
#include <Book/GameOverState.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Game");
	window.setFramerateLimit(60);

	State::Context context(window);
	StateStack stateStack(context);

	// Register all states
	stateStack.registerState<TitleState>(States::Title);
	stateStack.registerState<MenuState>(States::Menu);
	stateStack.registerState<PauseState>(States::Pause);
	stateStack.registerState<GameState>(States::Game);
	stateStack.registerState<EndState>(States::End);
	stateStack.registerState<GameOverState>(States::GameOver);

	// Push initial state
	stateStack.pushState(States::Title);

	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Time dt = clock.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			stateStack.handleEvent(event);
		}

		stateStack.update(dt);

		window.clear();
		stateStack.draw();
		window.display();
	}

	return 0;
}
