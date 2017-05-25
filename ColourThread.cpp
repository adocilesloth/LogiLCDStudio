/*******************************************
A Docile Sloth 2016 (adocilesloth@gmail.com)
*******************************************/

#include "LCDThreads.h"

using namespace std;

void Colour(atomic<bool>& close)
{
	wstring scene;
	bool leftlast = false;
	bool rightlast = false;
	bool uplast = false;
	bool downlast = false;
	bool oklast = false;
	bool cancellast = false;

	//fps
	int fps = 0;
	time_t tlastime;
	int fpslastime;
	int lastframes;
	wstringstream sfps;

	//bit rate
	float bitrate = 0.0;
	int bpslastime;
	int lastbytes;
	wstringstream sbyte;

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

	LogiLcdColorSetTitle(L"OBS", 255, 255, 255);
	
	//Wait for stuff to load or obs_frontend_streaming_active() causes a crash
	obs_source_t* sceneUsed = obs_frontend_get_current_scene();
	while(!sceneUsed)
	{
		LogiLcdColorSetText(0, L"Open Broadcasting Software", 255, 255, 255);
		LogiLcdColorSetText(2, L"   Loading...", 255, 255, 255);
		LogiLcdUpdate();
		sceneUsed = obs_frontend_get_current_scene();
		Sleep(16);
	}
	obs_source_release(sceneUsed);

	while (!close)
	{
		//mute and deafen buttons
		if(leftlast == true &&  LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_LEFT) == false) //button released
		{
			//OBSToggleMicMute();
		}
		if(rightlast == true && LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_RIGHT) == false) //button rleased
		{
			//OBSToggleDesktopMute();
		}
		leftlast = LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_LEFT);
		rightlast = LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_RIGHT);
		if(uplast == true &&  LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_UP) == false) //button released
		{
			//OBSToggleMicMute();
		}
		if(downlast == true && LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_DOWN) == false) //button rleased
		{
			//OBSToggleDesktopMute();
		}
		uplast = LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_UP);
		downlast = LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_DOWN);
		//stream and preview buttons
		if(oklast == true &&  LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_OK) == false) //button released
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
		if(cancellast == true &&  LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_CANCEL) == false) //button released
		{
			//OBSStartStopPreview();
		}
		oklast = LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_OK);
		cancellast = LogiLcdIsButtonPressed(LOGI_LCD_COLOR_BUTTON_CANCEL);

		//get scene information
		scene = L"Scene: ";
		scene.append(getScene());
		wchar_t *name = new wchar_t[scene.length() + 1];
		wcscpy(name, scene.c_str());	//convert to wchar_t
		LogiLcdColorSetText(1, name, 255, 255, 255); 
		delete [] name;					//delete temp crap
		scene = L"";

		if(obs_frontend_streaming_active() || obs_frontend_recording_active())	//streaming
		{
			//live
			LogiLcdColorSetText(0, L"Live \u25CF", 255, 0, 0);

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

			//fps
			sfps << L"FPS: ";
			getFPS(fps, lastframes, fpslastime);
			if(fps < 10)
			{
				sfps << L"0";
			}
			sfps << fps;
			wchar_t *wfps = new wchar_t[sfps.str().length() +1];
			wcscpy(wfps, sfps.str().c_str());				//copy string
			//fpspercent = double(fps)/OBSGetMaxFPS();
			/*if(fpspercent >= 0.83 && fpspercent < 0.96)
			{
				LogiLcdColorSetText(2, wfps, 255, 126, 0);	//print to lcd as yellow
			}
			else if(fpspercent < 0.83)
			{
				LogiLcdColorSetText(2, wfps, 255, 0, 0);	//print to lcd as red
			}*/
			//else
			//{
			LogiLcdColorSetText(2, wfps, 255, 255, 255);	//print to lcd
			//}
			delete [] wfps;
			sfps.str(L"");

			//bitrate
			sbyte << L"Bitrate: ";
			getbps(bitrate, lastbytes, bpslastime);
			sbyte << int(bitrate) << L"kb/s";
			wchar_t *wbit = new wchar_t[sbyte.str().length() +1];
			wcscpy(wbit, sbyte.str().c_str());				//copy string
			LogiLcdColorSetText(3, wbit, 255, 255, 255);	//print to lcd
			delete [] wbit;
			sbyte.str(L"");

			obs_output_t* output;
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
			percent = (double(dropped)/total)*100;
			frames << fixed << setprecision(2) << percent << L"%)";
			wchar_t *droppedframes = new wchar_t[frames.str().length() +1];
			wcscpy(droppedframes, frames.str().c_str());			//copy string

			if(percent >= 5 && percent < 10)	//between 5% and 10% dropped frames
			{
				LogiLcdColorSetText(4, droppedframes, 225, 126, 0);	//print to lcd as yellow
			}
			else if(percent >= 10)		//over 10% dropped
			{
				LogiLcdColorSetText(4, droppedframes, 225, 0, 0);	//print to lcd as red
			}
			else
			{
				LogiLcdColorSetText(4, droppedframes, 225, 225, 225);	//print to lcd
			}
			delete [] droppedframes;
			frames.str(L"");

			//stream time
			stime << L"Stream Uptime: ";
			uptime = streamTime(startime);
			hour = (uptime / (60*60)) % 60;
			min = (uptime / 60) % 60;
			sec = (uptime % 60);

			stime << hour << L":";
			if(min < 10)
			{
				stime << L"0";
			}
			stime << min << L":";
			if(sec < 10)
			{
				stime << L"0";
			}
			stime << sec;

			wchar_t *streamtime = new wchar_t[stime.str().length() +1];
			wcscpy(streamtime, stime.str().c_str());			//copy string
			LogiLcdColorSetText(5, streamtime, 255, 255, 255);	//print to lcd
			delete [] streamtime;
			stime.str(L"");
		}
		else
		{
			LogiLcdColorSetText(0, L"", 0, 0, 0);
			LogiLcdColorSetText(2, L"FPS: --", 255, 255, 255);
			LogiLcdColorSetText(3, L"Bitrate: ----kb/s", 255, 255, 255);
			LogiLcdColorSetText(4, L"Dropped Frames: -(-.--%)", 255, 255, 255);
			LogiLcdColorSetText(5, L"Stream Uptime: -:--:--", 255, 255, 255);
		}

		/*if(OBSGetMicMuted() && OBSGetDesktopMuted())
		{
			LogiLcdColorSetText(6, L"Muted and Deafened", 255, 0, 0);
		}
		else if(OBSGetMicMuted() && !OBSGetDesktopMuted())
		{
			LogiLcdColorSetText(6, L"Muted", 255, 126, 0);
		}
		else if(!OBSGetMicMuted() && OBSGetDesktopMuted())
		{
			LogiLcdColorSetText(6, L"Deafened", 255, 126, 0);
		}
		else
		{*/
		LogiLcdColorSetText(6, L"", 0, 0, 0);
		//}
		//update screen
		LogiLcdUpdate();

		Sleep(16);
	}

	LogiLcdShutdown();
	return;
}