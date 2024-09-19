/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <complex.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <raylib.h>

#define POINTS 500

#define TRIES 256

#define ARGB(a, r, g, b) ( \
		((a) & 0xff) << (8 * 3) | \
		((b) & 0xff) << (8 * 2) | \
		((g) & 0xff) << (8 * 1) | \
		((r) & 0xff) << (8 * 0))

static uint32_t fb[POINTS * POINTS];

double sig(double x) {
	return 1 / (1 + exp(-x));
}

uint64_t test(double complex c) {
	double complex z = 0;
	uint64_t i;
	for (i = 0; i < TRIES; i++) {
		if (cabs(z) == INFINITY)
			break;
		z = z*z + c;
	}

	return i;
}

struct Vector2 screen_to_cartesian(const struct Vector2 point,
				   const struct Rectangle cartesian_drawing_area) {
	struct Vector2 ret = {
		.x = point.x * cartesian_drawing_area.width / POINTS + cartesian_drawing_area.x,
		.y = point.y * cartesian_drawing_area.height / POINTS + cartesian_drawing_area.y,
	};

	ret.y *= -1;

	return ret;
}

void draw_mandel(struct Rectangle rec) {
	for(size_t y = 0; y < POINTS; y++) {
		for(size_t x = 0; x < POINTS; x++) {
			struct Vector2 coords = screen_to_cartesian((struct Vector2){x, y}, rec);

			uint8_t tries = test(coords.x + coords.y * I);

			fb[x + y * POINTS] = ARGB(0xff, tries, tries, tries);
		}
	}
}

int main() {
	InitWindow(POINTS, POINTS, "Mandelbrot set");

	draw_mandel((struct Rectangle) {
		.x = -1.5,
		.y = -1,
		.width = 2,
		.height = 2,
	});

	struct Image img = {
		.data = &fb,
		.width = POINTS,
		.height = POINTS,
		.mipmaps = 1,
		.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
	};

	struct Texture text = LoadTextureFromImage(img);

	while(!WindowShouldClose()) {
		BeginDrawing();
		{
			DrawTexture(text, 0, 0, WHITE);
		}
		EndDrawing();
	}
}
