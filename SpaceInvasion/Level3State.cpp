#include "Level3State.h"

Level3State::Level3State(Player *player, state thisGameState) : LevelStateBase(player, thisGameState) {
	invulnerabilityTime = 0.3f;
	hitTimer.restart();
}

bool Level3State::init(int width, int height) {
	cometExplosionFrames = new AnimationFrames("Resources\\Enemies\\AsteroidExplosion\\", "Asteroid_Explosion.txt");
	bool response = LevelStateBase::init(width, height);
	return response;
}

void Level3State::update() {
	if (!pressedFire) {
		if (KeyDown(DIK_SPACE)) {
			pressedFire = true;
		}
	}

	// if intro has finished playnig and you have pressed fire once, start the game.
	if (pressedFire && intro->hasPlayedOnce()) {
		updatePlayer();
		updateEnemies();
		updateDivEntities();
		checkCollisions();

		// Victory
		if (isVictory()) {
			if (!victoryPoseFinished) {
				updateVictoryPose();
			}
			else {
				setNewGameState((state)((int)thisGameState + 1));
			}
		}

		// Game Over
		if (isGameLost() && explosions.empty()) {
			setNewGameState(defeatState);
		}
	}
}

void Level3State::updateEnemies() {
	// update enemies and check if it is time for enemies to change direction.
	for (int i = 0; i < numberOfEnemiesY; i++) {		// Update position
		for (int j = 0; j < numberOfEnemiesX; j++) {
			enemies[i][j]->update();
			// Change direction
			if (enemies[i][j]->getPosition().x + enemies[i][j]->getSize().x > gameBoardWidth
				|| enemies[i][j]->getPosition().x < 0) {
				enemies[i][j]->changeDirection(true, false);
			}
			if (enemies[i][j]->getPosition().y + enemies[i][j]->getSize().y > gameBoardHeight
				|| enemies[i][j]->getPosition().y < 0) {
				enemies[i][j]->changeDirection(false, true);
			}
		}
	}
}

void Level3State::checkCollisions() {
	
	//TODO: should we move enemies that are crashed into by player?

	// Check collisions.
	// Not as horribly inefficient as it looks, it works as two nested loops irl. even if it looks like three
	// komplexity is: O((p*q)*n) where p*q = number of enemies and n = number of missiles.
	for (int i = 0; i < numberOfEnemiesY; i++) {
		for (int j = 0; j < numberOfEnemiesX; j++) {
			for (vector<Entity*>::size_type k = 0; k < projectiles.size(); k++) {
				// are our missiles and enemy colliding?
				if (isColliding(enemies[i][j], projectiles.at(k)) && enemies[i][j]->isAlive()) {
					projectiles[k]->kill();
					enemies[i][j]->hit(player->getDamage());
					D3DXVECTOR2 position(projectiles[k]->getPosition().x + projectiles[k]->getSize().x / 2 - player->getMissileExplosion()->getSize().x / 2,
						projectiles[k]->getPosition().y - player->getMissileExplosion()->getSize().y / 2);

					explosions.push_back(new Animation(player->getMissileExplosion(), 60, false, position, scaling));

					if (!enemies[i][j]->isAlive()) {	// If enemy has died, display new explosion animation
						// Calculate exactly where to put animation
						D3DXVECTOR2 position(
							enemies[i][j]->getPosition().x + enemies[i][j]->getSize().x / 2 - cometExplosionFrames->getSize().x / 2,
							enemies[i][j]->getPosition().y + enemies[i][j]->getSize().y / 2 - cometExplosionFrames->getSize().x / 2);
						float scale = (float)((enemies[i][j]->getSize().x < 300) ? 1 : 2);		// if it is a large comet, double the size of the debries
						explosions.push_back(new Animation(cometExplosionFrames, 60, false, position, scale));
					}
				}
			}
			// are enemy colliding with us?
			if (isColliding(enemies[i][j], player) && enemies[i][j]->isAlive()) {
				D3DXVECTOR2 playerVelocity = player->getVelocity();	// where to move the enemie if we are pushing on him
				if (playerVelocity.x < 0) {
					D3DXVECTOR2 newPosition(player->getPosition().x - (enemies[i][j]->getSize().x + 5), enemies[i][j]->getPosition().y);
					if (newPosition.x < 0) {
						newPosition.x = 5;
					}
					enemies[i][j]->setPosition(newPosition);
					enemies[i][j]->setDirection(-1);				// set the correct x-direction
					enemies[i][j]->setVelocity(D3DXVECTOR2(playerVelocity.x, enemies[i][j]->getVelocity().y));	// update enemy with new velocity
				}
				// Going right
				else if (playerVelocity.x > 0) {
					D3DXVECTOR2 newPosition(player->getPosition().x + (player->getSize().x + 5), enemies[i][j]->getPosition().y);
					if (newPosition.x >= (gameBoardWidth - enemies[i][j]->getSize().x)) {
						newPosition.x = (gameBoardWidth - enemies[i][j]->getSize().x) - 5;
					}
					enemies[i][j]->setPosition(newPosition);
					enemies[i][j]->setDirection(1);					// set the correct x-direction
					enemies[i][j]->setVelocity(D3DXVECTOR2(playerVelocity.x, enemies[i][j]->getVelocity().y));	// update enemy with new velocity
				}
				// Player standing still
				else {
					enemies[i][j]->changeDirection(true, false);	// change y-direction on enemy
				}
				enemies[i][j]->changeDirection(false, true);	// change y-direction on enemy

				// Reset hit timer on player
				if (hitTimer.getTime() > invulnerabilityTime) {
					player->hit(1);
					hitTimer.restart();
				}
			}
		}
	}
}

bool Level3State::isGameLost() {
	return (!player->isAlive());
}