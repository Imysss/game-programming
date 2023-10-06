#include "hw3.h"

HRESULT DemoApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);

		//선형 계조 생성
		if (SUCCEEDED(hr))
		{
			ID2D1GradientStopCollection* pGradientStops = NULL;
			static const D2D1_GRADIENT_STOP stops[] =
			{
				{   0.f,  { 0.f, 1.f, 1.f, 0.25f }  },
				{   1.f,  { 0.f, 0.f, 1.f, 1.f }  },
			};
			hr = m_pRenderTarget->CreateGradientStopCollection(stops, ARRAYSIZE(stops), &pGradientStops);
			
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateLinearGradientBrush(
					D2D1::LinearGradientBrushProperties(D2D1::Point2F(100, 0), D2D1::Point2F(100, 200)),
					D2D1::BrushProperties(),
					pGradientStops,
					&m_pLinearGradientBrush
				);
				pGradientStops->Release();
			}
		}

		if (SUCCEEDED(hr))
		{
			// Create a solid color brush for writing text.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black),
				&m_pTextBrush);
		}

		if (SUCCEEDED(hr))
		{
			// Create a scene brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black, 1.f),
				&m_pSceneBrush);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.f), &m_pBlackBrush);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.f), &m_pWhiteBrush);
		}

		/* 배경 및 물체의 비트맵생성 */
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"BACKGROUND",
				L"PNG",
				size.width,
				size.height,
				&m_pBackgroundBitmap
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"CHOCO",
				L"PNG",
				100,
				150,
				&ingredientTypeBitmap[0]
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"STICK",
				L"PNG",
				150,
				100,
				&ingredientTypeBitmap[1]
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"DISH",
				L"PNG",
				400,
				400,
				&dish_Bitmap[0]
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"DISH1",
				L"PNG",
				400,
				400,
				&dish_Bitmap[1]
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"DISH2",
				L"PNG",
				400,
				400,
				&dish_Bitmap[2]
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"DISH3",
				L"PNG",
				400,
				400,
				&dish_Bitmap[3]
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"DISH4",
				L"PNG",
				400,
				400,
				&dish_Bitmap[4]
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"DISH5",
				L"PNG",
				400,
				400,
				&dish_Bitmap[5]
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"COOLINGOFF",
				L"PNG",
				700,
				130,
				&coolingOff_Bitmap
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"COOLINGOFF_ING",
				L"PNG",
				700,
				130,
				&coolingOff_ing_Bitmap
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"NOTCOVER_SHAPE",
				L"PNG",
				200,
				200,
				&notcover_shape_Bitmap
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"CHOCOHALF_SHAPE",
				L"PNG",
				200,
				200,
				&chocohalf_shape_Bitmap
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"CHOCOFULL_SHAPE",
				L"PNG",
				200,
				200,
				&chocofull_shape_Bitmap
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"CHOCOFULLSTICK_SHAPE",
				L"PNG",
				200,
				200,
				&chocofullStick_shape_Bitmap
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"CHOCOFULLSTICK_CHOCO",
				L"PNG",
				100,
				100,
				&chocofullStick_choco_Bitmap
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"MALPUNGSUN",
				L"PNG",
				300,
				220,
				&malpungsun_Bitmap
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"BOMB",
				L"PNG",
				50,
				50,
				&bomb_Bitmap
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = LoadResourceBitmap(
				m_pRenderTarget,
				m_pWICFactory,
				L"EXPLOSION",
				L"PNG",
				750,
				650,
				&explosion_Bitmap
			);
		}

		//배경 및 물체의 Brush 생성 
		if (SUCCEEDED(hr))
		{
			D2D1_BITMAP_BRUSH_PROPERTIES propertiesXClampYClamp = D2D1::BitmapBrushProperties(
				D2D1_EXTEND_MODE_CLAMP,
				D2D1_EXTEND_MODE_CLAMP,
				D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
			);

			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					m_pBackgroundBitmap,
					propertiesXClampYClamp,
					&m_pBackgroundBitmapBrush
				);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					ingredientTypeBitmap[0],
					&ingredientTypeBitmapBrush[0]
				);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					ingredientTypeBitmap[1],
					&ingredientTypeBitmapBrush[1]
				);
			}

			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					dish_Bitmap[0], propertiesXClampYClamp,
					&dishBitmapBrush[0]);
			}
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					dish_Bitmap[1], propertiesXClampYClamp,
					&dishBitmapBrush[1]);
			}
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					dish_Bitmap[2], propertiesXClampYClamp,
					&dishBitmapBrush[2]);
			}
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					dish_Bitmap[3], propertiesXClampYClamp,
					&dishBitmapBrush[3]);
			}
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					dish_Bitmap[4], propertiesXClampYClamp,
					&dishBitmapBrush[4]);
			}
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					dish_Bitmap[5], propertiesXClampYClamp,
					&dishBitmapBrush[5]);
			}

			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					coolingOff_Bitmap, propertiesXClampYClamp,
					&coolingOffBitmapBrush);
			}
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					coolingOff_ing_Bitmap, propertiesXClampYClamp,
					&coolingOff_ing_BitmapBrush);
			}
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					notcover_shape_Bitmap, propertiesXClampYClamp,
					&notcover_shape_BitmapBrush);
			}

			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					chocohalf_shape_Bitmap, propertiesXClampYClamp,
					&chocohalf_shape_BitmapBrush);
			}
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					chocofull_shape_Bitmap, propertiesXClampYClamp,
					&chocofull_shape_BitmapBrush);
			}
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					chocofullStick_shape_Bitmap, propertiesXClampYClamp,
					&chocofullstick_shape_BitmapBrush);
			}
			if (SUCCEEDED(hr)) {
				hr = m_pRenderTarget->CreateBitmapBrush(
					chocofullStick_choco_Bitmap, propertiesXClampYClamp,
					&chocofullstick_choco_BitmapBrush);
			}
			ID2D1GradientStopCollection* pGradientStops = NULL;

			static const D2D1_GRADIENT_STOP gradientStops[] =
			{
				{   0.f,  D2D1::ColorF(D2D1::ColorF::Black, 1.0f)  },
				{   1.f,  D2D1::ColorF(D2D1::ColorF::White, 0.0f)  },
			};

			hr = m_pRenderTarget->CreateGradientStopCollection(gradientStops, 2, &pGradientStops);
			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateRadialGradientBrush(
					D2D1::RadialGradientBrushProperties(D2D1::Point2F(50, 50), D2D1::Point2F(0, 0), 50, 50),
					pGradientStops,
					&ingredientTypeOpacityBrush[0]
				);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateRadialGradientBrush(
					D2D1::RadialGradientBrushProperties(D2D1::Point2F(50, 50), D2D1::Point2F(0, 0), 50, 50),
					pGradientStops,
					&ingredientTypeOpacityBrush[1]
				);
			}


			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					malpungsun_Bitmap,
					propertiesXClampYClamp,
					&malpungsun_BitmapBrush
				);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					bomb_Bitmap,
					propertiesXClampYClamp,
					&bomb_BitmapBrush
				);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pRenderTarget->CreateBitmapBrush(
					explosion_Bitmap,
					propertiesXClampYClamp,
					&explosion_BitmapBrush
				);
			}
		}
	}
	return hr;
}