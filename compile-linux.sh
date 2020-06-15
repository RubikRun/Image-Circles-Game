#!/bin/bash
g++ -O3 -o game main.cpp Game/Game.cpp Image/Circle.cpp Image/Image.cpp -l sfml-graphics -l sfml-window -l sfml-system
