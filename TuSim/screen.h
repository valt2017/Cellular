#ifndef SCREEN_H
#define SCREEN_H

#include <windows.h>

//http://msdn.microsoft.com/en-us/library/windows/desktop/ms682088(v=vs.85).aspx#_win32_character_attributes
/*
Attribute	Meaning
FOREGROUND_BLUE	Text color contains blue.
FOREGROUND_GREEN	Text color contains green.
FOREGROUND_RED	Text color contains red.
FOREGROUND_INTENSITY	Text color is intensified.
BACKGROUND_BLUE	Background color contains blue.
BACKGROUND_GREEN	Background color contains green.
BACKGROUND_RED	Background color contains red.
BACKGROUND_INTENSITY	Background color is intensified.
*/

class Screen
{
  public:
    Screen(bool Erase=true);
    virtual ~Screen();
    unsigned int GetCurX();
    unsigned int GetCurY();
    void SetCurXY(unsigned int valX, unsigned int valY, unsigned int Clr = 0);
    void Cls();
    void SetColor(unsigned int Clr);
  protected:
  private:
    enum {MAX_X = 80};
    enum {MAX_Y = 25};
};

#endif // SCREEN_H
