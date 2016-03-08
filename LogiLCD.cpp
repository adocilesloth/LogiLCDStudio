/*******************************************
A Docile Sloth 2016 (adocilesloth@gmail.com)
*******************************************/

#include <obs-module.h>

#include "LCDThreads.h"

using namespace std;

thread LcdThread;

OBS_DECLARE_MODULE()

atomic<bool> close(false);
bool running = true;

bool obs_module_load(void)
{
	if(!LogiLcdInit(L"OBS", LOGI_LCD_TYPE_MONO | LOGI_LCD_TYPE_COLOR))
	{
		running = false;
		return true;
	}
	if(LogiLcdIsConnected(LOGI_LCD_TYPE_MONO) && LogiLcdIsConnected(LOGI_LCD_TYPE_COLOR))
	{
		//call dual thread
		//LcdThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Dual, NULL, 0, 0);
		LcdThread = thread(Dual, ref(close));
	}
	else if(LogiLcdIsConnected(LOGI_LCD_TYPE_MONO))
	{
		//call mono thread
		//LcdThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Mono, NULL, 0, 0);
		LcdThread = thread(Mono, ref(close));
	}
	else if(LogiLcdIsConnected(LOGI_LCD_TYPE_COLOR))
	{
		//call colour thread
		//LcdThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Colour, NULL, 0, 0);
		LcdThread = thread(Colour, ref(close));
	}
	return true;
}

//Calling this causes crashes...
/*void obs_module_unload(void)
{
	if(running)
	{
		close = true;
		LcdThread.join();
	}
	return;
}*/

const char *obs_module_author(void)
{
	return "A Docile Sloth";
}

const char *obs_module_name(void)
{
	return "Logitech LCD";
}

const char *obs_module_description(void)
{
	return "Adds Logitech LCD Monochrome and Colour support";
}