#pragma once
#include "raylib.h"
#ifndef CONTROLS_H
#define CONTROLS_H

struct Controls {
	bool up;
	bool left;
	bool right;
	bool down;
	bool space;
};

extern struct Controls controls;
struct Controls getControls();
void resetControls();
#endif
