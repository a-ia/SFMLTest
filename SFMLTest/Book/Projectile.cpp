#include <Book/Projectile.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cmath>

Projectile::Projectile(Type type, sf::Vector2f position, sf::Vector2f direction, const sf::Texture& texture)
	: mType(type)
	, mDirection(direction)
	, mSpeed(300.f)
	, mActive(true)
{
	mSprite.setTexture(texture);
	mSprite.setPosition(position);
	
	// Projectile sprite is 2x1, so set texture rect
	mSprite.setTextureRect(sf::IntRect(0, 0, 2, 1));
	
	// Center the sprite
	mSprite.setOrigin(1.f, 0.5f);
}

void Projectile::update(sf::Time dt)
{
	if (!mActive)
		return;

	sf::Vector2f movement = mDirection * mSpeed * dt.asSeconds();
	mSprite.move(movement);

	// Deactivate if out of bounds (assuming 2000x2000 world)
	sf::Vector2f pos = mSprite.getPosition();
	if (pos.x < -100 || pos.x > 2100 || pos.y < -100 || pos.y > 2100)
	{
		mActive = false;
	}
}

bool Projectile::isActive() const
{
	return mActive;
}

sf::FloatRect Projectile::getBoundingRect() const
{
	return mSprite.getGlobalBounds();
}

sf::Vector2f Projectile::getPosition() const
{
	return mSprite.getPosition();
}

void Projectile::setPosition(sf::Vector2f position)
{
	mSprite.setPosition(position);
}

Projectile::Type Projectile::getType() const
{
	return mType;
}

void Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mActive)
		target.draw(mSprite, states);
}

