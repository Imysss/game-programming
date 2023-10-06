#include "hw3.h"
#include "Character.h"
const int NUM_BOMB = 3;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			DemoApp app;

			if (SUCCEEDED(app.Initialize(hInstance)))
			{
				app.RunMessageLoop();
			}
		}
		CoUninitialize();
	}
	return 0;
}

DemoApp::DemoApp() :
	m_hwnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL),
	m_pWICFactory(NULL),

	//TEXT
	m_pTextBrush(NULL),
	m_pDWriteFactory(NULL),
	m_pTextFormat(NULL),
	m_AI_TextFormat(NULL),

	//BRUSH
	m_pSceneBrush(NULL),
	m_pStrokeStyleDash(NULL),

	//BITMAP
	m_pBackgroundBitmap(NULL),
	ingredientTypeBitmap(),
	dish_Bitmap(),
	coolingOff_Bitmap(NULL),
	coolingOff_ing_Bitmap(NULL),
	notcover_shape_Bitmap(NULL),
	chocohalf_shape_Bitmap(NULL),
	chocofull_shape_Bitmap(NULL),
	chocofullStick_shape_Bitmap(NULL),
	chocofullStick_choco_Bitmap(NULL),
	malpungsun_Bitmap(NULL),
	bomb_Bitmap(NULL),
	explosion_Bitmap(NULL),

	//BITMAP BRUSH
	m_pBackgroundBitmapBrush(NULL),
	ingredientTypeBitmapBrush(),
	ingredientTypeOpacityBrush(),
	dishBitmapBrush(),
	coolingOffBitmapBrush(NULL),
	coolingOff_ing_BitmapBrush(NULL),
	notcover_shape_BitmapBrush(NULL),
	chocohalf_shape_BitmapBrush(NULL),
	chocofull_shape_BitmapBrush(NULL),
	chocofullstick_shape_BitmapBrush(NULL),
	chocofullstick_choco_BitmapBrush(NULL),
	malpungsun_BitmapBrush(NULL),
	bomb_BitmapBrush(NULL),
	explosion_BitmapBrush(NULL),

	//PATH GEOMETRY & ANIMATION
	dish_PathGeometry(NULL),
	malpungsun_Geometry(NULL),
	m_pRectGeo(NULL),
	m_Animation(),
	m_Animation2()
{
}

DemoApp::~DemoApp()
{
	SAFE_RELEASE(m_pDirect2dFactory);
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pWICFactory);

	//TEXT
	SAFE_RELEASE(m_pTextBrush);
	SAFE_RELEASE(m_pDWriteFactory);
	SAFE_RELEASE(m_pTextFormat);
	SAFE_RELEASE(m_AI_TextFormat);

	//BRUSH
	SAFE_RELEASE(m_pSceneBrush);
	SAFE_RELEASE(m_pStrokeStyleDash);

	//BITMAP
	SAFE_RELEASE(m_pBackgroundBitmap);
	SAFE_RELEASE(coolingOff_Bitmap);
	SAFE_RELEASE(coolingOff_ing_Bitmap);
	SAFE_RELEASE(notcover_shape_Bitmap);
	SAFE_RELEASE(chocohalf_shape_Bitmap);
	SAFE_RELEASE(chocofull_shape_Bitmap);
	SAFE_RELEASE(chocofullStick_shape_Bitmap);
	SAFE_RELEASE(chocofullStick_choco_Bitmap);
	SAFE_RELEASE(malpungsun_Bitmap);
	SAFE_RELEASE(bomb_Bitmap);
	SAFE_RELEASE(explosion_Bitmap);

	//BITMAP BRUSH
	SAFE_RELEASE(m_pBackgroundBitmapBrush);
	SAFE_RELEASE(coolingOffBitmapBrush);
	SAFE_RELEASE(coolingOff_ing_BitmapBrush);
	SAFE_RELEASE(notcover_shape_BitmapBrush);
	SAFE_RELEASE(chocohalf_shape_BitmapBrush);
	SAFE_RELEASE(chocofull_shape_BitmapBrush);
	SAFE_RELEASE(chocofullstick_shape_BitmapBrush);
	SAFE_RELEASE(chocofullstick_choco_BitmapBrush);
	SAFE_RELEASE(malpungsun_BitmapBrush);
	SAFE_RELEASE(bomb_BitmapBrush);
	SAFE_RELEASE(explosion_BitmapBrush);

	//PATH GEOMETRY & ANIMATION
	SAFE_RELEASE(dish_PathGeometry);
	SAFE_RELEASE(malpungsun_Geometry);
	SAFE_RELEASE(m_pRectGeo);
}

//폭탄 위치 임의로 설정
void DemoApp::setRandomBombPosition()
{
	int posX[NUM_BOMB], posY[NUM_BOMB], cX, cY;
	int k = 0;
	bool duplicated;
	for (int i = 0; i < NUM_BOMB; i++)
	{
		do
		{
			cX = rand() % PLANE_SIZE_X;
			cY = rand() % PLANE_SIZE_Y;
			duplicated = false;
			if (cX < 0 || cY < 0 || cX>1400 || cY>800)
				duplicated = true;
			
			if (i != 0)
			{
				for (int j = 0; j < i; j++)
				{
					if (sqrt((float)(cX - posX[i]) * (cX - posX[i]) + (float)(cY - posY[i]) * (cY - posY[i]) < 10))
					{
						duplicated = true;
						break;
					}
				}
			}
		} while (duplicated);
		posX[k] = cX;
		posY[k] = cY;
		k++;
	}

	for (int i = 0; i < character.size(); i++)
	{
		character[i]->setPosition(D2D1::Point2F((float)posX[i], (float)posY[i]));
	}
}

HRESULT DemoApp::Initialize(HINSTANCE hInstance)
{
	HRESULT hr;

	hr = CreateDeviceIndependentResources();
	if (SUCCEEDED(hr))
	{
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = DemoApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = hInstance;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.lpszClassName = L"HW3_201701589(임예슬)";
		RegisterClassEx(&wcex);

		m_hwnd = CreateWindow(
			L"HW3_201701589(임예슬)", L" HW3_201701589(임예슬)",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			1400, 800, NULL, NULL, hInstance, this
		);
		hr = m_hwnd ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			addSound(m_hwnd);
			soundManager->play(0, TRUE);
			SetVolume(soundManager->pDSBPrimary, 70);
		}

		if (SUCCEEDED(hr))
		{
			for (int i = 0; i < NUM_BOMB; i++)
			{
				DWORD enemyStateTransitions[][3] =
				{
					{ Character::STATE_STAND, Character::EVENT_FINDTARGET, Character::STATE_FOLLOW },
					{ Character::STATE_STAND, Character::EVENT_BEATTACKED, Character::STATE_ATTACK },
					{ Character::STATE_STAND, Character::EVENT_DUBIOUS, Character::STATE_MOVE },
					{ Character::STATE_MOVE, Character::EVENT_FINDTARGET, Character::STATE_FOLLOW },
					{ Character::STATE_MOVE, Character::EVENT_STOPWALK, Character::STATE_STAND },
					{ Character::STATE_ATTACK, Character::EVENT_LOSTTARGET, Character::STATE_STAND },
					{ Character::STATE_ATTACK, Character::EVENT_HEALTHDRAINED, Character::STATE_RUNAWAY },
					{ Character::STATE_ATTACK, Character::EVENT_OUTOFATTACK, Character::STATE_FOLLOW },
					{ Character::STATE_FOLLOW, Character::EVENT_WITHINATTACK, Character::STATE_ATTACK },
					{ Character::STATE_FOLLOW, Character::EVENT_LOSTTARGET, Character::STATE_STAND },
					{ Character::STATE_RUNAWAY, Character::EVENT_LOSTTARGET, Character::STATE_STAND },
				};
				Character* bomb = new Character(Character::TYPE_AI, enemyStateTransitions, 11);
				character.push_back(bomb);
			}
			setRandomBombPosition();

			DWORD myStateTransition[][3] =
			{
				{Character::STATE_MOVE, Character::EVENT_UNDEFINED, Character::STATE_MOVE }
			};
			myCharacter = new Character(Character::TYPE_HUMAN, myStateTransition, 1);
		}
	}

	if (SUCCEEDED(hr))
	{
		ShowWindow(m_hwnd, SW_SHOWNORMAL);
		UpdateWindow(m_hwnd);
	}
	return hr;
}

HRESULT DemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	ID2D1GeometrySink* pSink = NULL;

	// Create a WIC factory.
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	if (SUCCEEDED(hr))
	{
		// Create a shared DirectWrite factory
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
		);
	}
	if (SUCCEEDED(hr))
	{
		hr = m_pDirect2dFactory->CreateRectangleGeometry(D2D1::RectF(0, 0, 100, 100), &m_pRectGeo);
	}

	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Verdana",     // The font family name.
			NULL,           // The font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			30.0f,
			L"en-us",
			&m_pTextFormat
		);
	}
	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Verdana",     // The font family name.
			NULL,           // The font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			25.0f,
			L"en-us",
			&m_money_TextFormat
		);
	}
	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Verdana",     // The font family name.
			NULL,           // The font collection (NULL sets it to use the system font collection).
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20.0f,
			L"en-us",
			&m_AI_TextFormat
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDirect2dFactory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_ROUND,
				D2D1_LINE_JOIN_MITER,
				10.0f,
				D2D1_DASH_STYLE_DASH,
				0.0f),
			NULL,
			0,
			&m_pStrokeStyleDash
		);
	}
	//경로를 그리는 기하
	if (SUCCEEDED(hr))
	{
		hr = m_pDirect2dFactory->CreatePathGeometry(&malpungsun_Geometry);
		if (SUCCEEDED(hr))
		{
			hr = malpungsun_Geometry->Open(&pSink);

			if (SUCCEEDED(hr))
			{
				pSink->BeginFigure(
					D2D1::Point2F(900, 220),
					D2D1_FIGURE_BEGIN_FILLED
				);

				pSink->AddArc(D2D1::ArcSegment(
					D2D1::Point2F(1200, 220),
					D2D1::SizeF(150, 100),
					0.f,
					D2D1_SWEEP_DIRECTION_CLOCKWISE,
					D2D1_ARC_SIZE_LARGE));
				pSink->AddQuadraticBezier(D2D1::QuadraticBezierSegment(D2D1::Point2F(1200, 250),
					D2D1::Point2F(1150, 270)));
				pSink->AddLine(D2D1::Point2F(1200, 320));
				pSink->AddLine(D2D1::Point2F(1120, 280));
				pSink->AddQuadraticBezier(D2D1::QuadraticBezierSegment(D2D1::Point2F(910, 300),
					D2D1::Point2F(900, 220)));
				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
				hr = pSink->Close();
			}
			SAFE_RELEASE(pSink);
		}
	}

	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < 5; i++)
		{
			hr = m_pDirect2dFactory->CreatePathGeometry(&pViewRange[i]);
			hr = m_pDirect2dFactory->CreatePathGeometry(&pAttackRange[i]);

			//View Range
			if (SUCCEEDED(hr))
			{
				hr = pViewRange[i]->Open(&pSink);
			}
			if (SUCCEEDED(hr))
			{
				D2D1_POINT_2F StartPos = D2D1::Point2F(-60, 0);
				pSink->BeginFigure(StartPos, D2D1_FIGURE_BEGIN_FILLED);

				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(60, 0), D2D1::SizeF(60, 60), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(-60, 0), D2D1::SizeF(60, 60), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

				hr = pSink->Close();
			}
			SAFE_RELEASE(pSink);

			//Attack Range
			if (SUCCEEDED(hr))
			{
				hr = pAttackRange[i]->Open(&pSink);
			}
			if (SUCCEEDED(hr))
			{
				D2D1_POINT_2F StartPos = D2D1::Point2F(-30, 0);
				pSink->BeginFigure(StartPos, D2D1_FIGURE_BEGIN_FILLED);

				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(30, 0), D2D1::SizeF(30, 30), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(-30, 0), D2D1::SizeF(30, 30), 0, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));

				pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

				hr = pSink->Close();
			}
			SAFE_RELEASE(pSink);
		}
	}
	return hr;
}

void DemoApp::DiscardDeviceResources()
{
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pTextBrush);
	SAFE_RELEASE(m_pSceneBrush);;
}

void DemoApp::RunMessageLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void DemoApp::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

HRESULT DemoApp::LoadResourceBitmap(
	ID2D1RenderTarget* pRenderTarget,
	IWICImagingFactory* pIWICFactory,
	PCWSTR resourceName,
	PCWSTR resourceType,
	UINT destinationWidth,
	UINT destinationHeight,
	ID2D1Bitmap** ppBitmap
)
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void* pImageFile = NULL;
	DWORD imageFileSize = 0;

	// Locate the resource.
	imageResHandle = FindResourceW(HINST_THISCOMPONENT, resourceName, resourceType);
	HRESULT hr = imageResHandle ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		// Load the resource.
		imageResDataHandle = LoadResource(HINST_THISCOMPONENT, imageResHandle);
		hr = imageResDataHandle ? S_OK : E_FAIL;
	}
	if (SUCCEEDED(hr))
	{
		// Lock it to get a system memory pointer.
		pImageFile = LockResource(imageResDataHandle);
		hr = pImageFile ? S_OK : E_FAIL;
	}
	if (SUCCEEDED(hr))
	{
		// Calculate the size.
		imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);
		hr = imageFileSize ? S_OK : E_FAIL;

	}
	if (SUCCEEDED(hr))
	{
		// Create a WIC stream to map onto the memory.
		hr = pIWICFactory->CreateStream(&pStream);
	}
	if (SUCCEEDED(hr))
	{
		// Initialize the stream with the memory pointer and size.
		hr = pStream->InitializeFromMemory(reinterpret_cast<BYTE*>(pImageFile), imageFileSize);
	}
	if (SUCCEEDED(hr))
	{
		// Create a decoder for the stream.
		hr = pIWICFactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder);
	}
	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr))
	{
		// Convert the image format to 32bppPBGRA (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}
	if (SUCCEEDED(hr))
	{
		// If a new width or height was specified, create an IWICBitmapScaler and use it to resize the image.
		if (destinationWidth != 0 || destinationHeight != 0)
		{
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
					destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
					destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
				}

				hr = pIWICFactory->CreateBitmapScaler(&pScaler);
				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(pSource, destinationWidth, destinationHeight, WICBitmapInterpolationModeCubic);
					if (SUCCEEDED(hr))
					{
						hr = pConverter->Initialize(pScaler, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
					}
				}
			}
		}
		else
		{
			hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
		}
	}
	if (SUCCEEDED(hr))
	{
		//create a Direct2D bitmap from the WIC bitmap.
		hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);
	}

	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pSource);
	SAFE_RELEASE(pStream);
	SAFE_RELEASE(pConverter);
	SAFE_RELEASE(pScaler);

	return hr;
}
