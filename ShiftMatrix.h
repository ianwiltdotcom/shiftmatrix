#include "Arduino.h"

#ifndef ShiftMatrix_h
#define ShiftMatrix_h

class Matrix
{
  public:
    Matrix(int shcp, int stcp, int ds);
	static void setupTimer();
    void update();
    void allOn();
    void allOff();
    void copyScreen(int screen[8]);
	void addScreen(int screen[8], int xoff, int yoff);
    void on(int x, int y);
    void off(int x, int y);
    void rowOn(int y);
    void rowOff(int y);
    void colOn(int x);
    void colOff(int x);
    int matrix[8];
  private:
    void _send(int x, int y);
    void _shiftOut(int myDataPin, int myClockPin, byte myDataOut);
    int _clockPin;
    int _latchPin;
    int _dataPin;
};

#endif
