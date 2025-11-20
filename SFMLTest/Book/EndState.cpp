#include <Book/EndState.hpp>
#include <Book/StateStack.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

EndState::EndState(StateStack& stack, Context context)
	: State(stack, context)
{
	if (!mFont.loadFromFile("Media/GIGI.ttf"))
	{
	}

	mText.setFont(mFont);
	mText.setString("Thanks for playing!");
	mText.setCharacterSize(30);
	mText.setFillColor(sf::Color::Black);
	sf::FloatRect bounds = mText.getLocalBounds();
	mText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	mText.setPosition(context.window->getSize().x / 2.f, context.window->getSize().y / 2.f);
}

EndState::~EndState()
{
}

void EndState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.clear(sf::Color::Black);
	window.draw(mText);
}

bool EndState::update(sf::Time dt)
{
	return false;
}

bool EndState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
	}

	return false;
}

