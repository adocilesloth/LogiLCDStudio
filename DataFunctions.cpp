/*******************************************
A Docile Sloth 2016 (adocilesloth@gmail.com)
*******************************************/
#include "DataFunctions.h"

int streamTime(int startime)
{
	time_t currtime;
	time(&currtime);
	int uptime = abs(startime - int(currtime));
	return uptime;
}

void getFPS(int &lastfps, int &lastframes, int &lastime, const char* streamName)
{
	time_t currtime;
	time(&currtime);
	if(currtime > lastime)
	{
		int currframes = obs_output_get_total_frames(obs_get_output_by_name(streamName));
		int fps = currframes - lastframes;
		lastfps = fps;
		lastframes = currframes;
		lastime = currtime;
	}
	return;
}

void getbps(float &lastbps, int &lastbytes, int &lastime, const char* streamName)
{
	//returns as kb/s
	time_t currtime;
	time(&currtime);
	if(currtime > lastime)
	{
		int currbytes = obs_output_get_total_bytes(obs_get_output_by_name(streamName));
		int bps = currbytes - lastbytes;
		lastbps = (bps / 1000) * 8;		//convert to kb/s
		lastbytes = currbytes;
		lastime = currtime;
	}
	return;
}