/*******************************************
A Docile Sloth 2016 (adocilesloth@gmail.com)
*******************************************/
#ifndef LCDTHREADS_H
#define LCDTHREADS_H

#include "LogitechLCDLib.h"
#include "obs.h"
#include "DataFunctions.h"

#include <string>
#include <sstream>
#include <atomic>
#include <iomanip>
#include <ctime>
#include <thread>

void Mono(std::atomic<bool>&);
void Colour(std::atomic<bool>&);
void Dual(std::atomic<bool>&);

#endif //LCDTHREADS_H