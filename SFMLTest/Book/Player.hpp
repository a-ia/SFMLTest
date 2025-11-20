#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

class Projectile;

class Player
{
public:
	Player(const sf::Texture& walkTexture, const sf::Texture& attackTexture, const sf::Texture& projectileTexture);

	void handleInput(sf::Keyboard::Key key, bool isPressed);
	void handleMouseInput(sf::Mouse::Button button, bool isPressed, sf::Vector2f mousePosition);
	void update(sf::Time dt);
	
	void takeDamage(int damage);
	int getHealth() const;
	bool isAlive() const;
	sf::Vector2f getPosition() const;
	sf::FloatRect getBoundingRect() const;
	
	void setPosition(sf::Vector2f position);
	sf::Vector2f getWorldPosition() const;
	void setWorldPosition(sf::Vector2f position);
	
	bool canShoot() const;
	Projectile* shoot();
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	void updateAnimation(sf::Time dt);
	void updateMovement(sf::Time dt);

private:
	enum Animation
	{
		Walk,
		Attack
	};

	sf::Sprite mSprite;
	const sf::Texture* mWalkTexture;
	const sf::Texture* mAttackTexture;
	const sf::Texture* mProjectileTexture;
	
	Animation mCurrentAnimation;
	sf::Time mAnimationTime;
	int mCurrentFrame;
	
	sf::Vector2f mVelocity;
	sf::Vector2f mWorldPosition;
	float mSpeed;
	
	bool mIsMoving;
	bool mIsAttacking;
	bool mFacingRight;
	
	int mHealth;
	int mMaxHealth;
	
	sf::Time mAttackCooldown;
	sf::Time mTimeSinceLastShot;
	static const sf::Time mShootInterval;
	static const int mMaxProjectiles = 5;
	int mProjectilesShot;
	sf::Time mProjectileResetTime;
	sf::Vector2f mLastProjectilePosition;
};

