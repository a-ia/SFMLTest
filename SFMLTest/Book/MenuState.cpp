#include <Book/MenuState.hpp>
#include <Book/StateStack.hpp>
#include <Book/States.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, mOptionIndex(0)
{
	// Load font
	if (!mFont.loadFromFile("Media/GIGI.ttf"))
	{
		// Fallback if font doesn't load
	}

	// Setup title
	mTitleText.setFont(mFont);
	mTitleText.setString("YOKAI BATTLE");
	mTitleText.setCharacterSize(48);
	mTitleText.setFillColor(sf::Color::Black);
	sf::FloatRect titleBounds = mTitleText.getLocalBounds();
	mTitleText.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
	mTitleText.setPosition(context.window->getSize().x / 2.f, 150.f);

	// Setup Enter option
	mEnterText.setFont(mFont);
	mEnterText.setString("Enter Game");
	mEnterText.setCharacterSize(30);
	mEnterText.setFillColor(sf::Color::Red);
	sf::FloatRect enterBounds = mEnterText.getLocalBounds();
	mEnterText.setOrigin(enterBounds.left + enterBounds.width / 2.f, enterBounds.top + enterBounds.height / 2.f);
	mEnterText.setPosition(context.window->getSize().x / 2.f, 300.f);

	// Setup Exit option
	mExitText.setFont(mFont);
	mExitText.setString("Exit");
	mExitText.setCharacterSize(30);
	mExitText.setFillColor(sf::Color::Black);
	sf::FloatRect exitBounds = mExitText.getLocalBounds();
	mExitText.setOrigin(exitBounds.left + exitBounds.width / 2.f, exitBounds.top + exitBounds.height / 2.f);
	mExitText.setPosition(context.window->getSize().x / 2.f, 350.f);
}

MenuState::~MenuState()
{
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.clear(sf::Color::White);

	window.draw(mTitleText);
	window.draw(mEnterText);
	window.draw(mExitText);
}

bool MenuState::update(sf::Time dt)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Up)
	{
		if (mOptionIndex > 0)
			mOptionIndex--;
		else
			mOptionIndex = Exit;
		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Down)
	{
		if (mOptionIndex < Exit)
			mOptionIndex++;
		else
			mOptionIndex = Enter;
		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Return)
	{
		if (mOptionIndex == Enter)
		{
			requestStackPop();
			requestStackPush(States::Game);
		}
		else if (mOptionIndex == Exit)
		{
			requestStackPop();
		}
	}

	return true;
}

void MenuState::updateOptionText()
{
	if (mOptionIndex == Enter)
	{
		mEnterText.setFillColor(sf::Color::Red);
		mExitText.setFillColor(sf::Color::Black);
	}
	else
	{
		mEnterText.setFillColor(sf::Color::Black);
		mExitText.setFillColor(sf::Color::Red);
	}
}
