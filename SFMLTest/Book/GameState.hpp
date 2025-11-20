#pragma once

#include <memory>
#include <vector>
#include <random>

#include <Book/State.hpp>
#include <Book/Player.hpp>
#include <Book/Enemy.hpp>
#include <Book/Projectile.hpp>
#include <Book/Explosion.hpp>
#include <Book/ResourceHolder.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <Book/Explosion.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>

class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);
	virtual ~GameState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	void updateProjectiles(sf::Time dt);
	void updateExplosions(sf::Time dt);
	void checkCollisions();
	void spawnEnemy();
	sf::Vector2f getRandomSpawnPosition();
	bool isValidSpawnPosition(sf::Vector2f position);
	void handleScreenRotation(sf::Keyboard::Key key, bool isPressed);

private:
	enum Textures
	{
		PlayerWalk,
		PlayerAttack,
		PlayerProjectile,
		EnemyWalk,
		EnemyAttack,
		EnemyProjectile,
		ExplosionTexture
	};

	ResourceHolder<sf::Texture, Textures> mTextures;
	
	std::unique_ptr<Player> mPlayer;
	std::vector<std::unique_ptr<Enemy>> mEnemies;
	std::vector<std::unique_ptr<Projectile>> mProjectiles;
	std::vector<std::unique_ptr<Explosion>> mExplosions;
	
	sf::View mWorldView;
	sf::View mHudView;
	float mScreenRotation;
	
	sf::Text mPlayerHealthText;
	sf::Font mFont;
	
	static const int mMaxEnemies = 5;
	std::mt19937 mRandomGenerator;
};

