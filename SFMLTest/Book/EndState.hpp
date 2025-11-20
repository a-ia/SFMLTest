#pragma once

#include <Book/State.hpp>
#include <SFML/Graphics/Text.hpp>

class EndState : public State
{
public:
	EndState(StateStack& stack, Context context);
	virtual ~EndState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Text mText;
	sf::Font mFont;
};

