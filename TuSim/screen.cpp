#include "stdafx.h"
#include "Screen.h"

Screen::Screen(bool Erase)
{
  if(Erase)
    Cls();
  SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

Screen::~Screen()
{
  SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

void Screen::SetCurXY(unsigned int valX, unsigned int valY, unsigned int Clr)
{
  COORD coord = {0,0};
  if(valX < MAX_X)
    coord.X = valX;
  if(valY < MAX_Y)
    coord.Y = valY;
  if(Clr != 0)
    SetColor(Clr);
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Screen::SetColor(unsigned int Clr)
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Clr);
}

unsigned int Screen::GetCurX()
{
  CONSOLE_SCREEN_BUFFER_INFO SBInfo;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo);
  return SBInfo.dwCursorPosition.X;
}

unsigned int Screen::GetCurY()
{
  CONSOLE_SCREEN_BUFFER_INFO SBInfo;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo);
  return SBInfo.dwCursorPosition.Y;
}

void Screen::Cls()
{
  system("cls");
}

