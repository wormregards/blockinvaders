#include "../header/controls.h"

struct Controls controls;
struct Controls getControls() {
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
		controls.right = true;
	}
	
	if (IsKeyReleased(KEY_D) || IsKeyReleased(KEY_RIGHT)) {
		controls.right = false;
	}

	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
		controls.left = true;
	}

	if (IsKeyReleased(KEY_A) || IsKeyReleased(KEY_LEFT)) {
		controls.left = false;
	}
 
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
		controls.up = true;
	}

	if (IsKeyReleased(KEY_W) || IsKeyReleased(KEY_UP)) {
		controls.up = false;
	}

	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
		controls.down = true;
	}

	if (IsKeyReleased(KEY_S) || IsKeyReleased(KEY_DOWN)) {
		controls.down = false;
	}

	if (IsKeyDown(KEY_SPACE)) {
		controls.space = true;
	}
	if (IsKeyReleased(KEY_SPACE)) {
		controls.space = false;
	}

	return controls;
};

void resetControls() {
	controls.up = 0;
	controls.down = 0;
	controls.left = 0;
	controls.right = 0;
}