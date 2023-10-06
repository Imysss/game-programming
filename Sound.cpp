#include "hw3.h"
#include "SoundManager.h"
#include <time.h>
using namespace D2D1;

BOOL DemoApp::addSound(HWND hWnd) {
	//SOUND
	soundManager = new CSoundManager;
	if (!soundManager->init(hWnd))
		return FALSE;

	// ���� ������ �߰� (id�� 0���� ����)
	int id;
	if (!soundManager->add(L"./sound/background.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/Ʋ ���� �Ҹ�.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/�ð���.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/������ ���� �Ҹ�.wav", &id))
		return FALSE;
	if (!soundManager->add(L"./sound/���� ����.wav", &id))
		return FALSE;
	return true;
}

BOOL DemoApp::SetVolume(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lVolume) {
	if (!lpDSBuffer) return FALSE; 
	if (FAILED(lpDSBuffer->SetVolume(DSVOLUME_TO_DB(lVolume))))
		return FALSE;
	return TRUE;
}

