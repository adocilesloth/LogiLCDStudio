/*******************************************
A Docile Sloth 2016 (adocilesloth@gmail.com)
*******************************************/
#ifndef DATAFUNCTIONS_H
#define DATAFUNCTIONS_H

#include "obs.h"
#include "obs-frontend-api\obs-frontend-api.h"

#include <ctime>
#include <string>

int streamTime(int);
void getFPS(int&, int&, int&);
void getbps(float&, int&, int&);
std::wstring getScene();

#endif