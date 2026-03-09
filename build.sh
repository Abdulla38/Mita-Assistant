#!/bin/bash

set -xe

gcc src/main.c src/audio/audio_capture.c src/speech/speech_recognition.c -o build/Mita -lm -lpthread -ldl -lvosk 
