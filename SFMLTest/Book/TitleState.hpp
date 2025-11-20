#pragma once

#include <Book/State.hpp>
#include <SFML/Graphics/Text.hpp>

class TitleState : public State
{
public:
	TitleState(StateStack& stack, Context context);
	virtual ~TitleState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	sf::Text mText;
	sf::Font mFont;
	bool mShowText;
	sf::Time mTextEffectTime;
};

