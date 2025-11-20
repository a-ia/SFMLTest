#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class Projectile
{
public:
	enum Type
	{
		Player,
		Enemy
	};

public:
	Projectile(Type type, sf::Vector2f position, sf::Vector2f direction, const sf::Texture& texture);

	void update(sf::Time dt);
	bool isActive() const;
	sf::FloatRect getBoundingRect() const;
	sf::Vector2f getPosition() const;
	Type getType() const;
	void setPosition(sf::Vector2f position);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Type mType;
	sf::Sprite mSprite;
	sf::Vector2f mDirection;
	float mSpeed;
	bool mActive;
};

