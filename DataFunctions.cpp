/*******************************************
A Docile Sloth 2016 (adocilesloth@gmail.com)
*******************************************/
#include "DataFunctions.h"
#include <Windows.h>

std::wstring s2ws(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

int streamTime(int startime)
{
	time_t currtime;
	time(&currtime);
	int uptime = abs(startime - int(currtime));
	return uptime;
}

void getFPS(int &lastfps, int &lastframes, int &lastime)
{
	time_t currtime;
	time(&currtime);

	obs_output_t* output;
	if(obs_frontend_streaming_active())
	{
		output = obs_frontend_get_streaming_output();
	}
	else if(obs_frontend_recording_active())
	{
		output = obs_frontend_get_recording_output();
	}

	if(currtime > lastime)
	{
		int currframes = obs_output_get_total_frames(output);
		int fps = currframes - lastframes;
		lastfps = fps;
		lastframes = currframes;
		lastime = currtime;
	}
	return;
}

void getbps(float &lastbps, int &lastbytes, int &lastime)
{
	//returns as kb/s
	time_t currtime;
	time(&currtime);

	obs_output_t* output;
	if(obs_frontend_streaming_active())
	{
		output = obs_frontend_get_streaming_output();
	}
	else if(obs_frontend_recording_active())
	{
		output = obs_frontend_get_recording_output();
	}

	if(currtime > lastime)
	{
		int currbytes = obs_output_get_total_bytes(output);
		int bps = currbytes - lastbytes;
		lastbps = (bps / 1000) * 8;		//convert to kb/s
		lastbytes = currbytes;
		lastime = currtime;
	}
	return;
}

std::wstring getScene()
{
	obs_source_t* sceneUsed = obs_frontend_get_current_scene();
	const char *sceneUsedName = obs_source_get_name(sceneUsed);
	obs_source_release(sceneUsed);
	std::string sceneName = sceneUsedName;
	std::wstring wsceneName = s2ws(sceneName);
	return wsceneName;
}