#include "Level5State.h"

Level5State::Level5State(Player *player, state thisGameState) : LevelStateBase(player, thisGameState) {

}

bool Level5State::init(int width, int height) {
	gameBoardWidth = width;
	gameBoardHeight = height;
	victoryPoseFinished = false;
	pressedFire = false;

	// "init" background
	char resourceResultString[100];
	strcpy_s(resourceResultString, resourceFolder);
	strcat_s(resourceResultString, "Background.jpg");
	background = LoadSurface(resourceResultString);
	if (!background) {
		return false;
	}

	// Update and validate player
	D3DXVECTOR2 position = D3DXVECTOR2(gameBoardWidth / 2 - player->getSize().x / 2, gameBoardHeight - player->getSize().y - 100);	// Position player in the bottom middle part of screen
	D3DXVECTOR2 velocity(0, 0);
	player->setPosition(position);
	player->setVelocity(velocity);
	if (!player->getTexture()) {
		return false;
	}

	// init music
	strcpy_s(resourceResultString, resourceFolder);
	strcat_s(resourceResultString, "Music.wav");
	music = LoadSound(resourceResultString);
	LoopSound(music);
	
	//init exposions
	strcpy_s(resourceResultString, resourceFolder);
	strcat_s(resourceResultString, "SmallExplosion\\");
	smallExplosion = new AnimationFrames(resourceResultString, "EnemyExplosion.txt");

	strcpy_s(resourceResultString, resourceFolder);
	strcat_s(resourceResultString, "LargeExplosion\\");
	largeExplosion = new AnimationFrames(resourceResultString, "EnemyExplosion.txt");
	
	// Create boss
	boss = new Boss(D3DXVECTOR2((float)gameBoardWidth/2,40.0f));
	boss->setVelocity(D3DXVECTOR2(2, 0));

	// Init intro animation
	strcpy_s(resourceResultString, resourceFolder);
	strcat_s(resourceResultString, "Intro\\");
	intro = new Animation(resourceResultString, "Intro.txt", 60);
	strcpy_s(resourceResultString, resourceFolder);
	strcat_s(resourceResultString, "PressFireToStart.png");
	pressFireToStart.texture = LoadTexture(resourceResultString);
	pressFireToStart.size = GetBitmapSize(resourceResultString);

	return true;
}

void Level5State::update() {
	if (!pressedFire) {
		if (KeyDown(DIK_SPACE)) {
			pressedFire = true;
		}
	}

	// if intro has finished playnig and you have pressed fire once, start the game.
	if (pressedFire && intro->hasPlayedOnce()) {
		updatePlayer();
		updateBoss();
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

void Level5State::checkCollisions() {
	for (vector<Entity*>::size_type i = 0; i < projectiles.size(); i++) {
		// are enemies missile and player colliding?
		if (!projectiles.at(i)->isFriendly() && isColliding(player, projectiles.at(i)) && player->isAlive()) {
			player->hit(projectiles[i]->getDamage());
			projectiles.at(i)->kill();

			if (!player->isAlive()) {
				D3DXVECTOR2 position = D3DXVECTOR2(player->getPosition().x + player->getSize().x / 2 - player->getPlayerExplosion()->getSize().x / 2,
					player->getPosition().y + player->getSize().y / 2 - player->getPlayerExplosion()->getSize().y / 2);

				explosions.push_back(new Animation(player->getPlayerExplosion(), 60, false, position, scaling));
			}
		}
		// Check projectile hit against boss
		else if (projectiles.at(i)->isFriendly() && isColliding(boss, projectiles.at(i)) && boss->isAlive()) {
			if (!boss->isImmortal()) {
				boss->hit(projectiles[i]->getDamage());
			}
			projectiles.at(i)->kill();

			D3DXVECTOR2 position(projectiles[i]->getPosition().x + projectiles[i]->getSize().x / 2 - player->getMissileExplosion()->getSize().x / 2,
				projectiles[i]->getPosition().y - player->getMissileExplosion()->getSize().y / 2);
			explosions.push_back(new Animation(player->getMissileExplosion(), 60, false, position, scaling));

			if (!boss->isAlive()) {
				position = D3DXVECTOR2(boss->getPosition().x + boss->getSize().x / 2 - largeExplosion->getSize().x / 2,
					boss->getPosition().y + boss->getSize().y / 2 - largeExplosion->getSize().y / 2);

				explosions.push_back(new Animation(largeExplosion, 60, false, position, scaling));
			}
		}

		// Check collision against detached parts
		for (vector<Entity*>::size_type j = 0; j < bossParts.size(); j++) {
			if (projectiles.at(i)->isFriendly() && isColliding(bossParts[j], projectiles[i])) {
				bossParts[j]->hit(projectiles[i]->getDamage());
				projectiles.at(i)->kill();

				D3DXVECTOR2 position(projectiles[i]->getPosition().x + projectiles[i]->getSize().x / 2 - player->getMissileExplosion()->getSize().x / 2,
					projectiles[i]->getPosition().y - player->getMissileExplosion()->getSize().y / 2);
				explosions.push_back(new Animation(player->getMissileExplosion(), 60, false, position, scaling));

				
				// enemy got killed, display corpse explosion
				if (!bossParts[j]->isAlive()) {
					float explosionSizeX = boss->isAtStage2() ? smallExplosion->getSize().x : largeExplosion->getSize().x;
					float explosionSizeY = boss->isAtStage2() ? smallExplosion->getSize().y : largeExplosion->getSize().y;

					position = D3DXVECTOR2(bossParts[j]->getPosition().x + bossParts[j]->getSize().x / 2 - explosionSizeX / 2,
						bossParts[j]->getPosition().y + bossParts[j]->getSize().y / 2 - explosionSizeY / 2);

					if (boss->isAtStage2()) {
						explosions.push_back(new Animation(smallExplosion, 60, false, position, scaling));
					}
					else {
						explosions.push_back(new Animation(largeExplosion, 60, false, position, scaling));
					}
				}
			}
		}
	}
}

void Level5State::draw() {

	// Draw background
	DrawBackground(background);
	drawIntro();

	// Draw the gameplay
	if (pressedFire && intro->hasPlayedOnce()) {
		drawPlayer();
		drawBoss();

		// Draw explosions
		for (vector<Entity*>::size_type i = 0; i < explosions.size(); i++) {
			DrawSprite(explosions[i]->getFrame(), explosions[i]->getPosition(), explosions[i]->getSize(), scaling);
		}

		// Draw projectiles
		for (vector<Entity*>::size_type i = 0; i < projectiles.size(); i++) {
			DrawSprite(projectiles.at(i)->getTexture(), projectiles.at(i)->getPosition(), projectiles.at(i)->getSize(), scaling, projectiles.at(i)->getAngle());
		}
	}
}

void Level5State::drawBoss() {
	if (boss->isAlive()) {
		// Draw boss texture
		DrawSprite(boss->getTexture().texture, boss->getPosition(), boss->getTexture().size, scaling);
		// Draw boss hp bar
		D3DXVECTOR2 hpPosition(boss->getPosition().x, boss->getPosition().y + boss->getTexture().size.y);
		float width = ((float)boss->getHealth() / (float)boss->getMaxHealth() * boss->getTexture().size.x);
		DrawSprite(boss->getHealthBarTexture().texture, hpPosition, D3DXVECTOR2(width, 20), scaling);

		for (vector<Entity*>::size_type i = 0; i < bossParts.size(); i++) {
			DrawSprite(bossParts[i]->getTexture(), bossParts[i]->getPosition(), bossParts[i]->getSize(), scaling, bossParts[i]->getAngle());
			// Draw hp bar
			D3DXVECTOR2 hpPosition(bossParts[i]->getPosition().x, bossParts[i]->getPosition().y + bossParts[i]->getSize().y);
			float width = ((float)bossParts[i]->getHealth() / (float)bossParts[i]->getMaxHealth() * bossParts[i]->getSize().x);
			DrawSprite(boss->getHealthBarTexture().texture, hpPosition, D3DXVECTOR2(width, 20), scaling);
		}
	}
}

bool Level5State::isVictory() {
	if (!boss->isAlive()) {
		return true;
	}
	else {
		return false;
	}
}

void Level5State::updateBoss() {
	boss->update();

	if (boss->getPosition().x + boss->getSize().x > gameBoardWidth || boss->getPosition().x < 0) {
		boss->changeDirection(true, false);

		if (boss->isAtStage2()) {
			for (vector<BossPart*>::size_type i = 0; i < bossParts.size(); i++) {
				bossParts[i]->changeDirection(true, false);
			}
		}
	}

	// Boss firing
	if (boss->fireRightWing()) {
		D3DXVECTOR2 position(boss->getPosition().x + boss->getSize().x *  0.09f - boss->getWingWeapon().size.x / 2, boss->getPosition().y + boss->getSize().y * 0.66f);
		D3DXVECTOR2 velocity(0, 5);
		addProjectile(boss->getWingWeapon(), position, velocity, false, boss->getWingWeaponDamage());
	}
	if (boss->fireLeftWing()) {
		D3DXVECTOR2 position(boss->getPosition().x + boss->getSize().x *  0.91f - boss->getWingWeapon().size.x / 2, boss->getPosition().y + boss->getSize().y * 0.66f);
		D3DXVECTOR2 velocity(0, 5);
		addProjectile(boss->getWingWeapon(), position, velocity, false, boss->getWingWeaponDamage());
	}
	if (boss->fireMainWeapon()) {
		D3DXVECTOR2 position(boss->getPosition().x + boss->getSize().x / 2.0f - boss->getMainWeapon().size.x / 2, boss->getPosition().y + boss->getSize().y);
		D3DXVECTOR2 velocity(0, 5);
		addProjectile(boss->getMainWeapon(), position, velocity, false, boss->getMainWeaponDamage());
	}

	if (boss->spawnSpiders()) {
		// Epic fudgefactory down below!! Don't touch if you don't know that you are doing
		D3DXVECTOR2 spiderSize  = boss->getSpiderTexture().size;
		D3DXVECTOR2 bossSize    = boss->getTexture().size;
		D3DXVECTOR2 middleBack  = { boss->getPosition().x + bossSize.x /   2.0f - spiderSize.x / 2, boss->getPosition().y };
		D3DXVECTOR2 leftBack    = { boss->getPosition().x + bossSize.x * 0.185f - spiderSize.x / 2, boss->getPosition().y + boss->getSize().y * 0.05f };
		D3DXVECTOR2 leftMiddle  = { boss->getPosition().x + bossSize.x *  0.14f - spiderSize.x / 2, boss->getPosition().y + boss->getSize().y * 0.20f };
		D3DXVECTOR2 leftFront   = { boss->getPosition().x + bossSize.x *  0.09f - spiderSize.x / 2, boss->getPosition().y + boss->getSize().y * 0.4f };
		D3DXVECTOR2 rightBack   = { boss->getPosition().x + bossSize.x * 0.815f - spiderSize.x / 2, boss->getPosition().y + boss->getSize().y * 0.05f };
		D3DXVECTOR2 rightMiddle = { boss->getPosition().x + bossSize.x *  0.86f - spiderSize.x / 2, boss->getPosition().y + boss->getSize().y * 0.20f };
		D3DXVECTOR2 rightFront  = { boss->getPosition().x + bossSize.x *  0.91f - spiderSize.x / 2, boss->getPosition().y + boss->getSize().y * 0.4f };
		
		// Spawn spiders at given position (calculated above)
		float bossVelocityX = boss->getVelocity().x;
		bossParts.push_back(new BossPart(boss->getSpiderTexture().texture, boss->getSpiderTexture().size, middleBack,  5, false,  0.0f + bossVelocityX, 2.0f, 0, 0, boss->getSpiderWeaponRateOfFire(), boss->getSpiderWeaponDamage()));
		bossParts.push_back(new BossPart(boss->getSpiderTexture().texture, boss->getSpiderTexture().size, leftBack, 5, false, 0.2f + bossVelocityX, 1.8f, 0, 0, boss->getSpiderWeaponRateOfFire(), boss->getSpiderWeaponDamage()));
		bossParts.push_back(new BossPart(boss->getSpiderTexture().texture, boss->getSpiderTexture().size, leftMiddle, 5, false, 0.0f + bossVelocityX, 1.4f, 0, 0, boss->getSpiderWeaponRateOfFire(), boss->getSpiderWeaponDamage()));
		bossParts.push_back(new BossPart(boss->getSpiderTexture().texture, boss->getSpiderTexture().size, leftFront, 5, false, -0.2f + bossVelocityX, 0.9f, 0, 0, boss->getSpiderWeaponRateOfFire(), boss->getSpiderWeaponDamage()));
		bossParts.push_back(new BossPart(boss->getSpiderTexture().texture, boss->getSpiderTexture().size, rightBack, 5, false, -0.2f + bossVelocityX, 1.8f, 0, 0, boss->getSpiderWeaponRateOfFire(), boss->getSpiderWeaponDamage()));
		bossParts.push_back(new BossPart(boss->getSpiderTexture().texture, boss->getSpiderTexture().size, rightMiddle, 5, false, 0.0f + bossVelocityX, 1.4f, 0, 0, boss->getSpiderWeaponRateOfFire(), boss->getSpiderWeaponDamage()));
		bossParts.push_back(new BossPart(boss->getSpiderTexture().texture, boss->getSpiderTexture().size, rightFront, 5, false, 0.2f + bossVelocityX, 0.9f, 0, 0, boss->getSpiderWeaponRateOfFire(), boss->getSpiderWeaponDamage()));
		boss->resetSpawnParts();
		
		boss->setImmortality();
		
	}
	if (boss->spawnRightWing()) {
		float bossVelocityX = boss->getVelocity().x;
		D3DXVECTOR2 position(boss->getPosition().x - boss->getSize().x / 4.5f, boss->getPosition().y);
		bossParts.push_back(new BossPart(boss->getRightWingTexture().texture, boss->getRightWingTexture().size, position, 10, false, -0.5f + bossVelocityX, 0.125f, 0, -0.001f));
		boss->resetSpawnParts();
		boss->setImmortality();
	}
	if (boss->spawnLeftWing()) {
		float bossVelocityX = boss->getVelocity().x;
		D3DXVECTOR2 position(boss->getPosition().x + boss->getSize().x / 4.5f, boss->getPosition().y);
		bossParts.push_back(new BossPart(boss->getLeftWingTexture().texture, boss->getLeftWingTexture().size, position, 10, false, 0.5f + bossVelocityX, 0.125f, 0, 0.001f));
		boss->resetSpawnParts();
		boss->setImmortality();
	}

	// Update any eventual boss parts that has detached
	for (vector<BossPart*>::size_type i = 0; i < bossParts.size(); i++) {
		bossParts[i]->update();
		boss->setImmortality();
		
		if (boss->isAtStage2()){
			if (!boss->playSpiderAnimation()) {
				bossParts[i]->setVelocity(boss->getVelocity());
			}
			if (bossParts[i]->fire()) {
				D3DXVECTOR2 position(bossParts[i]->getPosition().x + bossParts[i]->getSize().x / 2, bossParts[i]->getPosition().y + bossParts[i]->getSize().y);
				D3DXVECTOR2 velocity(0, 5);
				addProjectile(boss->getSpiderWeapon(), position, velocity, false, bossParts[i]->getDamage());
			}
		}

		if (boss->isAtStage3() || boss->isAtStage4()) {
			if (bossParts[i]->getPosition().x + bossParts[i]->getSize().x > gameBoardWidth || bossParts[i]->getPosition().x < 0) {
				bossParts[i]->changeDirection(true, false);
			}
			if (bossParts[i]->getPosition().y + bossParts[i]->getSize().y > gameBoardWidth || bossParts[i]->getPosition().y < 0) {
				bossParts[i]->changeDirection(false, true);
			}
		}
			
		// Remove any parts that are dead.
		if (!bossParts[i]->isAlive()) {
			bossParts.erase(bossParts.begin() + i);
		}
	}

	if (bossParts.size() == 0 && boss->isImmortal()) {
		boss->setImmortality(false);
	}
}