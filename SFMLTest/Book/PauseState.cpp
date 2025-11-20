#include <Book/PauseState.hpp>
#include <Book/StateStack.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
{
	if (!mFont.loadFromFile("Media/GIGI.ttf"))
	{
	}

	mPausedText.setFont(mFont);
	mPausedText.setString("PAUSED");
	mPausedText.setCharacterSize(48);
	mPausedText.setFillColor(sf::Color::Black);
	sf::FloatRect bounds = mPausedText.getLocalBounds();
	mPausedText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	mPausedText.setPosition(context.window->getSize().x / 2.f, context.window->getSize().y / 2.f - 50.f);

	mInstructionText.setFont(mFont);
	mInstructionText.setString("Press Escape to resume");
	mInstructionText.setCharacterSize(20);
	mInstructionText.setFillColor(sf::Color::Black);
	sf::FloatRect instBounds = mInstructionText.getLocalBounds();
	mInstructionText.setOrigin(instBounds.left + instBounds.width / 2.f, instBounds.top + instBounds.height / 2.f);
	mInstructionText.setPosition(context.window->getSize().x / 2.f, context.window->getSize().y / 2.f + 50.f);
}

PauseState::~PauseState()
{
}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mPausedText);
	window.draw(mInstructionText);
}

bool PauseState::update(sf::Time dt)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		requestStackPop();
	}

	return false;
}
