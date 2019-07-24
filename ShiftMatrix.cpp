#include "Arduino.h"
#include "ShiftMatrix.h"

Matrix::Matrix(int dataPin, int clockPin, int latchPin) {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  _clockPin = clockPin;
  _latchPin = latchPin;
  _dataPin = dataPin;
}

void Matrix::setupTimer() {
	cli();

	//set timer1 interrupt at 10Hz
	TCCR1A = 0;// set entire TCCR1A register to 0
	TCCR1B = 0;// same for TCCR1B
	TCNT1 = 0;//initialize counter value to 0
			  // set compare match register for 1hz increments
	OCR1A = 2499;// = (16*10^6) / (1*1024) - 1 (must be <65536)
				 // turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS12 and CS10 bits for 1024 prescaler
	TCCR1B |= (1 << CS11) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);

	sei();
}

void Matrix::on(int x, int y) {
  matrix[y] |= 1 << x;
}

void Matrix::off(int x, int y) {
  matrix[y] &= ~(1 << x);
}

void Matrix::allOn() {
  for (int i = 0; i < 8; i++) {
    matrix[i] = 255;
  }
}

void Matrix::allOff() {
  for (int i = 0; i < 8; i++) {
    matrix[i] = 0;
  }
}

void Matrix::copyScreen(int screen[8]) {
  memcpy(matrix, screen, sizeof(matrix));
}

void Matrix::addScreen(int screen[8], int xoff, int yoff) {
	for (int i = 0; i < 8; i++) {
		matrix[i] |= screen[i - yoff] << xoff;
  }
}

void Matrix::rowOn(int y) {
  matrix[y] = 0b11111111;
}

void Matrix::rowOff(int y) {
  matrix[y] = 0;
}

void Matrix::colOn(int x) {
  for (int i = 0; i < 8; i++) {
    matrix[i] |= (1 << x);
  }
}

void Matrix::colOff(int x) {
  for (int i = 0; i < 8; i++) {
    matrix[i] &= ~(1 << x);
  }
}

void Matrix::_send(int x, int y) {
  digitalWrite(_latchPin, 1);
  _shiftOut(_dataPin, _clockPin, ~x);
  _shiftOut(_dataPin, _clockPin, y);
  digitalWrite(_latchPin, 0);
}

void Matrix::_shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  int i = 0;
  int pinState;
  pinMode(_clockPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);

  digitalWrite(_dataPin, 0);
  digitalWrite(_clockPin, 0);

  for (i = 7; i >= 0; i--)  {
    digitalWrite(_clockPin, 0);
    if ( myDataOut & (1 << i) ) {
      pinState = 1;
    }
    else {
      pinState = 0;
    }
    digitalWrite(_dataPin, pinState);
    digitalWrite(_clockPin, 1);
    digitalWrite(_dataPin, 0);
  }
  digitalWrite(_clockPin, 0);
}

void Matrix::update() {
  for (int i = 0; i < 8; i++) {
    _send(matrix[i], 1 << i);
  }
}
