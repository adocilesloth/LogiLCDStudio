/*******************************************
A Docile Sloth 2016 (adocilesloth@gmail.com)
*******************************************/
#include "LCDThreads.h"

using namespace std;

void Dual(atomic<bool>& close)
{
	bool live = false;

	wstring scene;
	//mono buttons
	bool miclast = false;
	bool desklast = false;
	bool livelast = false;
	bool altdisplast = false;
	bool altdisplay = false;
	//colour buttons
	bool leftlast = false;
	bool rightlast = false;
	bool uplast = false;
	bool downlast = false;
	bool oklast = false;
	bool cancellast = false;

	//fps and bitrate
	int fps = 0;
	time_t tlastime;
	int fpslastime;
	int lastframes;

	float bitrate = 0.0;
	int bpslastime;
	int lastbytes;
	wstringstream fpsbyte;
	wstringstream sfps;
	wstringstream sbyte;
	
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

	//stream info
	bool firstime = true;
	obs_output_t* output;

	LogiLcdColorSetTitle(L"OBS", 255, 255, 255);
	
	//Wait for stuff to load or obs_frontend_streaming_active() causes a crash
	obs_source_t* sceneUsed = obs_frontend_get_current_scene();
	while(!sceneUsed)
	{
		LogiLcdMonoSetText(0, L"Open Broadcasting Software");
		LogiLcdMonoSetText(1, L"   Loading...");
		LogiLcdColorSetText(0, L"Open Broadcasting Software", 255, 255, 255);
		LogiLcdColorSetText(2, L"   Loading...", 255, 255, 255);
		LogiLcdUpdate();
		sceneUsed = obs_frontend_get_current_scene();
		Sleep(16);
	}
	obs_source_release(sceneUsed);

	while (!close)
	{
		/*MONO*/
		//mono mute and deafen buttons
		/*if(miclast == true &&  LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_1) == false) //button released
		{
			OBSToggleMicMute();
		}
		if(desklast == true && LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_2) == false) //button rleased
		{
			OBSToggleDesktopMute();
		}
		miclast = LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_1);
		desklast = LogiLcdIsButtonPressed(LOGI_LCD_MONO_BUTTON_2);*/

		//mono stream and preview buttons
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
		LogiLcdMonoSetText(1, name);
		LogiLcdColorSetText(1, name, 255, 255, 255); 
		delete [] name;					//delete temp crap
		scene = L"";

		if(obs_frontend_streaming_active() || obs_frontend_recording_active())	//streaming
		{
			//LogiLcdMonoSetBackground(mono_background_started);	//button help bitmap for mono
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

			/*if(OBSGetMicMuted() && OBSGetDesktopMuted())
			{
				LogiLcdMonoSetText(0, L"OBS     Mute||Deaf       \u25CF");
				LogiLcdColorSetText(6, L"Muted and Deafened", 255, 0, 0);
			}
			else if(OBSGetMicMuted() && !OBSGetDesktopMuted())
			{
				LogiLcdMonoSetText(0, L"OBS     Mute|            \u25CF");
				LogiLcdColorSetText(6, L"Muted", 255, 126, 0);
			}
			else if(!OBSGetMicMuted() && OBSGetDesktopMuted())
			{
				LogiLcdMonoSetText(0, L"OBS          |Deaf       \u25CF");
				LogiLcdColorSetText(6, L"Deafened", 255, 126, 0);
			}
			else
			{*/
				LogiLcdMonoSetText(0, L"OBS                      \u25CF");
				LogiLcdColorSetText(6, L"", 0, 0, 0);
			//}

			//fps and bitrate
			fpsbyte << L"FPS: ";
			sfps << L"FPS: ";
			getFPS(fps, lastframes, fpslastime);
			if(fps < 10)
			{
				fpsbyte << L"0";
				sfps << L"0";
			}
			fpsbyte << fps << L"      Bitrate: ";
			sfps << fps;
			sbyte << L"Bitrate: ";
			getbps(bitrate, lastbytes, bpslastime);	//get as kb/s
			fpsbyte << int(bitrate);
			sbyte << int(bitrate) << L"kb/s";
			//mono fps and bitrate
			wchar_t *fpsbit = new wchar_t[fpsbyte.str().length() +1];
			wcscpy(fpsbit, fpsbyte.str().c_str());		//copy string
			//colour fps
			wchar_t *wfps = new wchar_t[sfps.str().length() +1];
			wcscpy(wfps, sfps.str().c_str());				//copy string
			//colour bitrate
			wchar_t *wbit = new wchar_t[sbyte.str().length() +1];
			wcscpy(wbit, sbyte.str().c_str());				//copy string
			//print to LCDs
			LogiLcdMonoSetText(2, fpsbit);			//print to mono lcd
			//fpspercent = double(fps)/OBSGetMaxFPS();
			/*if(fpspercent >= 0.83 && fpspercent < 0.96)
			{
				LogiLcdColorSetText(2, wfps, 255, 126, 0);	//print to lcd as yellow
			}
			else if(fpspercent < 0.83)
			{
				LogiLcdColorSetText(2, wfps, 255, 0, 0);	//print to lcd as red
			}
			else
			{*/
				LogiLcdColorSetText(2, wfps, 255, 255, 255);	//print colour to lcd
			//}
			LogiLcdColorSetText(3, wbit, 255, 255, 255);	//print colour to lcd
			//clear up
			delete [] fpsbit;
			delete [] wfps;
			delete [] wbit;
			fpsbyte.str(L"");
			sfps.str(L"");
			sbyte.str(L"");

			if(obs_frontend_streaming_active())
			{
				output = obs_frontend_get_streaming_output();
			}
			else if(obs_frontend_recording_active())
			{
				output = obs_frontend_get_recording_output();
			}

			//dropped frames calculations
			frames << L"Dropped Frames: ";
			dropped = obs_output_get_frames_dropped(output);
			frames << dropped << L"(";
			total = obs_output_get_total_frames(output);
			percent = (double(dropped)/total)*100;
			frames << fixed << setprecision(2) << percent << L"%)";
			wchar_t *droppedframes = new wchar_t[frames.str().length() +1];
			wcscpy(droppedframes, frames.str().c_str());			//copy string

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
			wcscpy(streamtime, stime.str().c_str());			//copy string*/

			if(altdisplay)
			{
				//dropped frames
				LogiLcdMonoSetText(3, droppedframes);			//print to lcd
			}
			else
			{
				//stream time
				LogiLcdMonoSetText(3, streamtime);			//print to lcd
				
			}

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
				LogiLcdColorSetText(4, droppedframes, 225, 225, 225);	//print colour to lcd
			}
			LogiLcdColorSetText(5, streamtime, 255, 255, 255);	//print to lcd

			delete [] droppedframes;
			delete [] streamtime;
			frames.str(L"");
			stime.str(L"");
		}
		else
		{
			//LogiLcdMonoSetBackground(mono_background_stopped);	//button help bitmap for mono
			LogiLcdColorSetText(0, L"", 0, 0, 0);
			/*if(OBSGetMicMuted() && OBSGetDesktopMuted())
			{
				LogiLcdMonoSetText(0, L"OBS     Mute||Deaf       \u25CB");
				LogiLcdColorSetText(6, L"Muted and Deafened", 255, 0, 0);
			}
			else if(OBSGetMicMuted() && !OBSGetDesktopMuted())
			{
				LogiLcdMonoSetText(0, L"OBS     Mute|            \u25CB");
				LogiLcdColorSetText(6, L"Muted", 255, 126, 0);
			}
			else if(!OBSGetMicMuted() && OBSGetDesktopMuted())
			{
				LogiLcdMonoSetText(0, L"OBS          |Deaf       \u25CB");
				LogiLcdColorSetText(6, L"Deafened", 255, 126, 0);
			}
			else
			{*/
				LogiLcdMonoSetText(0, L"OBS                      \u25CB");
				LogiLcdColorSetText(6, L"", 0, 0, 0);
			//}

			LogiLcdColorSetText(0, L"", 0, 0, 0);

			LogiLcdMonoSetText(2, L"FPS: --      Bitrate: ----");
			LogiLcdColorSetText(2, L"FPS: --");
			LogiLcdColorSetText(3, L"Bitrate: ----kb/s", 255, 255, 255);
			if(altdisplay)
			{
				LogiLcdMonoSetText(3, L"Dropped Frames: -(-.--%)");
			}
			else
			{
				LogiLcdMonoSetText(3, L"Stream Uptime: -:--:--");
			}
			LogiLcdColorSetText(4, L"Dropped Frames: -(-.--%)", 255, 255, 255);
			LogiLcdColorSetText(5, L"Stream Uptime: -:--:--", 255, 255, 255);
		}
		//update screen
		LogiLcdUpdate();

		Sleep(16);
	}
	LogiLcdShutdown();
	return;
}