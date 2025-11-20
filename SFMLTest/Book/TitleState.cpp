#include <Book/TitleState.hpp>
#include <Book/StateStack.hpp>
#include <Book/States.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, mShowText(true)
	, mTextEffectTime(sf::Time::Zero)
{
	if (!mFont.loadFromFile("Media/GIGI.ttf"))
	{
	}

	mText.setFont(mFont);
	mText.setString("Press any key to continue");
	mText.setCharacterSize(20);
	mText.setFillColor(sf::Color::Black);
	sf::FloatRect bounds = mText.getLocalBounds();
	mText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	mText.setPosition(context.window->getSize().x / 2.f, context.window->getSize().y / 2.f);
}

TitleState::~TitleState()
{
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.clear(sf::Color::White);

	if (mShowText)
		window.draw(mText);
}

bool TitleState::update(sf::Time dt)
{
	mTextEffectTime += dt;

	if (mTextEffectTime >= sf::seconds(0.5f))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}

	return true;
}

