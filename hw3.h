#pragma once

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

// DX Header Files:
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <dwmapi.h>
#include <string>
#include "Animation.h"
#include "SoundManager.h"
#include "Character.h"

// 자원 안전 반환 매크로.
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

// 현재 모듈의 시작주소 얻기.
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}
#else
#define Assert(b)
#endif
#endif

class DemoApp
{
public:
	DemoApp();
	~DemoApp();
	HRESULT Initialize(HINSTANCE hInstance);
	void RunMessageLoop();

private:
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	HRESULT OnRender();
	void OnResize(UINT width, UINT height);
	void WriteActionInfo();
	HRESULT LoadResourceBitmap(
		ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR resourceName,
		PCWSTR resourceType,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap
	);

	//사운드 관련
	void soundOn(DemoApp* pDemoApp, int i, bool trueorfalse);
	BOOL SetVolume(LPDIRECTSOUNDBUFFER lpDSBuffer, LONG lVolume);
	BOOL addSound(HWND hWnd);

	//초기화 관련
	void clearFlag();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;

	//Color Brush
	ID2D1SolidColorBrush* m_pSceneBrush;
	ID2D1SolidColorBrush* m_pTextBrush;
	ID2D1StrokeStyle* m_pStrokeStyleDash;
	ID2D1SolidColorBrush* m_pBlackBrush;
	ID2D1SolidColorBrush* m_pWhiteBrush;

	//DWrite
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	IDWriteTextFormat* m_money_TextFormat;
	IDWriteTextFormat* m_AI_TextFormat;

	//WIC and Bitmaps
	IWICImagingFactory* m_pWICFactory;
	ID2D1Bitmap* m_pBackgroundBitmap;
	ID2D1Bitmap* ingredientTypeBitmap[2];
	ID2D1Bitmap* dish_Bitmap[6]; //접시
	ID2D1Bitmap* coolingOff_Bitmap; //냉각기 (냉각 전, 냉각 후)
	ID2D1Bitmap* coolingOff_ing_Bitmap; //냉각 중인 냉각기

	ID2D1Bitmap* notcover_shape_Bitmap; //아무것도 넣지 않은 틀
	ID2D1Bitmap* chocohalf_shape_Bitmap; //초코 반만 넣은 틀 모양
	ID2D1Bitmap* chocofull_shape_Bitmap; //초코 전부 넣은 틀 모양
	ID2D1Bitmap* chocofullStick_shape_Bitmap; //초코+비스킷 넣은 틀 모양
	ID2D1Bitmap* chocofullStick_choco_Bitmap; //초코 완성된 모양

	ID2D1Bitmap* malpungsun_Bitmap; //말풍선

	ID2D1Bitmap* bomb_Bitmap; //폭탄
	ID2D1Bitmap* explosion_Bitmap; //폭탄 터지는 것

	//Bitmap Brushes
	ID2D1BitmapBrush* m_pBackgroundBitmapBrush;
	ID2D1BitmapBrush* ingredientTypeBitmapBrush[2];
	ID2D1RadialGradientBrush* ingredientTypeOpacityBrush[2];
	ID2D1BitmapBrush* dishBitmapBrush[6]; //접시
	ID2D1BitmapBrush* coolingOffBitmapBrush; //냉각기 (냉각 전, 냉각 후)

	ID2D1BitmapBrush* coolingOff_ing_BitmapBrush; //냉각 중인 냉각기

	ID2D1BitmapBrush* notcover_shape_BitmapBrush; //아무것도 넣지 않은 틀
	ID2D1BitmapBrush* chocohalf_shape_BitmapBrush; //초코 반만 넣은 틀
	ID2D1BitmapBrush* chocofull_shape_BitmapBrush; //초코 전부 넣은 틀
	ID2D1BitmapBrush* chocofullstick_shape_BitmapBrush; //초코+비스캣 넣은 틀
	ID2D1BitmapBrush* chocofullstick_choco_BitmapBrush; //완성된 초코

	ID2D1BitmapBrush* malpungsun_BitmapBrush; //말풍선

	ID2D1BitmapBrush* bomb_BitmapBrush; //폭탄
	ID2D1BitmapBrush* explosion_BitmapBrush; //폭탄 터지는 것

	//Geometry
	ID2D1PathGeometry* dish_PathGeometry;
	ID2D1PathGeometry* malpungsun_Geometry;

	ID2D1RectangleGeometry* m_pRectGeo;

	//LinearGradientBrush
	ID2D1LinearGradientBrush* m_pLinearGradientBrush;

	//Animation
	LinearAnimation<float> m_Animation;
	LinearAnimation<float> m_Animation2;

	LARGE_INTEGER m_nPrevTime;
	LARGE_INTEGER m_nFrequency;
	DWM_TIMING_INFO m_Character1_DwmTimingInfo;
	
	//Sound 변수
	LPDIRECTSOUNDBUFFER g_lpDSBG[2] = { NULL };
	CSoundManager* soundManager;

	//AI
	void setRandomBombPosition();
	void drawAICharacter(bool isAIShow);
	std::vector<Character*> character;
	Character* myCharacter;

	//AI Geometry
	ID2D1PathGeometry* pViewRange[5];
	ID2D1PathGeometry* pAttackRange[5];
};

