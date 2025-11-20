#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>

class Explosion
{
public:
	Explosion(sf::Vector2f position, const sf::Texture& texture);

	void update(sf::Time dt);
	bool isFinished() const;
	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f position);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite mSprite;
	sf::Time mElapsedTime;
	sf::Time mDuration;
	int mCurrentFrame;
	static const int mFrameCount = 3;
};

