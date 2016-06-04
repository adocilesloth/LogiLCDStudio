/*******************************************
A Docile Sloth 2016 (adocilesloth@gmail.com)
*******************************************/
#ifndef DATAFUNCTIONS_H
#define DATAFUNCTIONS_H

#include "obs.h"

#include <ctime>
#include <string>

int streamTime(int);
void getFPS(int&, int&, int&, const char*);
void getbps(float&, int&, int&, const char*);
std::wstring getScene();

#endif