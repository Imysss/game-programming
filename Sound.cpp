#include "hw3.h"
#include "SoundManager.h"
#include <time.h>
using namespace D2D1;

BOOL DemoApp::addSound(HWND hWnd) {
	//SOUND
	soundManager = new CSoundManager;
	if (!soundManager->init(hWnd))
		return FALSE;

	// 사운드 파일을 추가 (id는 0부터 시작)
	int id;
	if (!soundManager->add(L"./sound/background.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/틀 선택 소리.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/냉각기.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/아이템 선택 소리.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/코인 증가.wav", &id))
		return FALSE;
	return true;
}

BOOL DemoApp::SetVolume(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lVolume) {
	if (!lpDSBuffer) return FALSE; 
	if (FAILED(lpDSBuffer->SetVolume(DSVOLUME_TO_DB(lVolume))))
		return FALSE;
	return TRUE;
}

