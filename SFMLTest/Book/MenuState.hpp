#pragma once

#include <Book/State.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuState : public State
{
public:
	MenuState(StateStack& stack, Context context);
	virtual ~MenuState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	void updateOptionText();

private:
	enum OptionNames
	{
		Enter,
		Exit,
	};

private:
	sf::Text mTitleText;
	sf::Text mEnterText;
	sf::Text mExitText;
	sf::Font mFont;
	std::size_t mOptionIndex;
};

