#!/bin/sh

set -xe
gcc src/wavs.c -Wall -Wextra -lm -o wavs
