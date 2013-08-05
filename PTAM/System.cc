// Copyright 2008 Isis Innovation Limited
#include "System.h"
#include "OpenGL.h"
#include <gvars3/instances.h>
#include <stdlib.h>
#include "ATANCamera.h"
#include "MapMaker.h"
#include "Tracker.h"
#include "ARDriver.h"
#include "MapViewer.h"
#include <fstream>
#define _POSIX_SOURCE
#include <sys/stat.h>
#include <unistd.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include <cvd/image_io.h>
#include <stdlib.h>

using namespace CVD;
using namespace std;
using namespace GVars3;

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    for(int i = 0; i < 80; i ++)
    {
    	if(buf[i] == ':') buf[i] = '-';
    }
    return buf;
}

System::System() :
		mGLWindow(mVideoSource.Size(), "PTAM") {
	GUI.RegisterCommand("exit", GUICommandCallBack, this);
	GUI.RegisterCommand("quit", GUICommandCallBack, this);

	mimFrameBW.resize(mVideoSource.Size());
	mimFrameRGB.resize(mVideoSource.Size());
	// First, check if the camera is calibrated.
	// If not, we need to run the calibration widget.
	Vector<NUMTRACKERCAMPARAMETERS> vTest;

	vTest = GV3::get<Vector<NUMTRACKERCAMPARAMETERS> >("Camera.Parameters",
			ATANCamera::mvDefaultParams, HIDDEN);
	mpCamera = new ATANCamera("Camera");
	Vector<2> v2;
	if (v2 == v2)
		;
	if (vTest == ATANCamera::mvDefaultParams) {
		cout << endl;
		cout
				<< "! Camera.Parameters is not set, need to run the CameraCalibrator tool"
				<< endl;
		cout
				<< "  and/or put the Camera.Parameters= line into the appropriate .cfg file."
				<< endl;
		exit(1);
	}

	mpMap = new Map;
	mpMapMaker = new MapMaker(*mpMap, *mpCamera);
	mpTracker = new Tracker(mVideoSource.Size(), *mpCamera, *mpMap,
			*mpMapMaker);
	mpARDriver = new ARDriver(*mpCamera, mVideoSource.Size(), mGLWindow);
	mpMapViewer = new MapViewer(*mpMap, mGLWindow);

	GUI.ParseLine("GLWindow.AddMenu Menu Menu");
	GUI.ParseLine("Menu.ShowMenu Root");
	GUI.ParseLine("Menu.AddMenuButton Root Reset Reset Root");
	GUI.ParseLine("Menu.AddMenuButton Root Spacebar PokeTracker Root");
	GUI.ParseLine("DrawAR=0");
	GUI.ParseLine("DrawMap=0");
	GUI.ParseLine("Menu.AddMenuToggle Root \"View Map\" DrawMap Root");
	GUI.ParseLine("Menu.AddMenuToggle Root \"Draw AR\" DrawAR Root");

	mbDone = false;
}
;

void System::Run() {
	int i = 0;
	ofstream
		poses;
	string
		date = currentDateTime();
	time_t t = time(0);

	mkdir("out", S_IRWXU|S_IRGRP|S_IXGRP);
	mkdir(("out/"+date).c_str(), S_IRWXU|S_IRGRP|S_IXGRP);
	poses.open(("out/"+date+"/"+date + ".campose").c_str());

	while (!mbDone) {

		// We use two versions of each video frame:
		// One black and white (for processing by the tracker etc)
		// and one RGB, for drawing.

		// Grab new video frame...
		mVideoSource.GetAndFillFrameBWandRGB(mimFrameBW, mimFrameRGB);
		static bool bFirstFrame = true;
		if (bFirstFrame) {
			mpARDriver->Init();
			bFirstFrame = false;
		}

		mGLWindow.SetupViewport();
		mGLWindow.SetupVideoOrtho();
		mGLWindow.SetupVideoRasterPosAndZoom();

		if (!mpMap->IsGood())
			mpARDriver->Reset();

		static gvar3<int> gvnDrawMap("DrawMap", 0, HIDDEN | SILENT);
		static gvar3<int> gvnDrawAR("DrawAR", 0, HIDDEN | SILENT);

		bool bDrawMap = mpMap->IsGood() && *gvnDrawMap;
		bool bDrawAR = mpMap->IsGood() && *gvnDrawAR;

		mpTracker->TrackFrame(mimFrameBW, !bDrawAR && !bDrawMap);

		SE3<> cPose = mpTracker->GetCurrentPose();

		if (bDrawMap)
			mpMapViewer->DrawMap(cPose);
		else if (bDrawAR)
			mpARDriver->Render(mimFrameRGB, cPose);

		if(mpMap->IsGood())
		{
			char buffer [33];

			sprintf(buffer,"%d",i);
			string filename = "out/"+date+"/"+date+"_"+string(buffer)+".png";
			img_save(mimFrameRGB, filename);

			Vector<3>& translation = cPose.get_translation();
			SO3<>& rotation = cPose.get_rotation();
			Matrix <3,3> rotMtx = rotation.get_matrix();
			poses
				<< filename << endl
				<< translation
				<< endl
				<< rotMtx
				<< endl;
		}

		//      mGLWindow.GetMousePoseUpdate();
		string sCaption;
		if (bDrawMap)
			sCaption = mpMapViewer->GetMessageForUser();
		else
			sCaption = mpTracker->GetMessageForUser();
		mGLWindow.DrawCaption(sCaption);
		mGLWindow.DrawMenus();
		mGLWindow.swap_buffers();
		mGLWindow.HandlePendingEvents();
	}

	poses.close();
}

void System::GUICommandCallBack(void *ptr, string sCommand, string sParams) {
	if (sCommand == "quit" || sCommand == "exit")
		static_cast<System*>(ptr)->mbDone = true;
}

