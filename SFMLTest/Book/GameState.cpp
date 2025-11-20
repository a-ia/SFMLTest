#include <Book/GameState.hpp>
#include <Book/StateStack.hpp>
#include <Book/States.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <Book/Explosion.hpp>
#include <cmath>
#include <cstdlib>

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, mScreenRotation(0.f)
	, mRandomGenerator(std::random_device{}())
{
	// Load textures
	mTextures.load(Textures::PlayerWalk, "Media/Textures/sprites/players/vengeful_yokai/vengeful_yokai_walk.png");
	mTextures.load(Textures::PlayerAttack, "Media/Textures/sprites/players/vengeful_yokai/vengeful_yokai_attack.png");
	mTextures.load(Textures::PlayerProjectile, "Media/Textures/sprites/players/vengeful_yokai/vengeful_yokai_projectile.png");
	mTextures.load(Textures::EnemyWalk, "Media/Textures/sprites/players/evil_yokai/evil_yokai_walk.png");
	mTextures.load(Textures::EnemyAttack, "Media/Textures/sprites/players/evil_yokai/evil_yokai_attack.png");
	mTextures.load(Textures::EnemyProjectile, "Media/Textures/sprites/players/evil_yokai/evil_yokai_projectile.png");
	mTextures.load(Textures::ExplosionTexture, "Media/Textures/sprites/misc/explosion.png");

	// Load font
	   	{
		// Fallback if font doesn't load
	}

	// Create player
	mPlayer = std::unique_ptr<Player>(new Player(
		mTextures.get(Textures::PlayerWalk),
		mTextures.get(Textures::PlayerAttack),
		mTextures.get(Textures::PlayerProjectile)
	));
	mPlayer->setPosition(sf::Vector2f(320.f, 240.f)); // Center of screen

	// Setup views
	sf::Vector2f viewSize = sf::Vector2f(context.window->getSize());
	mWorldView.setSize(viewSize);
	mWorldView.setCenter(320.f, 240.f);
	mHudView.setSize(viewSize);
	mHudView.setCenter(viewSize / 2.f);

	// Setup HUD text
	mPlayerHealthText.setFont(mFont);
	mPlayerHealthText.setCharacterSize(20);
	mPlayerHealthText.setFillColor(sf::Color::Black);
	mPlayerHealthText.setPosition(10.f, 10.f);

	// Spawn initial enemies
	for (int i = 0; i < mMaxEnemies; ++i)
	{
		spawnEnemy();
	}
}

GameState::~GameState()
{
}

void GameState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	// Clear with white background
	window.clear(sf::Color::Black);

	// Apply screen rotation to world view
	sf::View rotatedView = mWorldView;
	rotatedView.setRotation(mScreenRotation);
	window.setView(rotatedView);

	// Draw player (always centered)
	mPlayer->draw(window, sf::RenderStates::Default);

	// Transform and draw enemies (world to screen)
	sf::Vector2f playerWorldPos = mPlayer->getWorldPosition();
	sf::Vector2f playerScreenPos = mPlayer->getPosition();
	sf::Vector2f screenCenter(320.f, 240.f);
	
	float angle = -mScreenRotation * 3.14159f / 180.f;
	float cosA = std::cos(angle);
	float sinA = std::sin(angle);

	for (const auto& enemy : mEnemies)
	{
		if (enemy->isAlive())
		{
			// Transform enemy world position to screen position
			sf::Vector2f enemyWorldPos = enemy->getPosition();
			sf::Vector2f worldOffset = enemyWorldPos - playerWorldPos;
			
			// Rotate offset
			sf::Vector2f rotatedOffset(
				worldOffset.x * cosA - worldOffset.y * sinA,
				worldOffset.x * sinA + worldOffset.y * cosA
			);
			
			sf::Vector2f enemyScreenPos = screenCenter + rotatedOffset;
			enemy->setPosition(enemyScreenPos);
			enemy->draw(window, sf::RenderStates::Default);
		}
	}

	// Store original positions for enemy projectiles and transform for drawing
	std::vector<sf::Vector2f> enemyProjectileWorldPositions;
	for (const auto& projectile : mProjectiles)
	{
		if (projectile->isActive())
		{
			if (projectile->getType() == Projectile::Enemy)
			{
				// Store world position
				enemyProjectileWorldPositions.push_back(projectile->getPosition());
				
				// Transform to screen for drawing
				sf::Vector2f projWorldPos = projectile->getPosition();
				sf::Vector2f worldOffset = projWorldPos - playerWorldPos;
				
				sf::Vector2f rotatedOffset(
					worldOffset.x * cosA - worldOffset.y * sinA,
					worldOffset.x * sinA + worldOffset.y * cosA
				);
				
				sf::Vector2f projScreenPos = screenCenter + rotatedOffset;
				projectile->setPosition(projScreenPos);
			}
			projectile->draw(window, sf::RenderStates::Default);
		}
	}
	
	// Restore enemy projectile world positions
	int enemyProjIndex = 0;
	for (auto& projectile : mProjectiles)
	{
		if (projectile->isActive() && projectile->getType() == Projectile::Enemy)
		{
			if (enemyProjIndex < enemyProjectileWorldPositions.size())
			{
				projectile->setPosition(enemyProjectileWorldPositions[enemyProjIndex]);
				enemyProjIndex++;
			}
		}
	}

	// Store and transform explosion positions
	std::vector<sf::Vector2f> explosionWorldPositions;
	for (const auto& explosion : mExplosions)
	{
		if (!explosion->isFinished())
		{
			// Store world position
			explosionWorldPositions.push_back(explosion->getPosition());
			
			// Explosions are in world space, transform to screen
			sf::Vector2f expWorldPos = explosion->getPosition();
			sf::Vector2f worldOffset = expWorldPos - playerWorldPos;
			
			sf::Vector2f rotatedOffset(
				worldOffset.x * cosA - worldOffset.y * sinA,
				worldOffset.x * sinA + worldOffset.y * cosA
			);
			
			sf::Vector2f expScreenPos = screenCenter + rotatedOffset;
			explosion->setPosition(expScreenPos);
			explosion->draw(window, sf::RenderStates::Default);
		}
	}
	
	// Restore explosion world positions
	int expIndex = 0;
	for (auto& explosion : mExplosions)
	{
		if (!explosion->isFinished())
		{
			if (expIndex < explosionWorldPositions.size())
			{
				explosion->setPosition(explosionWorldPositions[expIndex]);
				expIndex++;
			}
		}
	}

	// Draw HUD
	window.setView(mHudView);
	window.draw(mPlayerHealthText);

	// Draw enemy health
	float yOffset = 30.f;
	for (size_t i = 0; i < mEnemies.size(); ++i)
	{
		if (mEnemies[i]->isAlive())
		{
			sf::Text enemyHealth;
			enemyHealth.setFont(mFont);
			enemyHealth.setCharacterSize(16);
			enemyHealth.setFillColor(sf::Color::Red);
			enemyHealth.setString("Enemy " + std::to_string(i + 1) + ": " + std::to_string(mEnemies[i]->getHealth()));
			enemyHealth.setPosition(10.f, 10.f + yOffset);
			window.draw(enemyHealth);
			yOffset += 20.f;
		}
	}
}

bool GameState::update(sf::Time dt)
{
	// Update player
	mPlayer->update(dt);

	// Update enemies
	for (auto& enemy : mEnemies)
	{
		if (enemy->isAlive())
		{
			enemy->update(dt, *mPlayer);

			// Enemy shooting
			if (enemy->canShoot())
			{
				Projectile* proj = enemy->shoot(*mPlayer);
				if (proj)
				{
					mProjectiles.push_back(std::unique_ptr<Projectile>(proj));
				}
			}
		}
	}

	updateProjectiles(dt);
	updateExplosions(dt);
	checkCollisions();

	// Update HUD
	mPlayerHealthText.setString("Player Health: " + std::to_string(mPlayer->getHealth()));

	// Check if player is dead
	if (!mPlayer->isAlive())
	{
		requestStackPush(States::GameOver);
	}

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			requestStackPush(States::Pause);
		}
		else
		{
			mPlayer->handleInput(event.key.code, true);
			handleScreenRotation(event.key.code, true);
		}
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		mPlayer->handleInput(event.key.code, false);
		handleScreenRotation(event.key.code, false);
	}
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2f mousePos = sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
		mPlayer->handleMouseInput(event.mouseButton.button, true, mousePos);

		// Player shooting
		if (event.mouseButton.button == sf::Mouse::Left && mPlayer->canShoot())
		{
			Projectile* proj = mPlayer->shoot();
			if (proj)
			{
				mProjectiles.push_back(std::unique_ptr<Projectile>(proj));
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		sf::Vector2f mousePos = sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
		mPlayer->handleMouseInput(event.mouseButton.button, false, mousePos);
	}

	return true;
}

void GameState::updateProjectiles(sf::Time dt)
{
	for (auto it = mProjectiles.begin(); it != mProjectiles.end();)
	{
		(*it)->update(dt);
		if (!(*it)->isActive())
		{
			it = mProjectiles.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void GameState::updateExplosions(sf::Time dt)
{
	for (auto it = mExplosions.begin(); it != mExplosions.end();)
	{
		(*it)->update(dt);
		if ((*it)->isFinished())
		{
			it = mExplosions.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void GameState::checkCollisions()
{
	sf::Vector2f playerWorldPos = mPlayer->getWorldPosition();
	sf::Vector2f playerScreenPos = mPlayer->getPosition();
	sf::Vector2f screenCenter(320.f, 240.f);
	
	float angle = -mScreenRotation * 3.14159f / 180.f;
	float cosA = std::cos(angle);
	float sinA = std::sin(angle);

	// Check player projectiles vs enemies
	for (auto projIt = mProjectiles.begin(); projIt != mProjectiles.end();)
	{
		if ((*projIt)->getType() == Projectile::Player && (*projIt)->isActive())
		{
			bool hit = false;
			for (auto enemyIt = mEnemies.begin(); enemyIt != mEnemies.end(); ++enemyIt)
			{
				if ((*enemyIt)->isAlive())
				{
					// Transform enemy world position to screen for collision
					sf::Vector2f enemyWorldPos = (*enemyIt)->getPosition();
					sf::Vector2f worldOffset = enemyWorldPos - playerWorldPos;
					sf::Vector2f rotatedOffset(
						worldOffset.x * cosA - worldOffset.y * sinA,
						worldOffset.x * sinA + worldOffset.y * cosA
					);
					sf::Vector2f enemyScreenPos = screenCenter + rotatedOffset;
					
					// Check collision in screen space
					sf::FloatRect projRect = (*projIt)->getBoundingRect();
					sf::FloatRect enemyRect(enemyScreenPos.x - 4.f, enemyScreenPos.y - 4.f, 8.f, 8.f);
					
					if (projRect.intersects(enemyRect))
					{
						(*enemyIt)->takeDamage(10);
						hit = true;
						
						if (!(*enemyIt)->isAlive())
						{
							// Create explosion at enemy world position
							mExplosions.push_back(std::unique_ptr<Explosion>(new Explosion(enemyWorldPos, mTextures.get(Textures::ExplosionTexture))));
							
							// Spawn new enemy
							spawnEnemy();
							
							// Remove dead enemy
							enemyIt = mEnemies.erase(enemyIt);
						}
						break;
					}
				}
			}
			
			if (hit)
			{
				projIt = mProjectiles.erase(projIt);
			}
			else
			{
				++projIt;
			}
		}
		else
		{
			++projIt;
		}
	}

	// Check enemy projectiles vs player
	for (auto projIt = mProjectiles.begin(); projIt != mProjectiles.end();)
	{
		if ((*projIt)->getType() == Projectile::Enemy && (*projIt)->isActive())
		{
			// Enemy projectiles are in world space, need to check against player screen position
			// For simplicity, check if projectile is near player center in screen space
			sf::Vector2f projWorldPos = (*projIt)->getPosition();
			sf::Vector2f worldOffset = projWorldPos - playerWorldPos;
			sf::Vector2f rotatedOffset(
				worldOffset.x * cosA - worldOffset.y * sinA,
				worldOffset.x * sinA + worldOffset.y * cosA
			);
			sf::Vector2f projScreenPos = screenCenter + rotatedOffset;
			
			sf::FloatRect projRect(projScreenPos.x - 1.f, projScreenPos.y - 0.5f, 2.f, 1.f);
			if (projRect.intersects(mPlayer->getBoundingRect()))
			{
				mPlayer->takeDamage(10);
				projIt = mProjectiles.erase(projIt);
			}
			else
			{
				++projIt;
			}
		}
		else
		{
			++projIt;
		}
	}
}

void GameState::spawnEnemy()
{
	sf::Vector2f spawnPos = getRandomSpawnPosition();
	
	std::unique_ptr<Enemy> enemy(new Enemy(
		spawnPos,
		mTextures.get(Textures::EnemyWalk),
		mTextures.get(Textures::EnemyAttack),
		mTextures.get(Textures::EnemyProjectile)
	));
	
	mEnemies.push_back(std::move(enemy));
}

sf::Vector2f GameState::getRandomSpawnPosition()
{
	std::uniform_real_distribution<float> xDist(50.f, 590.f);
	std::uniform_real_distribution<float> yDist(50.f, 430.f);
	
	sf::Vector2f pos;
	do
	{
		pos = sf::Vector2f(xDist(mRandomGenerator), yDist(mRandomGenerator));
	} while (!isValidSpawnPosition(pos));
	
	return pos;
}

bool GameState::isValidSpawnPosition(sf::Vector2f position)
{
	// Don't spawn near player (center of screen)
	sf::Vector2f playerPos(320.f, 240.f);
	float distance = std::sqrt(
		(position.x - playerPos.x) * (position.x - playerPos.x) +
		(position.y - playerPos.y) * (position.y - playerPos.y)
	);
	
	if (distance < 150.f) // Minimum distance from player
		return false;
	
	// Don't spawn too close to other enemies
	for (const auto& enemy : mEnemies)
	{
		if (enemy->isAlive())
		{
			sf::Vector2f enemyPos = enemy->getPosition();
			float enemyDistance = std::sqrt(
				(position.x - enemyPos.x) * (position.x - enemyPos.x) +
				(position.y - enemyPos.y) * (position.y - enemyPos.y)
			);
			
			if (enemyDistance < 80.f)
				return false;
		}
	}
	
	return true;
}

void GameState::handleScreenRotation(sf::Keyboard::Key key, bool isPressed)
{
	if (isPressed)
	{
		if (key == sf::Keyboard::LShift)
		{
			mScreenRotation -= 90.f;
			if (mScreenRotation < 0.f)
				mScreenRotation += 360.f;
		}
		else if (key == sf::Keyboard::Space)
		{
			mScreenRotation += 90.f;
			if (mScreenRotation >= 360.f)
				mScreenRotation -= 360.f;
		}
	}
}



