#include "LevelStateBase.h"
#include "DirectX.h"
#include "Enemy.h"

LevelStateBase::LevelStateBase(Player *player, state thisGameState) {
	background = NULL;
	this->music = NULL;
	this->player = player;
	this->thisGameState = thisGameState;

	char level[2];
	level[0] = (char)((int)thisGameState + 48);
	level[1] = (char)('\0');

	//resourceString
	strcpy_s(resourceFolder, "Resources\\Levels\\Level");
	strcat_s(resourceFolder, level);
	strcat_s(resourceFolder, "\\");
}

bool LevelStateBase::init(int width, int height) {
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

	// init enemy force
	strcpy_s(resourceResultString, resourceFolder);
	strcat_s(resourceResultString, "Force.txt");
	if (!init_enemyForce(resourceResultString)) {
		return false;
	}
	strcpy_s(resourceResultString, resourceFolder);
	strcat_s(resourceResultString, "EnemyExplosion\\");
	enemyExplosion = new AnimationFrames(resourceResultString, "EnemyExplosion.txt");
	
	// init music
	strcpy_s(resourceResultString, resourceFolder);
	strcat_s(resourceResultString, "Music.wav");
	music = LoadSound(resourceResultString);
	LoopSound(music);

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

bool LevelStateBase::init_enemyForce(char* filepath) {
	// Create enemy force from file
	vector<vector<string>> enemyForceAsId;

	string line;
	ifstream force(filepath);

	if (force.is_open()) {
		while (getline(force, line)) {
			istringstream iss(line);
			vector<string> tokens{ istream_iterator < string > {iss}, istream_iterator < string > {} };
			enemyForceAsId.push_back(tokens);
		}
		force.close();
	}
	else {
		return false;
	}

	numberOfEnemiesX = enemyForceAsId[0].size();	// A bit ugly, but it works
	numberOfEnemiesY = enemyForceAsId.size();

	enemies = new Enemy**[numberOfEnemiesY];
	for (int i = 0; i < numberOfEnemiesY; i++) {
		for (int j = 0; j < numberOfEnemiesX; j++) {
			enemies[i] = new Enemy*[numberOfEnemiesX];
		}
	}

	D3DXVECTOR2 position = D3DXVECTOR2(40, 40);
	bool updateSpacing = true;
	for (int i = 0; i < numberOfEnemiesY; i++) {
		for (int j = 0; j < numberOfEnemiesX; j++) {

			if(i != 0) {
				position.x = enemies[i - 1][j]->getPosition().x;
			}

			string id(enemyForceAsId[i][j]);
			if (id.compare("10") == 0) {
				enemies[i][j] = new Enemy_10(position);
			}
			else if (id.compare("11") == 0) {
				enemies[i][j] = new Enemy_11(position);
			}
			else if (id.compare("12") == 0) {
				enemies[i][j] = new Enemy_12(position);
			}
			else if (id.compare("20") == 0) {
				enemies[i][j] = new Enemy_20(position);
			}
			else if (id.compare("21") == 0) {
				enemies[i][j] = new Enemy_21(position);
			}
			else if (id.compare("30") == 0) {
				enemies[i][j] = new Enemy_30(position);
			}
			else if (id.compare("31") == 0) {
				enemies[i][j] = new Enemy_31(position);
			}
			else if (id.compare("32") == 0) {
				enemies[i][j] = new Enemy_32(position);
			}
			else if (id.compare("33") == 0) {
				enemies[i][j] = new Enemy_33(position);
			}
			else if (id.compare("34") == 0) {
				enemies[i][j] = new Enemy_34(position);
			}
			else if (id.compare("40") == 0) {
				enemies[i][j] = new Enemy_40(position);
			}
			else if (id.compare("41") == 0) {
				enemies[i][j] = new Enemy_41(position);
			}
			else {
				// Something went wrong when reading the file.
				return false;
			}
			
			if (i == 0) {
				position.x += enemies[i][j]->getSize().x * 1.2f;
			}			

			if (!enemies[i][j]->getTexture()) {
				return false;
			}
		}
		position.y += enemies[i][0]->getSize().y * 1.1f;
		position.x = 40;

	}

	return true;
}

void LevelStateBase::draw() {

	// Draw background
	DrawBackground(background);
	drawIntro();

	// Draw the gameplay
	if (pressedFire && intro->hasPlayedOnce()) {
		drawPlayer();
		drawEnemies();

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

void LevelStateBase::drawIntro() {
	// If intro has not yet been played, play it.
	if (!pressedFire || !intro->hasPlayedOnce()) {
		//draw intro
		D3DXVECTOR2 introPosition;
		introPosition.x = gameBoardWidth / 2 - intro->getSize().x / 2;
		introPosition.y = gameBoardHeight / 2 - intro->getSize().y / 2;
		DrawSprite(intro->getFrame(), introPosition, intro->getSize(), scaling);

		if (intro->hasPlayedOnce()) {
			introPosition.x = gameBoardWidth / 2 - pressFireToStart.size.x / 2;
			introPosition.y += pressFireToStart.size.x / 3;
			DrawSprite(pressFireToStart.texture, introPosition, pressFireToStart.size, scaling);
		}
	}
}

void LevelStateBase::drawPlayer() {
	if (player->isAlive()) {
		// Draw player texture
		DrawSprite(player->getTexture(), player->getPosition(), player->getSize(), scaling);
		if (player->getDisplayShield()) {	// Draw shield sprite

			float x = player->getPosition().x + player->getSize().x / 2;
			x = x - player->getShieldTexture().size.x / 2;
			float y = player->getPosition().y + player->getSize().y / 2;
			y = y - player->getShieldTexture().size.y / 2;

			DrawSprite(player->getShieldTexture().texture, D3DXVECTOR2(x, y), player->getShieldTexture().size, scaling);
		}
		if (!isVictory()) {
			// Draw hpBar.
			D3DXVECTOR2 barPosition(player->getPosition().x, player->getPosition().y + player->getSize().y);
			float width = ((float)player->getHealth() / (float)player->getMaxHealth() * player->getSize().x);
			DrawSprite(player->getHealthBarTexture().texture, barPosition, D3DXVECTOR2(width, 20), scaling);
			// Draw shield bar
			barPosition.y += 30;	// move down one row.
			width = (player->getShield() / player->getMaxShield() * player->getSize().x);
			DrawSprite(player->getShieldBarTexture().texture, barPosition, D3DXVECTOR2(width, 20), scaling);
		}
		// Draw afterburner if we are displaying victory state
		if (isVictory()) {
			D3DXVECTOR2 position(player->getPosition().x + player->getSize().x/2  - player->getAfterburnerTexture().size.x/2,
				player->getPosition().y + player->getSize().y*0.6f);
			DrawSprite(player->getAfterburnerTexture().texture, position, player->getAfterburnerTexture().size, scaling);
		}
	}
}

void LevelStateBase::drawEnemies() {
	for (int i = 0; i < numberOfEnemiesY; i++) {
		for (int j = 0; j < numberOfEnemiesX; j++) {
			if (enemies[i][j]->isAlive()) {
				// Draw enemy texture
				DrawSprite(enemies[i][j]->getTexture(), enemies[i][j]->getPosition(), enemies[i][j]->getSize(), scaling);
				// Draw enemy hp-bar
				D3DXVECTOR2 hpPosition(enemies[i][j]->getPosition().x, enemies[i][j]->getPosition().y + enemies[i][j]->getSize().y);
				float width = ((float)enemies[i][j]->getHealth() / (float)enemies[i][j]->getMaxHealth() * enemies[i][j]->getSize().x);
				DrawSprite(enemies[i][j]->getHealthBarTexture().texture, hpPosition, D3DXVECTOR2(width, 20), scaling);
			}
		}
	}
}

void LevelStateBase::update() {
	
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

		// Game Over && all explosions (including player) is played.
		if (isGameLost() && explosions.empty()) {
			setNewGameState(defeatState);
		}
	}
}

void LevelStateBase::updatePlayer() {
	// You should only be able to steer the player if he is alive.
	if (player->isAlive()) {
		player->update();
		// Steer player ship
		if (KeyDown(DIK_LEFT)) {
			player->goLeft();
		}
		if (KeyDown(DIK_RIGHT)) {
			player->goRight();
		}
		// if there is no steering of the ship.
		if (!KeyDown(DIK_LEFT) && !KeyDown(DIK_RIGHT)) {
			player->noSteering();
		}

		// Keep player within bounds
		if (player->getPosition().x < 0) {
			player->setPosition(D3DXVECTOR2(0, player->getPosition().y));
		}
		else if (player->getPosition().x > gameBoardWidth - player->getSize().x) {
			player->setPosition(D3DXVECTOR2(gameBoardWidth - player->getSize().x, player->getPosition().y));
		}

		if (KeyDown(DIK_SPACE)) {
			if (!pressedFire) {
				pressedFire = true;
			}
			if (player->getFireTimer()->getTime() > player->getRateOfFire()) {
				player->getMissileSound()->Stop();
				player->getMissileSound()->Play();
				D3DXVECTOR2 velocity(0, -15);
				D3DXVECTOR2 position(player->getPosition().x + player->getSize().x / 2 - player->getMissileTexture().size.x/2, player->getPosition().y);
				addProjectile(player->getMissileTexture(), position, velocity, true, player->getDamage());
				player->getFireTimer()->restart();
			}
		}
	}
}

void LevelStateBase::updateVictoryPose() {
	player->goUp();
	player->update();
	if (player->getPosition().y < -gameBoardHeight/2) {
		victoryPoseFinished = true;
	}
}

void LevelStateBase::updateEnemies() {
	// update enemies and check if it is time for enemies to change direction.
	bool changeDirection = false;
	for (int i = 0; i < numberOfEnemiesY; i++) {		// Update position
		for (int j = 0; j < numberOfEnemiesX; j++) {
			enemies[i][j]->update();
			if (enemies[i][j]->getPosition().x + enemies[i][j]->getSize().x > gameBoardWidth
				|| enemies[i][j]->getPosition().x < 0) {
				changeDirection = true;
			}
		}
	}
	if (changeDirection) {								// change direction
		for (int i = 0; i < numberOfEnemiesY; i++) {
			for (int j = 0; j < numberOfEnemiesX; j++) {
				enemies[i][j]->changeDirection(true, false);
			}
		}
	}

	for (int i = 0; i < numberOfEnemiesY; i++) {
		for (int j = 0; j < numberOfEnemiesX; j++) {
			if (enemies[i][j]->isAlive() && enemies[i][j]->fire()) {
				float xPos = enemies[i][j]->getPosition().x;
				float yPos = enemies[i][j]->getPosition().y;

				enemies[i][j]->getWeaponSound()->Play();
				D3DXVECTOR2 velocity(0, 15);
				D3DXVECTOR2 position(xPos + enemies[i][j]->getSize().x / 2 - enemies[i][j]->getWeaponTexture().size.x / 2,
					yPos + enemies[i][j]->getSize().y);
				addProjectile(enemies[i][j]->getWeaponTexture(), position, velocity, false, enemies[i][j]->getDamage());
			}
		}
	}
}

void LevelStateBase::updateDivEntities() {
	// update projectiles.
	for (vector<Entity*>::size_type i = 0; i < projectiles.size(); i++) {
		projectiles.at(i)->update();

		float yPos = projectiles.at(i)->getPosition().y;
		float buffert = gameBoardHeight * 1.1f;

		// if projectile is out of bounds
		if (yPos > gameBoardHeight + buffert || yPos < -buffert) {
			projectiles.at(i)->kill();
		}

		// if projectile is dead
		if (!projectiles.at(i)->isAlive()) {
			delete projectiles.at(i);
			projectiles.erase(projectiles.begin() + i);
		}
	}

	// remove explosions if they are finnished playing
	for (vector<Entity*>::size_type i = 0; i < explosions.size(); i++) {
		if (explosions[i]->hasPlayedOnce()) {
			delete explosions.at(i);
			explosions.erase(explosions.begin() + i);
		}
	}
}

void LevelStateBase::checkCollisions() {
	// Check collisions.
	// Not as horribly inefficient as it looks, it works as two nested loops irl. even if it looks like three
	// komplexity is: O((p*q)*n) where p*q = number of enemies and n = number of missiles.
	for (int i = 0; i < numberOfEnemiesY; i++) {
		for (int j = 0; j < numberOfEnemiesX; j++) {
			for (vector<Entity*>::size_type k = 0; k < projectiles.size(); k++) {
				// are our missiles and enemy colliding?
				if (projectiles[k]->isFriendly() && isColliding(enemies[i][j], projectiles.at(k)) && enemies[i][j]->isAlive()) {
					projectiles[k]->kill();
					enemies[i][j]->hit(player->getDamage());

					D3DXVECTOR2 position(projectiles[k]->getPosition().x + projectiles[k]->getSize().x/2 - player->getMissileExplosion()->getSize().x/2,
						projectiles[k]->getPosition().y - player->getMissileExplosion()->getSize().y / 2);
					explosions.push_back(new Animation(player->getMissileExplosion(), 60, false, position, scaling));

					// enemy got killed, display corpse explosion
					if (!enemies[i][j]->isAlive()) {
						position = D3DXVECTOR2(enemies[i][j]->getPosition().x + enemies[i][j]->getSize().x / 2 - enemyExplosion->getSize().x / 2,
							enemies[i][j]->getPosition().y + enemies[i][j]->getSize().y / 2 - enemyExplosion->getSize().y / 2);
						explosions.push_back(new Animation(enemyExplosion, 60, false, position, scaling));
					}
				}
			}
		}
	}
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
	}
}

void LevelStateBase::addProjectile(Texture texture, D3DXVECTOR2 position, D3DXVECTOR2 velocity, bool friendly, int damage) {
	projectiles.push_back(new Projectile(texture.texture, texture.size, position, damage, friendly, velocity.x, velocity.y, texture.rotation));
}

int LevelStateBase::isColliding(Entity *a, Entity *b) {
	RECT r1;
	r1.top = (long)a->getPosition().y;
	r1.bottom = (long)a->getPosition().y + (long)a->getSize().y;
	r1.left = (long)a->getPosition().x;
	r1.right = (long)a->getPosition().x + (long)a->getSize().x;

	RECT r2;
	r2.top = (long)b->getPosition().y;
	r2.bottom = (long)b->getPosition().y + (long)b->getSize().y;
	r2.left = (long)b->getPosition().x;
	r2.right = (long)b->getPosition().x + (long)b->getSize().x;

	RECT dest;

	return IntersectRect(&dest, &r1, &r2);
}

bool LevelStateBase::isVictory() {
	
	// Check victory condition
	bool victory = true;
	for (int i = 0; i < numberOfEnemiesY; i++) {
		for (int j = 0; j < numberOfEnemiesX; j++) {
			if (enemies[i][j]->isAlive()) {
				victory = false;
			}
		}
	}
	return victory;
}

bool LevelStateBase::isGameLost() {
	if (!player->isAlive()) {
		return true;
	}

	for (int i = 0; i < numberOfEnemiesY; i++) {
		for (int j = 0; j < numberOfEnemiesX; j++) {
			if (enemies[i][j]->isAlive()) {
				// check if a living enemie reaches bottom of screen
				if (enemies[i][j]->getPosition().y + enemies[i][j]->getSize().y >= gameBoardHeight)
					return true;
			}
		}
	}

	return false;
}

void LevelStateBase::release() {
	music->Stop();
	delete music;
	background->Release();
}
