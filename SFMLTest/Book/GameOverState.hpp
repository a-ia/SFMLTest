#pragma once

#include <Book/State.hpp>
#include <SFML/Graphics/Text.hpp>

class GameOverState : public State
{
public:
	GameOverState(StateStack& stack, Context context);
	virtual ~GameOverState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Text mGameOverText;
	sf::Text mInstructionText;
	sf::Font mFont;
};

