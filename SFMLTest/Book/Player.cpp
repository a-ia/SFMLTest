#include <Book/Player.hpp>
#include <Book/Projectile.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cmath>

const sf::Time Player::mShootInterval = sf::seconds(0.1f);

Player::Player(const sf::Texture& walkTexture, const sf::Texture& attackTexture, const sf::Texture& projectileTexture)
	: mWalkTexture(&walkTexture)
	, mAttackTexture(&attackTexture)
	, mProjectileTexture(&projectileTexture)
	, mCurrentAnimation(Walk)
	, mAnimationTime(sf::Time::Zero)
	, mCurrentFrame(0)
	, mVelocity(0.f, 0.f)
	, mWorldPosition(1000.f, 1000.f)
	, mSpeed(200.f)
	, mIsMoving(false)
	, mIsAttacking(false)
	, mFacingRight(true)
	, mHealth(100)
	, mMaxHealth(100)
	, mAttackCooldown(sf::Time::Zero)
	, mTimeSinceLastShot(sf::Time::Zero)
	, mProjectilesShot(0)
	, mProjectileResetTime(sf::Time::Zero)
	, mLastProjectilePosition(0.f, 0.f)
{
	mSprite.setTexture(walkTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, 8, 8));
	mSprite.setOrigin(4.f, 4.f);
}

void Player::handleInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::A)
	{
		if (isPressed)
		{
			mVelocity.x = -1.f;
			mFacingRight = false;
			mIsMoving = true;
		}
		else if (mVelocity.x < 0.f)
		{
			mVelocity.x = 0.f;
		}
	}
	else if (key == sf::Keyboard::D)
	{
		if (isPressed)
		{
			mVelocity.x = 1.f;
			mFacingRight = true;
			mIsMoving = true;
		}
		else if (mVelocity.x > 0.f)
		{
			mVelocity.x = 0.f;
		}
	}
	else if (key == sf::Keyboard::W)
	{
		if (isPressed)
		{
			mVelocity.y = -1.f;
			mIsMoving = true;
		}
		else if (mVelocity.y < 0.f)
		{
			mVelocity.y = 0.f;
		}
	}
	else if (key == sf::Keyboard::S)
	{
		if (isPressed)
		{
			mVelocity.y = 1.f;
			mIsMoving = true;
		}
		else if (mVelocity.y > 0.f)
		{
			mVelocity.y = 0.f;
		}
	}

	if (mVelocity.x == 0.f && mVelocity.y == 0.f)
		mIsMoving = false;
}

void Player::handleMouseInput(sf::Mouse::Button button, bool isPressed, sf::Vector2f mousePosition)
{
	if (button == sf::Mouse::Left && isPressed)
	{
		mIsAttacking = true;
		mCurrentAnimation = Attack;
		mAnimationTime = sf::Time::Zero;
		mCurrentFrame = 0;
	}
}

void Player::update(sf::Time dt)
{
	mTimeSinceLastShot += dt;
	mProjectileResetTime += dt;
	
	// Reset projectile count after a delay
	if (mProjectileResetTime >= sf::seconds(2.0f))
	{
		mProjectilesShot = 0;
		mProjectileResetTime = sf::Time::Zero;
		mLastProjectilePosition = sf::Vector2f(0.f, 0.f);
	}

	if (mIsAttacking)
	{
		mAttackCooldown += dt;
		if (mAttackCooldown >= sf::seconds(0.3f))
		{
			mIsAttacking = false;
			mAttackCooldown = sf::Time::Zero;
			mCurrentAnimation = Walk;
		}
	}

	updateAnimation(dt);
	updateMovement(dt);
}

void Player::updateAnimation(sf::Time dt)
{
	mAnimationTime += dt;

	if (mCurrentAnimation == Walk)
	{
		// Walk animation: 3 frames, 24x8 total
		sf::Time frameTime = sf::seconds(0.2f);
		int frame = static_cast<int>((mAnimationTime.asSeconds() / frameTime.asSeconds()));
		
		if (mIsMoving)
		{
			frame = frame % 3;
			mSprite.setTexture(*mWalkTexture);
			mSprite.setTextureRect(sf::IntRect(frame * 8, 0, 8, 8));
			
			if (!mFacingRight)
			{
				mSprite.setScale(-1.f, 1.f);
				mSprite.setOrigin(4.f, 4.f);
			}
			else
			{
				mSprite.setScale(1.f, 1.f);
				mSprite.setOrigin(4.f, 4.f);
			}
		}
		else
		{
			mSprite.setTexture(*mWalkTexture);
			mSprite.setTextureRect(sf::IntRect(0, 0, 8, 8));
		}
	}
	else if (mCurrentAnimation == Attack)
	{
		// Attack animation: 2 frames, 16x8 total
		sf::Time frameTime = sf::seconds(0.15f);
		int frame = static_cast<int>((mAnimationTime.asSeconds() / frameTime.asSeconds()));
		frame = std::min(frame, 1);
		
		mSprite.setTexture(*mAttackTexture);
		mSprite.setTextureRect(sf::IntRect(frame * 8, 0, 8, 8));
		
		if (!mFacingRight)
		{
			mSprite.setScale(-1.f, 1.f);
			mSprite.setOrigin(4.f, 4.f);
		}
		else
		{
			mSprite.setScale(1.f, 1.f);
			mSprite.setOrigin(4.f, 4.f);
		}
	}
}

void Player::updateMovement(sf::Time dt)
{
	if (mVelocity.x != 0.f || mVelocity.y != 0.f)
	{
		sf::Vector2f movement = mVelocity;
		float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
		if (length > 0.f)
		{
			movement /= length;
			movement *= mSpeed * dt.asSeconds();
			mWorldPosition += movement;
		}
	}
}

void Player::takeDamage(int damage)
{
	mHealth -= damage;
	if (mHealth < 0)
		mHealth = 0;
}

int Player::getHealth() const
{
	return mHealth;
}

bool Player::isAlive() const
{
	return mHealth > 0;
}

sf::Vector2f Player::getPosition() const
{
	// Return screen position (always center)
	return sf::Vector2f(320.f, 240.f);
}

sf::FloatRect Player::getBoundingRect() const
{
	sf::FloatRect rect = mSprite.getGlobalBounds();
	rect.left = 320.f - 4.f;
	rect.top = 240.f - 4.f;
	rect.width = 8.f;
	rect.height = 8.f;
	return rect;
}

void Player::setPosition(sf::Vector2f position)
{
	mSprite.setPosition(position);
}

sf::Vector2f Player::getWorldPosition() const
{
	return mWorldPosition;
}

void Player::setWorldPosition(sf::Vector2f position)
{
	mWorldPosition = position;
}

bool Player::canShoot() const
{
	return mIsAttacking && mTimeSinceLastShot >= mShootInterval && mProjectilesShot < mMaxProjectiles;
}

Projectile* Player::shoot()
{
	if (!canShoot())
		return nullptr;

	mProjectilesShot++;
	mTimeSinceLastShot = sf::Time::Zero;

	sf::Vector2f direction(1.f, 0.f);
	if (!mFacingRight)
		direction.x = -1.f;

	// Adjust direction based on vertical movement
	if (mVelocity.y < 0.f) // Moving up
		direction = sf::Vector2f(direction.x, -0.5f);
	else if (mVelocity.y > 0.f) // Moving down
		direction = sf::Vector2f(direction.x, 0.5f);

	// Normalize direction
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 0.f)
		direction /= length;

	sf::Vector2f shootPosition = getPosition();
	shootPosition += direction * 10.f; // Offset from player
	
	// Space projectiles by 4 pixels
	if (mProjectilesShot > 1)
	{
		shootPosition = mLastProjectilePosition + direction * 4.f;
	}
	mLastProjectilePosition = shootPosition;

	return new Projectile(Projectile::Player, shootPosition, direction, *mProjectileTexture);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

