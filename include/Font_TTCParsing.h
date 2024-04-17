#pragma once
#include <freetype/freetype.h>
#include <iostream>

void printFontStyles(const char* filePath);

int findFontStyleIndex(const char* filePath, const char* styleName);
