/*
AudioAnalyzer.cpp - Library for audio spectrum analyzer.
Created by Lauren Pan,November 16, 2010.
Version 1.2
Add optional analog pin
*/

#ifndef AudioAnalyzer_h
#define AudioAnalyzer_h
#include "Arduino.h"

#define Maxband 7


class Analyzer{
public:
	Analyzer();
	Analyzer(int,int,int);
	void Init();
	void ReadFreq(int *);

private:
	int _DCPin;
	int _StrobePin;
	int _RSTPin;
	void RstModule();
	unsigned long _TimepointSt;
	unsigned long _TimepointNow;
};

#endif