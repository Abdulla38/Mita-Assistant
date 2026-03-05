#!/bin/bash

set -xe

gcc src/main.c -o main -lm -lpthread -ldl -lvosk 
