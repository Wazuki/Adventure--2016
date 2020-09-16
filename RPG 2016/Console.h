// console.h
//
#pragma once

//
// Clears the screen
//
void clrscr();

//
// Moves the cursor to x, y in console window
// ie x=left\right y=top\bottom
//
void gotoxy(int x, int y);

//
// Set text and background colors
//
void setrgb(int color);

