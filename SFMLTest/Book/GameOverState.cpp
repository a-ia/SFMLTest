#include <Book/GameOverState.hpp>
#include <Book/StateStack.hpp>
#include <Book/States.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

GameOverState::GameOverState(StateStack& stack, Context context)
	: State(stack, context)
{
	if (!mFont.loadFromFile("Media/GIGI.ttf"))
	{
	}

	mGameOverText.setFont(mFont);
	mGameOverText.setString("GAME OVER");
	mGameOverText.setCharacterSize(48);
	mGameOverText.setFillColor(sf::Color::Red);
	sf::FloatRect bounds = mGameOverText.getLocalBounds();
	mGameOverText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	mGameOverText.setPosition(context.window->getSize().x / 2.f, context.window->getSize().y / 2.f - 50.f);

	mInstructionText.setFont(mFont);
	mInstructionText.setString("Press Enter to return to menu");
	mInstructionText.setCharacterSize(20);
	mInstructionText.setFillColor(sf::Color::Black);
	sf::FloatRect instBounds = mInstructionText.getLocalBounds();
	mInstructionText.setOrigin(instBounds.left + instBounds.width / 2.f, instBounds.top + instBounds.height / 2.f);
	mInstructionText.setPosition(context.window->getSize().x / 2.f, context.window->getSize().y / 2.f + 50.f);
}

GameOverState::~GameOverState()
{
}

void GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.clear(sf::Color::Black);
	window.draw(mGameOverText);
	window.draw(mInstructionText);
}

bool GameOverState::update(sf::Time dt)
{
	return false;
}

bool GameOverState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
	{
		requestStateClear();
		requestStackPush(States::Menu);
	}

	return false;
}
