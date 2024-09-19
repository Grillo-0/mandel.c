# 
# Copyright Arthur Grillo (c) 2024
# 
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
# 

CC ?= clang
CFLAGS := -ggdb -Wall -Wextra -pedantic -std=gnu2x
LDFLAGS := -lm $(shell pkg-config --libs raylib)

mandel: mandel.c

run: mandel
	./mandel
