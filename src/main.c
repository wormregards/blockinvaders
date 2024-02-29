#include "../header/controls.h"
#include "../assets/playerSprite.h"

void handlePlayerMovement(float speed, float deltaTime, Rectangle *rectangle) {
	if (getControls().left) {
		rectangle->x -= speed * deltaTime;
	}

	if (getControls().right) {
		rectangle->x += speed * deltaTime;
	}

}

int main() {
	int screenWidth = 600;
	int screenHeight = 700;
	int targetFPS = 60;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Block Invaders");
	SetTargetFPS(targetFPS);

	Rectangle map = { screenWidth / 2, screenHeight - 100, screenWidth, screenHeight };
	float speed = 500.f;
	float deltaTime = GetFrameTime();

	Camera2D camera = { 0 };
	Vector2 camPosition = { 20 , 20 };
	camera.target = camPosition;
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	camera.target.x = map.x;
	camera.target.y = map.y;
	camera.offset = (Vector2){ GetScreenWidth() / 2 - (map.width / 2), GetScreenHeight() / 2 - (map.height / 2) };

	Rectangle player = { 0, 0, PLAYERSPRITE_WIDTH, PLAYERSPRITE_HEIGHT};
	player.x = map.x;
	player.y = map.y + (map.height - player.height);

	Rectangle rectangles[100] = { 0 };
	int rectangleCount = 0;

	Rectangle enemies[44];
	int enemyCount = 0;
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 4; y++) {
			enemies[enemyCount].width = 30;
			enemies[enemyCount].height = 30;
			enemies[enemyCount].x = map.x + ((enemies[enemyCount].width + 6) * x);
			enemies[enemyCount].y = 100 + map.y + ((enemies[enemyCount].height + 6) * y);

			enemyCount++;
		}
	}

	bool collision = false;

	Image playerImage = {
	   .data = PLAYERSPRITE_DATA,
	   .width = PLAYERSPRITE_WIDTH,
	   .height = PLAYERSPRITE_HEIGHT,
	   .format = PLAYERSPRITE_FORMAT,
	   .mipmaps = 1
	};
	
	Texture2D playerSprite = LoadTextureFromImage(playerImage);


	Rectangle enemyContainer = { 0 , 0, 390, 100 };
	enemyContainer.x = map.x;
	enemyContainer.y = map.y;
	int containerSpeed = 100;
	int jumpDistance = 20;
	int totalEnemies = 40;

	bool isLose = false;

	Rectangle bottomBounds = { 0, 0,GetScreenWidth(), 100};
	bottomBounds.x = map.x;
	bottomBounds.y = map.y + map.height - (player.height - 10);
	bottomBounds.width = map.width;

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground((Color) { 0x17, 0x17, 0x17, 0xFF });
		deltaTime = GetFrameTime();
		if (IsKeyPressed(KEY_SPACE)) {
			// Add a new rectangle to the array when space key is pressed
			if (rectangleCount < 100) {
				rectangles[rectangleCount].x = player.x + player.width / 2;
				rectangles[rectangleCount].y = player.y;
				rectangles[rectangleCount].height = 30;
				rectangles[rectangleCount].width = 10;
				rectangleCount++;
			}

		}
		handlePlayerMovement(speed, deltaTime, &player);
		if (GetScreenWidth() != screenWidth || GetScreenHeight() != screenHeight) {
			// screen resize event
			screenWidth = GetScreenWidth();
			screenHeight = GetScreenHeight();
			map.height = screenHeight;
			map.y = screenHeight - 100;

			player.y = map.y + (map.height - player.height);
			enemyCount = 0;
			enemyContainer.x = map.x;
			enemyContainer.y = map.y;
			for (int x = 0; x < 11; x++)
			{
				for (int y = 0; y < 4; y++) {
 
					enemies[enemyCount].x = map.x + ((enemies[enemyCount].width + 6) * x);
					enemies[enemyCount].y = 100 + map.y + ((enemies[enemyCount].height + 6) * y);

					enemyCount++;
				}
			}
 
			bottomBounds.x = map.x;
			bottomBounds.y = map.y + map.height - (player.height - 10);
			bottomBounds.width = map.width;

			camera.target.x = map.x;
			camera.target.y = map.y;
			camera.offset = (Vector2){ screenWidth / 2 - (map.width / 2), screenHeight / 2 - (map.height / 2) };
		}

		// handle container
		enemyContainer.x += containerSpeed * deltaTime;
		if (enemyContainer.x + enemyContainer.width > map.x + map.width) {
			containerSpeed = -100;
			enemyContainer.y += jumpDistance;
		}

		if (enemyContainer.x <= map.x) {
			containerSpeed = 100;
			enemyContainer.y += jumpDistance;
		}
		if (isLose == false) {
			BeginMode2D(camera);
			for (int i = 0; i < 44; i++) {
				enemies[i].x += containerSpeed * deltaTime;
				if (enemyContainer.x + enemyContainer.width > map.x + map.width) {
					enemies[i].y += jumpDistance;
				}

				if (enemyContainer.x <= map.x) {
					enemies[i].y += jumpDistance;
				}

				if (CheckCollisionRecs(bottomBounds, enemies[i])) {
					isLose = true;
				};
				DrawRectangleRec(enemies[i], WHITE);
			}
			DrawTextureRec(playerSprite, (Rectangle){0, 0, PLAYERSPRITE_WIDTH, PLAYERSPRITE_HEIGHT}, (Vector2) { player.x, player.y }, WHITE);
		
			for (int i = 0; i < 100; i++) {
			
				rectangles[i].y -= 600 * deltaTime;
				DrawRectangleRec(rectangles[i], ORANGE);
			

				for (int a = 0; a < 44; a++) {
					collision = CheckCollisionRecs(rectangles[i], enemies[a]);

					if (collision) {
						// Move all subsequent rectangles back by one position
						totalEnemies--;
						// delete enemy
						for (int b = a; b < 43; b++) {
							enemies[b] = enemies[b + 1];
						
	;					}
					
						// delete bullet
						for (int j = i; j < 99; j++) {
							rectangles[j] = rectangles[j + 1];
						}
						rectangleCount--;
						break;
					}

					if (rectangles[i].y < map.y && rectangles[i].y > 0) {
						for (int j = i; j < 99; j++) {
							rectangles[j] = rectangles[j + 1];
						}
						rectangleCount--;
						break;
					}
				}
			
			}
			EndMode2D();
		}

		if (totalEnemies <= 0) {
			DrawText("You Win!", 100, 100, 20, WHITE);
		}

		if (isLose) {
			DrawText("You Lose!", 100, 100, 20, WHITE);
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

