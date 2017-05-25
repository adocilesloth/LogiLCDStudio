/*******************************************
A Docile Sloth 2016 (adocilesloth@gmail.com)
*******************************************/

#include "LCDThreads.h"
//#include <fstream>

using namespace std;

void Mono(atomic<bool>& close)
{
	wstring scene;
	bool miclast = false;
	bool desklast = false;
	bool livelast = false;
	bool altdisplast = false;

	//fps and bitrate
	int fps = 0;
	time_t tlastime;
	int fpslastime;
	int lastframes;

	float bitrate = 0.0;
	int bpslastime;
	int lastbytes;
	wstringstream fpsbyte;

	bool altdisplay = false;
	//dropped frames
	int dropped;
	int total;
	double percent;
	wstringstream frames;
	//stream time
	time_t tstartime;
	int startime;
	int uptime;
	int sec;
	int min;
	int hour;
	wstringstream stime;

	//volume and mic levels
	float mvol;		//mic volume
	float dvol;		//desktop volume

	//stream info
	bool firstime = true;
	obs_output_t* output;

	//ofstream outfile;
	//outfile.open("D:/OBS/build32/rundir/Release/obs-plugins/32bit/outfile2.txt");
	//outfile << obs_output_active(streamOutput) << endl;	
	
	//Wait for stuff to load or obs_frontend_streaming_active() causes a crash
	obs_source_t* sceneUsed = obs_frontend_get_current_scene();
	while(!sceneUsed)
	{
		LogiLcdMonoSetText(0, L"Open Broadcasting Software");
		LogiLcdMonoSetText(1, L"   Loading...");
		LogiLcdUpdate();
		sceneUsed = obs_frontend_get_current_scene();
		Sleep(16);
	}
	obs_source_release(sceneUsed);

	while(!close) //Text line length  is 26 characters
	{
		//see what's streaming if anything
		/*if(obs_frontend_streaming_active() || obs_frontend_recording_active())
		{
			streaming = true;
		}
		else
		{
			streaming = false;
		}*/

		///mute and deafen buttons
		/*if(miclast == true && LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_1) == false) //button released
		{
			//OBSToggleMicMute();
		}*/
		if(desklast == true && LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_2) == false) //button rleased
		{
			obs_source_t* sceneUsed = obs_frontend_get_current_scene();
			if(sceneUsed)
			{
				if(obs_source_muted(sceneUsed))
				{
					obs_source_set_muted(sceneUsed, false);
				}
				else
				{
					obs_source_set_muted(sceneUsed, true);
				}
				obs_source_release(sceneUsed);
			}
		}
		//miclast = LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_1);
		desklast = LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_2);
		//stream and preview buttons
		if(livelast == true && LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_0) == false)
		{
			if(obs_frontend_streaming_active())
			{
				obs_frontend_streaming_stop();
			}
			else
			{
				obs_frontend_streaming_start();
			}
		}
		if(altdisplast == true && LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_3) == false)
		{
			if(altdisplay)
			{
				altdisplay = false;
			}
			else
			{
				altdisplay = true;
			}
		}
		livelast = LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_0);
		altdisplast = LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_3);

		//scene information
		scene = L"Scene: ";
		scene.append(getScene());
		wchar_t *name = new wchar_t[scene.length() + 1];
		wcscpy(name, scene.c_str());	//convert to wchar_t
		LogiLcdMonoSetText(1, name);	//print
		delete[] name;					//delete temp crap
		scene = L"";

		if(obs_frontend_streaming_active() || obs_frontend_recording_active())	//streaming
		{
			//LogiLcdMonoSetBackground(mono_background_started);
			/*if(OBSGetMicMuted() && OBSGetDesktopMuted())
			{
				LogiLcdMonoSetText(0, L"OBS     Mute||Deaf   live\u25CF");
			}
			else if(OBSGetMicMuted() && !OBSGetDesktopMuted())
			{
				LogiLcdMonoSetText(0, L"OBS     Mute|        live\u25CF");
			}
			else if(!OBSGetMicMuted() && OBSGetDesktopMuted())
			{
				LogiLcdMonoSetText(0,L"OBS          |Deaf   live\u25CF");
			}
			else
			{*///-------------------------------------------------------------------------------------------------
				LogiLcdMonoSetText(0, L"OBS                  live\u25CF");
			//}
			
			if(firstime == true)
			{
				firstime = false;
				time(&tstartime);
				time(&tlastime);
				startime = int(tstartime);
				fpslastime = int(tlastime);
				bpslastime = int(tlastime);
				lastframes = 0;
				lastbytes = 0;
			}

			//fps and bitrate
			fpsbyte << L"FPS: ";
			getFPS(fps, lastframes, fpslastime);//, streamName[indx]);
			if(fps < 10)
			{
				fpsbyte << L"0";
			}
			fpsbyte << fps << L"      Bitrate: ";
			
			getbps(bitrate, lastbytes, bpslastime);//, streamName[indx]);
			fpsbyte << int(bitrate);

			wchar_t *fpsbit = new wchar_t[fpsbyte.str().length() + 1];
			wcscpy(fpsbit, fpsbyte.str().c_str());		//copy string
			LogiLcdMonoSetText(2, fpsbit);			//print to lcd
			delete[] fpsbit;
			fpsbyte.str(L"");

			if(altdisplay)
			{
				if(obs_frontend_streaming_active())
				{
					output = obs_frontend_get_streaming_output();
				}
				else if(obs_frontend_recording_active())
				{
					output = obs_frontend_get_recording_output();
				}

				//dropped frames
				frames << L"Dropped Frames: ";
				dropped = obs_output_get_frames_dropped(output);
				frames << dropped << L"(";
				total  = obs_output_get_total_frames(output);
				percent = (double(dropped) / total) * 100;
				frames << fixed << setprecision(2) << percent << L"%)";

				wchar_t *droppedframes = new wchar_t[frames.str().length() +1];
				wcscpy(droppedframes, frames.str().c_str());		//copy string
				LogiLcdMonoSetText(3, droppedframes);			//print to lcd
				delete[] droppedframes;
				frames.str(L"");
			}
			else
			{
				//stream time
				stime << L"Stream Uptime: ";
				uptime = streamTime(startime);
				hour = (uptime / (60 * 60)) % 60;
				min = (uptime / 60) % 60;
				sec = (uptime % 60);

				stime << hour << L":";
				if (min < 10)
				{
					stime << L"0";
				}
				stime << min << L":";
				if (sec < 10)
				{
					stime << L"0";
				}
				stime << sec;

				wchar_t *streamtime = new wchar_t[stime.str().length() + 1];
				wcscpy(streamtime, stime.str().c_str());		//copy string
				LogiLcdMonoSetText(3, streamtime);			//print to lcd
				delete[] streamtime;
				stime.str(L"");
			}
		}//end of streaming
		else
		{
			//LogiLcdMonoSetBackground(mono_background_stopped);
			/*if(OBSGetMicMuted() && OBSGetDesktopMuted())
			{
				LogiLcdMonoSetText(0, L"OBS     Mute||Deaf       \u25CB");
			}
			else if(OBSGetMicMuted() && !OBSGetDesktopMuted())
			{
				LogiLcdMonoSetText(0, L"OBS     Mute|            \u25CB");
			}
			else if(!OBSGetMicMuted() && OBSGetDesktopMuted())
			{
				LogiLcdMonoSetText(0, L"OBS          |Deaf       \u25CB");
			}
			else
			{*///-------------------------------------------------------------------------------------------
				LogiLcdMonoSetText(0, L"OBS                      \u25CB");
			//}

			LogiLcdMonoSetText(2, L"FPS: --      Bitrate: ----");

			if(firstime == false)
			{
				firstime =true;
			}
			if(altdisplay)
			{
				LogiLcdMonoSetText(3, L"Dropped Frames: -(-.--%)");
			}
			else
			{
				LogiLcdMonoSetText(3, L"Stream Uptime: -:--:--");
			}
		}
		//update screen
		LogiLcdUpdate();

		Sleep(16);
	}
	LogiLcdShutdown();
	return;
}