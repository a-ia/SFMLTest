#include <Book/Enemy.hpp>
#include <Book/Projectile.hpp>
#include <Book/Player.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cmath>
#include <cstdlib>

const sf::Time Enemy::mShootInterval = sf::seconds(0.15f);
const float Enemy::mAgroRange = 200.f;
const float Enemy::mWanderRange = 50.f;

Enemy::Enemy(sf::Vector2f position, const sf::Texture& walkTexture, const sf::Texture& attackTexture, const sf::Texture& projectileTexture)
	: mWalkTexture(&walkTexture)
	, mAttackTexture(&attackTexture)
	, mProjectileTexture(&projectileTexture)
	, mCurrentAnimation(Walk)
	, mAnimationTime(sf::Time::Zero)
	, mCurrentFrame(0)
	, mPosition(position)
	, mSpawnPosition(position)
	, mVelocity(0.f, 0.f)
	, mSpeed(100.f) // Half of player speed
	, mIsMoving(false)
	, mIsAttacking(false)
	, mFacingRight(true)
	, mIsAgro(false)
	, mHealth(50)
	, mMaxHealth(50)
	, mAttackCooldown(sf::Time::Zero)
	, mTimeSinceLastShot(sf::Time::Zero)
	, mProjectilesShot(0)
	, mProjectileResetTime(sf::Time::Zero)
	, mLastProjectilePosition(0.f, 0.f)
{
	mSprite.setTexture(walkTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, 8, 8));
	mSprite.setOrigin(4.f, 4.f);
	mSprite.setPosition(position);
}

void Enemy::update(sf::Time dt, const Player& player)
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

	updateAI(dt, player);
	updateAnimation(dt);
}

void Enemy::updateAI(sf::Time dt, const Player& player)
{
	sf::Vector2f playerPos = player.getWorldPosition();
	sf::Vector2f toPlayer = playerPos - mPosition;
	float distanceToPlayer = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

	// Check if player is in agro range
	if (distanceToPlayer <= mAgroRange)
	{
		mIsAgro = true;
		
		// Move toward player
		if (distanceToPlayer > 20.f) // Don't move if too close
		{
			sf::Vector2f direction = toPlayer;
			float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
			if (length > 0.f)
			{
				direction /= length;
				mVelocity = direction;
				mPosition += mVelocity * mSpeed * dt.asSeconds();
				mIsMoving = true;
				
				// Face the player
				mFacingRight = (toPlayer.x > 0.f);
			}
		}
		else
		{
			mIsMoving = false;
		}

		// Attack player
		if (canShoot())
		{
			mIsAttacking = true;
			mCurrentAnimation = Attack;
			mAnimationTime = sf::Time::Zero;
			mCurrentFrame = 0;
		}
	}
	else
	{
		mIsAgro = false;
		
		// Wander in small range around spawn
		sf::Vector2f toSpawn = mSpawnPosition - mPosition;
		float distanceToSpawn = std::sqrt(toSpawn.x * toSpawn.x + toSpawn.y * toSpawn.y);
		
		if (distanceToSpawn > mWanderRange)
		{
			// Move back toward spawn
			sf::Vector2f direction = toSpawn;
			float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
			if (length > 0.f)
			{
				direction /= length;
				mVelocity = direction;
				mPosition += mVelocity * mSpeed * dt.asSeconds();
				mIsMoving = true;
				mFacingRight = (direction.x > 0.f);
			}
		}
		else
		{
			// Random wander
			static sf::Time wanderTime = sf::Time::Zero;
			wanderTime += dt;
			
			if (wanderTime >= sf::seconds(1.0f))
			{
				float angle = (rand() % 360) * 3.14159f / 180.f;
				mVelocity = sf::Vector2f(std::cos(angle), std::sin(angle));
				mFacingRight = (mVelocity.x > 0.f);
				wanderTime = sf::Time::Zero;
			}
			
			if (distanceToSpawn < mWanderRange)
			{
				mPosition += mVelocity * mSpeed * 0.5f * dt.asSeconds();
				mIsMoving = true;
			}
			else
			{
				mIsMoving = false;
			}
		}
	}

	mSprite.setPosition(mPosition);
}

void Enemy::updateAnimation(sf::Time dt)
{
	mAnimationTime += dt;

	if (mCurrentAnimation == Walk)
	{
		// Walk animation: 2 frames, 16x8 total
		sf::Time frameTime = sf::seconds(0.2f);
		int frame = static_cast<int>((mAnimationTime.asSeconds() / frameTime.asSeconds()));
		
		if (mIsMoving)
		{
			frame = frame % 2;
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

void Enemy::takeDamage(int damage)
{
	mHealth -= damage;
	if (mHealth < 0)
		mHealth = 0;
}

int Enemy::getHealth() const
{
	return mHealth;
}

bool Enemy::isAlive() const
{
	return mHealth > 0;
}

sf::Vector2f Enemy::getPosition() const
{
	return mPosition;
}

sf::FloatRect Enemy::getBoundingRect() const
{
	sf::FloatRect rect = mSprite.getGlobalBounds();
	rect.width = 8.f;
	rect.height = 8.f;
	return rect;
}

void Enemy::setPosition(sf::Vector2f position)
{
	mPosition = position;
	mSprite.setPosition(position);
}

bool Enemy::canShoot() const
{
	return mIsAgro && mTimeSinceLastShot >= mShootInterval && mProjectilesShot < mMaxProjectiles;
}

Projectile* Enemy::shoot(const Player& player)
{
	if (!canShoot())
		return nullptr;

	mProjectilesShot++;
	mTimeSinceLastShot = sf::Time::Zero;

	sf::Vector2f playerPos = player.getWorldPosition();
	sf::Vector2f direction = playerPos - mPosition;
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 0.f)
		direction /= length;

	sf::Vector2f shootPosition = mPosition;
	shootPosition += direction * 10.f; // Offset from enemy
	
	// Space projectiles by 4 pixels
	if (mProjectilesShot > 1)
	{
		shootPosition = mLastProjectilePosition + direction * 4.f;
	}
	mLastProjectilePosition = shootPosition;

	return new Projectile(Projectile::Enemy, shootPosition, direction, *mProjectileTexture);
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

