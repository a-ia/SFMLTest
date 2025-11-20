#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class Projectile;
class Player;

class Enemy
{
public:
	Enemy(sf::Vector2f position, const sf::Texture& walkTexture, const sf::Texture& attackTexture, const sf::Texture& projectileTexture);

	void update(sf::Time dt, const Player& player);
	
	void takeDamage(int damage);
	int getHealth() const;
	bool isAlive() const;
	sf::Vector2f getPosition() const;
	sf::FloatRect getBoundingRect() const;
	
	void setPosition(sf::Vector2f position);
	
	bool canShoot() const;
	Projectile* shoot(const Player& player);
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	void updateAnimation(sf::Time dt);
	void updateAI(sf::Time dt, const Player& player);

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
	
	sf::Vector2f mPosition;
	sf::Vector2f mSpawnPosition;
	sf::Vector2f mVelocity;
	float mSpeed;
	
	bool mIsMoving;
	bool mIsAttacking;
	bool mFacingRight;
	bool mIsAgro;
	
	int mHealth;
	int mMaxHealth;
	
	sf::Time mAttackCooldown;
	sf::Time mTimeSinceLastShot;
	static const sf::Time mShootInterval;
	static const float mAgroRange;
	static const float mWanderRange;
	static const int mMaxProjectiles = 3;
	int mProjectilesShot;
	sf::Time mProjectileResetTime;
	sf::Vector2f mLastProjectilePosition;
};

