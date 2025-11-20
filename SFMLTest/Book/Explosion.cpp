#include <Book/Explosion.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Explosion::Explosion(sf::Vector2f position, const sf::Texture& texture)
	: mElapsedTime(sf::Time::Zero)
	, mDuration(sf::seconds(0.5f))
	, mCurrentFrame(0)
{
	mSprite.setTexture(texture);
	mSprite.setPosition(position);
	
	// Explosion sprite is 16x8 with 3 frames (8x8 each)
	mSprite.setTextureRect(sf::IntRect(0, 0, 8, 8));
	mSprite.setOrigin(4.f, 4.f);
}

void Explosion::update(sf::Time dt)
{
	mElapsedTime += dt;

	// Update frame based on elapsed time
	int frame = static_cast<int>((mElapsedTime.asSeconds() / mDuration.asSeconds()) * mFrameCount);
	if (frame >= mFrameCount)
		frame = mFrameCount - 1;

	if (frame != mCurrentFrame)
	{
		mCurrentFrame = frame;
		mSprite.setTextureRect(sf::IntRect(mCurrentFrame * 8, 0, 8, 8));
	}
}

bool Explosion::isFinished() const
{
	return mElapsedTime >= mDuration;
}

sf::Vector2f Explosion::getPosition() const
{
	return mSprite.getPosition();
}

void Explosion::setPosition(sf::Vector2f position)
{
	mSprite.setPosition(position);
}

void Explosion::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!isFinished())
		target.draw(mSprite, states);
}

