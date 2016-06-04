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

	const char* streamName[5] = {"simple_stream", "adv_stream", "simple_file_output", "adv_file_output", "adv_ffmpeg_output"};
	bool streaming = false;
	int indx = 0;

	//ofstream outfile;
	//outfile.open("D:/OBS/build32/rundir/Release/obs-plugins/32bit/outfile.txt");
	//outfile << obs_output_active(streamOutput) << endl;	

	Sleep(1000);

	while(!close) //Text line length  is 26 characters
	{
		//see what's streaming if anything
		if(obs_output_active(obs_get_output_by_name("simple_stream")))
		{
			indx = 0;
			streaming = true;
		}
		else if(obs_output_active(obs_get_output_by_name("adv_stream")))
		{
			indx = 1;
			streaming = true;
		}
		else if(obs_output_active(obs_get_output_by_name("simple_file_output")))
		{
			indx = 2;
			streaming = true;
		}
		else if(obs_output_active(obs_get_output_by_name("adv_file_output")))
		{
			indx = 3;
			streaming = true;
		}
		else if(obs_output_active(obs_get_output_by_name("adv_ffmpeg_output")))
		{
			indx = 4;
			streaming = true;
		}
		else
		{
			streaming = false;
		}

		dvol = obs_get_master_volume();

		///mute and deafen buttons
		/*if(miclast == true && LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_1) == false) //button released
		{
			//OBSToggleMicMute();
		}*/
		/*if(desklast == true && LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_2) == false) //button rleased
		{
		}*/
		//miclast = LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_1);
		//desklast = LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_2);
		//stream and preview buttons
		if(livelast == true && LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_0) == false)
		{
			/*if(obs_output_active(obs_get_output_by_name(streamName)))
			{
				//obs_output_stop(streamOutput);
				streaming = !streaming;
			}
			else
			{
				//obs_output_start(streamOutput);
				streaming = !streaming;
			}*/
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

		if(obs_output_active(obs_get_output_by_name(streamName[indx])))	//streaming
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
			{*/
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
			getFPS(fps, lastframes, fpslastime, streamName[indx]);
			if(fps < 10)
			{
				fpsbyte << L"0";
			}
			fpsbyte << fps << L"      Bitrate: ";
			
			getbps(bitrate, lastbytes, bpslastime, streamName[indx]);
			fpsbyte << int(bitrate);

			wchar_t *fpsbit = new wchar_t[fpsbyte.str().length() + 1];
			wcscpy(fpsbit, fpsbyte.str().c_str());		//copy string
			LogiLcdMonoSetText(2, fpsbit);			//print to lcd
			delete[] fpsbit;
			fpsbyte.str(L"");

			if(altdisplay)
			{
				//dropped frames
				frames << L"Dropped Frames: ";
				dropped = obs_output_get_frames_dropped(obs_get_output_by_name(streamName[indx]));
				frames << dropped << L"(";
				total  = obs_output_get_total_frames(obs_get_output_by_name(streamName[indx]));
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
			{*/
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