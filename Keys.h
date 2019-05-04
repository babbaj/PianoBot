#pragma once

#include <unordered_map>


// map uppercase char to char that is used to create it
const std::unordered_map<char, char> BLACK_KEYS = {
    {'!', '1'},
    {'@', '2'},
    {'$', '4'},
    {'%', '5'},
    {'^', '6'},
    {'*', '8'},
    {'(', '9'},

    {'Q', 'Q'}, // TODO: simplify
    {'W', 'W'},
    {'E', 'E'},
    {'T', 'T'},
    {'Y', 'Y'},
    {'I', 'I'},
    {'O', 'O'},
    {'P', 'P'},
    {'S', 'S'},
    {'D', 'D'},
    {'G', 'G'},
    {'H', 'H'},
    {'J', 'J'},
    {'L', 'L'},
    {'Z', 'Z'},
    {'C', 'C'},
    {'V', 'V'},
    {'B', 'B'}
};

const std::set<char> VALID_KEYS = {
    '1', '!', '2', '@', '3', '4', '$', '5', '%', '6', '^', '7', '8', '*', '9', '(', '0',
    'q', 'Q', 'w', 'W', 'e', 'E', 'r', 't', 'T', 'y', 'Y', 'u', 'i', 'I', 'o', 'O', 'p', 'P', 'a', 's', 'S', 'd', 'D', 'f', 'g', 'G', 'h', 'H', 'j', 'J', 'k', 'l', 'L', 'z', 'Z', 'x', 'c', 'C', 'v', 'V', 'b', 'B', 'n', 'm'
};

const std::set<char> GROUP_CHARS = {
    '[', ']'
};

const std::set<char> PAUSE_CHARS = {
    '#', '&', ')', '|'/*, '-'*/
};

bool isBlackKey(char key) {
    return BLACK_KEYS.find(key) != BLACK_KEYS.end();
}