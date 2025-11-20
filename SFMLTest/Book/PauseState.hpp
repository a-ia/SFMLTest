#pragma once

#include <Book/State.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class PauseState : public State
{
public:
	PauseState(StateStack& stack, Context context);
	virtual ~PauseState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Text mPausedText;
	sf::Text mInstructionText;
	sf::Font mFont;
};

