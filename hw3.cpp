#include "hw3.h"
#include <time.h>
#include <string>
using namespace D2D1;

//������ â ũ��
FLOAT winWidth;
FLOAT winHeight;

//ȭ�� ����
WCHAR input_str;
bool isShow = false;
bool isAIShow = false;

//���� ���콺 ��ġ
D2D_POINT_2F currentMousePosition;

//�� ���� ���� ����, ��Ĺ�� ��ġ
D2D1_RECT_F ingredientRectangle[2] = { D2D1::RectF(286, 41, 320, 148), D2D1::RectF(591, 62, 663, 100) };
D2D1_RECT_F chocoShape[6] = {
	D2D1::RectF(159, 353, 236, 419), D2D1::RectF(400, 347, 475, 420), D2D1::RectF(640, 353, 719, 420),
	D2D1::RectF(137, 514, 221, 591), D2D1::RectF(400, 514, 481, 591), D2D1::RectF(661, 514, 741, 591) };
D2D1_RECT_F coolingOffRectangle = D2D1::RectF(153, 667, 723, 733);
D2D1_RECT_F dishRectangle = D2D1::RectF(889, 375, 1217, 691);

//���� ������
D2D1_POINT_2F dishMove = { 0, 0 };

//ĳ���� ����
int chocoCnt = 0;
int money = 0;

//flag
BOOL isOrdering = false; BOOL isCooling = false;
BOOL isCoolingOff = false; BOOL isDish = false; BOOL isChoco = false; BOOL isStick = false;
BOOL isChocoNULL0 = true;  BOOL isChocoHalf0 = false;  BOOL isChocoFull0 = false; BOOL isChocoOk0 = false; BOOL isChocoCool0 = false; 
BOOL isChocoNULL1 = true;  BOOL isChocoHalf1 = false;  BOOL isChocoFull1 = false; BOOL isChocoOk1 = false; BOOL isChocoCool1 = false;
BOOL isChocoNULL2 = true;  BOOL isChocoHalf2 = false;  BOOL isChocoFull2 = false; BOOL isChocoOk2 = false; BOOL isChocoCool2 = false;
BOOL isChocoNULL3 = true;  BOOL isChocoHalf3 = false;  BOOL isChocoFull3 = false; BOOL isChocoOk3 = false; BOOL isChocoCool3 = false;
BOOL isChocoNULL4 = true;  BOOL isChocoHalf4 = false;  BOOL isChocoFull4 = false; BOOL isChocoOk4 = false; BOOL isChocoCool4 = false;
BOOL isChocoNULL5 = true;  BOOL isChocoHalf5 = false;  BOOL isChocoFull5 = false; BOOL isChocoOk5 = false; BOOL isChocoCool5 = false;

//flag �ʱ�ȭ�ϴ� �Լ�
void DemoApp::clearFlag()
{
	isChocoNULL0 = true; isChocoHalf0 = false; isChocoFull0 = false; isChocoOk0 = false; isChocoCool0 = false;
	isChocoNULL1 = true; isChocoHalf1 = false; isChocoFull1 = false; isChocoOk1 = false; isChocoCool1 = false;
	isChocoNULL2 = true; isChocoHalf2 = false; isChocoFull2 = false; isChocoOk2 = false; isChocoCool2 = false;
	isChocoNULL3 = true; isChocoHalf3 = false; isChocoFull3 = false; isChocoOk3 = false; isChocoCool3 = false;
	isChocoNULL4 = true; isChocoHalf4 = false; isChocoFull4 = false; isChocoOk4 = false; isChocoCool4 = false;
	isChocoNULL5 = true; isChocoHalf5 = false; isChocoFull5 = false; isChocoOk5 = false; isChocoCool5 = false;
	isCoolingOff = false; isCooling = false;
}

//������
D2D1_SIZE_F rtSize;
D2D1_SIZE_U ch0Size, ch1Size;

//�ð�
time_t cooling_time;
clock_t lastClientComeTime;

enum ingredient {
	Choco = 0,
	Stick = 1,
	nothing = 2
};
std::string getIngredientName[2] = { "����", "��Ĺ" };
ingredient currentIngredient = nothing;
int orderingChocoCount = 0;

//���콺 Ŭ�� Ȯ��
BOOL mouseClickCheck = false;

enum soundIndex {
	backgroundSound=0,
	addIngredientSound=1,
	coolingSound=2,
	itemClickSound=3,
	coinSound=4,
	explosionSound=5
};
bool isMute = false;
int currentSound = 70;

//���콺 Ŀ�� ��ġ Ȯ��
boolean isMouseIn(D2D1_POINT_2F inputMouse, D2D1_RECT_F candidateRect)
{
	if (inputMouse.x >= candidateRect.left && inputMouse.x <= candidateRect.right && inputMouse.y >= candidateRect.top && inputMouse.y <= candidateRect.bottom)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//���� ���
D2D1_RECT_F getRect(int width, int height) {
	return RectF(0, 0, width, height);
}

//���콺 Ŀ�� ��ġ, ���� ��ġ�� ũ�� ����
void DemoApp::WriteActionInfo()
{
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	// debug
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	WCHAR szText[250];
	swprintf_s(szText, L"���콺x : %1.f\n���콺y : %1.f\n��� ����: %d\n", currentMousePosition.x, currentMousePosition.y, currentSound);
	m_pRenderTarget->DrawText(
		szText,
		wcslen(szText),
		m_pTextFormat,
		D2D1::RectF(10.0f, 100.0f, 400.0f, 240.0f),
		m_pTextBrush
	);

	swprintf_s(szText, L"������ �ϴ� ���� ����: %d", orderingChocoCount - chocoCnt);
	m_pRenderTarget->DrawText(
		szText,
		wcslen(szText),
		m_money_TextFormat,
		D2D1::RectF(1000.0f, 15.0f, 1400.f, 80.f),
		m_pTextBrush
	);

	swprintf_s(szText, L"�ݾ�: %d", money);
	m_pRenderTarget->DrawText(
		szText,
		wcslen(szText),
		m_money_TextFormat,
		D2D1::RectF(1000.0f, 60.0f, 1400.f, 200.f),
		m_pTextBrush
	);

	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void DemoApp::soundOn(DemoApp* pDemoApp, int i, bool trueorfalse) {
	if (isMute == false) {
		pDemoApp->soundManager->play(i, trueorfalse);
	}
}

HRESULT DemoApp::OnRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		rtSize = m_pRenderTarget->GetSize();
		ch0Size, ch1Size;

		// Get ready to draw.
		m_pRenderTarget->BeginDraw();

		// Reset to the identity transform.
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		// Clear the render target contents.
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		//D2D1_RECT_F rcBrushRect = D2D1::RectF(0, 0, rtSize.width, rtSize.height);
		D2D1_RECT_F rcBrushRect = D2D1::RectF(0, 0, 100, 100);
		D2D1_RECT_F rcBrushRect2 = RectF(0, 0, 200, 200);


		//��Ʈ�� ��� �׸���
		m_pRenderTarget->FillRectangle(&RectF(0, 0, rtSize.width, rtSize.height), m_pBackgroundBitmapBrush);

		//��ǳ�� �׸���
		m_pRenderTarget->DrawGeometry(malpungsun_Geometry, m_pBlackBrush, 5);
		m_pRenderTarget->FillGeometry(malpungsun_Geometry, m_pWhiteBrush);

		//��� �׸���
		//1. ���� (���)
		D2D1::Matrix3x2F ingredient_choco;
		ingredient_choco = D2D1::Matrix3x2F::Translation(250, 20);
		m_pRenderTarget->SetTransform(ingredient_choco);
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		m_pRenderTarget->FillGeometry(m_pRectGeo, ingredientTypeBitmapBrush[0], ingredientTypeOpacityBrush[0]);
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		//m_pRenderTarget->FillRectangle(&RectF(0, 0, 100, 150), ingredientTypeBitmapBrush[0]);

		//2. ��Ĺ
		D2D1::Matrix3x2F ingredient_stick;
		ingredient_stick = D2D1::Matrix3x2F::Translation(550, 30);
		m_pRenderTarget->SetTransform(ingredient_stick);
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		m_pRenderTarget->FillGeometry(m_pRectGeo, ingredientTypeBitmapBrush[1], ingredientTypeOpacityBrush[1]);
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		//m_pRenderTarget->FillRectangle(&RectF(0, 0, 150, 100), ingredientTypeBitmapBrush[1]);

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(170, 450) * D2D1::Matrix3x2F::Scale(2, 1, D2D1::Point2F(0, 0)));


		//�ֹ� �ޱ�
		if (!isOrdering) {
			lastClientComeTime = clock();
			srand(time(NULL));
			orderingChocoCount = rand() % 5 + 1;
			isOrdering = true;
		}
		if ((clock() - lastClientComeTime) / CLOCKS_PER_SEC < 2)
		{
			m_pRenderTarget->SetTransform(Matrix3x2F::Translation((double)(clock() - lastClientComeTime) / CLOCKS_PER_SEC * 2, 350));
			m_pRenderTarget->SetTransform(Matrix3x2F::Translation(1400, 350));
			m_pRenderTarget->FillRectangle(&RectF(0, 0, 400, 400), dishBitmapBrush[0]);
			isChocoNULL0 = true;
		}
		else
		{
			m_pRenderTarget->SetTransform(Matrix3x2F::Translation(850, 350));
			if (chocoCnt == 1)
				m_pRenderTarget->FillRectangle(&RectF(0, 0, 400, 400), dishBitmapBrush[1]);
			else if (chocoCnt == 2)
				m_pRenderTarget->FillRectangle(&RectF(0, 0, 400, 400), dishBitmapBrush[2]);
			else if (chocoCnt == 3)
				m_pRenderTarget->FillRectangle(&RectF(0, 0, 400, 400), dishBitmapBrush[3]);
			else if (chocoCnt == 4)
				m_pRenderTarget->FillRectangle(&RectF(0, 0, 400, 400), dishBitmapBrush[4]);
			else if (chocoCnt == 5)
				m_pRenderTarget->FillRectangle(&RectF(0, 0, 400, 400), dishBitmapBrush[5]);
			else if (chocoCnt == 0)
			{
				m_pRenderTarget->FillRectangle(&RectF(0, 0, 400, 400), dishBitmapBrush[0]);
			}
		}

		//��ǳ�� ���� �ؽ�Ʈ �׸���
		m_pRenderTarget->SetTransform(Matrix3x2F::Translation(900, 120));
		WCHAR szText[250];
		swprintf_s(szText, L" %d�� �ּ���", orderingChocoCount);
		m_pRenderTarget->DrawText(
			szText,
			wcslen(szText),
			m_pTextFormat,
			RectF(70.f, 50.0f, 500.0f, 400.0f),
			m_pTextBrush
		);

	
		//Ʋ �׸���
		//Ʋ 1
		D2D1::Matrix3x2F shape1;
		shape1 = D2D1::Matrix3x2F::Translation(100, 300);
		m_pRenderTarget->SetTransform(shape1);
		m_pRenderTarget->FillRectangle(&rcBrushRect2, notcover_shape_BitmapBrush);

		//��Ʈ�� �귯�� ����
		if (isChocoOk0)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocofullstick_shape_BitmapBrush);
		else if (isChocoFull0)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocofull_shape_BitmapBrush);
		else if (isChocoHalf0)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocohalf_shape_BitmapBrush);
		else if (isChocoNULL0)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, notcover_shape_BitmapBrush);
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		//Ʋ 2
		D2D1::Matrix3x2F shape2;
		shape2 = D2D1::Matrix3x2F::Translation(340, 300);
		m_pRenderTarget->SetTransform(shape2);
		m_pRenderTarget->FillRectangle(&rcBrushRect2, notcover_shape_BitmapBrush);

		if (isChocoOk1)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocofullstick_shape_BitmapBrush);
		else if (isChocoFull1)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocofull_shape_BitmapBrush);
		else if (isChocoHalf1)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocohalf_shape_BitmapBrush);
		else if (isChocoNULL1)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, notcover_shape_BitmapBrush);
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		//Ʋ 3
		D2D1::Matrix3x2F shape3;
		shape3 = D2D1::Matrix3x2F::Translation(580, 300);
		m_pRenderTarget->SetTransform(shape3);
		m_pRenderTarget->FillRectangle(&rcBrushRect2, notcover_shape_BitmapBrush);

		if (isChocoOk2)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocofullstick_shape_BitmapBrush);
		else if (isChocoFull2)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocofull_shape_BitmapBrush);
		else if (isChocoHalf2)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocohalf_shape_BitmapBrush);
		else if (isChocoNULL2)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, notcover_shape_BitmapBrush);
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		//Ʋ 4
		D2D1::Matrix3x2F shape4;
		shape4 = D2D1::Matrix3x2F::Translation(80, 470);
		m_pRenderTarget->SetTransform(shape4);
		m_pRenderTarget->FillRectangle(&rcBrushRect2, notcover_shape_BitmapBrush);

		if (isChocoOk3)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocofullstick_shape_BitmapBrush);
		else if (isChocoFull3)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocofull_shape_BitmapBrush);
		else if (isChocoHalf3)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocohalf_shape_BitmapBrush);
		else if (isChocoNULL3)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, notcover_shape_BitmapBrush);
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		//Ʋ 5
		D2D1::Matrix3x2F shape5;
		shape5 = D2D1::Matrix3x2F::Translation(340, 470);
		m_pRenderTarget->SetTransform(shape5);
		m_pRenderTarget->FillRectangle(&rcBrushRect2, notcover_shape_BitmapBrush);

		if (isChocoOk4)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocofullstick_shape_BitmapBrush);
		else if (isChocoFull4)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocofull_shape_BitmapBrush);
		else if (isChocoHalf4)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocohalf_shape_BitmapBrush);
		else if (isChocoNULL4)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, notcover_shape_BitmapBrush);
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		//Ʋ 6
		D2D1::Matrix3x2F shape6;
		shape6 = D2D1::Matrix3x2F::Translation(580, 470);
		m_pRenderTarget->SetTransform(shape6);
		m_pRenderTarget->FillRectangle(&rcBrushRect2, notcover_shape_BitmapBrush);

		if (isChocoOk5)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocofullstick_shape_BitmapBrush);
		else if (isChocoFull5)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocofull_shape_BitmapBrush);
		else if (isChocoHalf5)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, chocohalf_shape_BitmapBrush);
		else if (isChocoNULL5)
			m_pRenderTarget->FillRectangle(&rcBrushRect2, notcover_shape_BitmapBrush);
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		//�ð��� �׸���
		D2D1::Matrix3x2F coolingoff;
		coolingoff = D2D1::Matrix3x2F::Translation(100, 640);
		m_pRenderTarget->SetTransform(coolingoff);
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		m_pRenderTarget->FillGeometry(m_pRectGeo, coolingOffBitmapBrush);
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

		//6�� �̻� �ð��Ⱑ �������� �� IsCooling=true�� ����
		if (isCoolingOff && time(NULL) - cooling_time >= 6) {
			isCooling = true;
			isCoolingOff = false;
		}
		if (isCoolingOff)
			m_pRenderTarget->FillRectangle(&getRect(700, 130), coolingOff_ing_BitmapBrush);
		else
			m_pRenderTarget->FillRectangle(&getRect(700, 130), coolingOffBitmapBrush);

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		//AI �׸���
		drawAICharacter(isAIShow);

		//��� ���콺 ��θ� ���� �׸��� 
		if (isChoco) {
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(currentMousePosition.x, currentMousePosition.y));
			m_pRenderTarget->FillRectangle(&RectF(0, 0, 100, 150), ingredientTypeBitmapBrush[0]);
		}
		else if (isStick) {
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(currentMousePosition.x, currentMousePosition.y));
			m_pRenderTarget->FillRectangle(&RectF(0, 0, 150, 100), ingredientTypeBitmapBrush[1]);
		}
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		//�ϼ��� ���� ���콺 ��θ� ���� �׸���
		if (isChocoCool0)
		{
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(currentMousePosition.x, currentMousePosition.y));
			m_pRenderTarget->FillRectangle(&RectF(0, 0, 100, 100), chocofullstick_choco_BitmapBrush);
		}
		else if (isChocoCool1)
		{
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(currentMousePosition.x, currentMousePosition.y));
			m_pRenderTarget->FillRectangle(&RectF(0, 0, 100, 100), chocofullstick_choco_BitmapBrush);
		}
		else if (isChocoCool2)
		{
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(currentMousePosition.x, currentMousePosition.y));
			m_pRenderTarget->FillRectangle(&RectF(0, 0, 100, 100), chocofullstick_choco_BitmapBrush);
		}
		else if (isChocoCool3)
		{
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(currentMousePosition.x, currentMousePosition.y));
			m_pRenderTarget->FillRectangle(&RectF(0, 0, 100, 100), chocofullstick_choco_BitmapBrush);
		}
		else if (isChocoCool4)
		{
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(currentMousePosition.x, currentMousePosition.y));
			m_pRenderTarget->FillRectangle(&RectF(0, 0, 100, 100), chocofullstick_choco_BitmapBrush);
		}
		else if (isChocoCool5)
		{
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(currentMousePosition.x, currentMousePosition.y));
			m_pRenderTarget->FillRectangle(&RectF(0, 0, 100, 100), chocofullstick_choco_BitmapBrush);
		}
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		WriteActionInfo(); // Write mouse cursor position, rectangle rotation & resize, etc...

		hr = m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

// Handles window messages.
LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		DemoApp* pDemoApp = (DemoApp*)pcs->lpCreateParams;

		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(pDemoApp));

		result = 1;
	}
	else
	{
		DemoApp* pDemoApp = reinterpret_cast<DemoApp*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));

		bool wasHandled = false;

		if (pDemoApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			// ������ â ũ�� ��ȯ ����
			case WM_GETMINMAXINFO:
			{
				MINMAXINFO* mmi = (MINMAXINFO*)lParam;
				mmi->ptMinTrackSize.x = 1400;
				mmi->ptMinTrackSize.y = 800;
				mmi->ptMaxTrackSize.x = 1400;
				mmi->ptMaxTrackSize.y = 800;
			}
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_LBUTTONUP:
			{
				if (isChoco || isStick) {
					if (isChoco)
						currentIngredient = Choco;
					else if (isStick)
						currentIngredient = Stick;
					pDemoApp->soundOn(pDemoApp, itemClickSound, FALSE);
				}


				cooling_time = time(NULL);
				//isChoco = false; isStick = false;
				//isKettle = false; isnambi = false;
				InvalidateRect(hwnd, NULL, FALSE);
				break;
			}
			case WM_LBUTTONDOWN:
			{
				currentMousePosition.x = LOWORD(lParam);
				currentMousePosition.y = HIWORD(lParam);

				//��� ����
				if (isMouseIn(currentMousePosition, ingredientRectangle[0]))
				{
					isChoco = !isChoco;
				}
				else if (isMouseIn(currentMousePosition, ingredientRectangle[1]))
				{
					isStick = !isStick;
				}

				//Ʋ 1
				if (isChoco && isChocoNULL0 && isMouseIn(currentMousePosition, chocoShape[0]))
				{ //Ʋ�� �ƹ��͵� ������ �� ���� ������ isChocoHalf Ȱ��ȭ
					isChocoHalf0 = true; isChocoNULL0 = false; isCooling = false; isChoco = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isChoco && isChocoHalf0 && isMouseIn(currentMousePosition, chocoShape[0]))
				{ //Ʋ�� ���ڰ� ��ƴ�� ���� �� ���� ������ isChocoFull Ȱ��ȭ
					isChocoFull0 = true; isChocoHalf0 = false; isChoco = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isStick && isChocoFull0 && isMouseIn(currentMousePosition, chocoShape[0]))
				{ //Ʋ�� ���ڰ� ���� �� ���� �� ��Ĺ ������ isChocoOk Ȱ��ȭ
					isChocoOk0 = true; isChocoFull0 = false; isStick = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isChocoOk0 && isCooling && isMouseIn(currentMousePosition, chocoShape[0]))
				{ //Ʋ�� ���ڰ��ڰ� �ϼ��ǰ� �ð����� �ߴٸ� isChocoNull0 Ȱ��ȭ
					isChocoCool0 = true; isChocoOk0 = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}

				//Ʋ 2
				if (isChoco && isChocoNULL1 && isMouseIn(currentMousePosition, chocoShape[1]))
				{ //Ʋ�� �ƹ��͵� ������ �� ���� ������ isChocoHalf Ȱ��ȭ
					isChocoHalf1 = true; isChocoNULL1 = false; isCooling = false; isChoco = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isChoco && isChocoHalf1 && isMouseIn(currentMousePosition, chocoShape[1]))
				{ //Ʋ�� ���ڰ� ��ƴ�� ���� �� ���� ������ isChocoFull Ȱ��ȭ
					isChocoFull1 = true; isChocoHalf1 = false; isChoco = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isStick && isChocoFull1 && isMouseIn(currentMousePosition, chocoShape[1]))
				{ //Ʋ�� ���ڰ� ���� �� ���� �� ��Ĺ ������ isChocoOk Ȱ��ȭ
					isChocoOk1 = true; isChocoFull1 = false; isStick = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isChocoOk1 && isCooling && isMouseIn(currentMousePosition, chocoShape[1]))
				{ //Ʋ�� ���ڰ��ڰ� �ϼ��ǰ� �ð����� �ߴٸ� isChocoNull0 Ȱ��ȭ
					isChocoCool1 = true; isChocoOk1 = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}

				//Ʋ 3
				else if (isChoco && isChocoNULL2 && isMouseIn(currentMousePosition, chocoShape[2]))
				{ //Ʋ�� �ƹ��͵� ������ �� ���� ������ isChocoHalf Ȱ��ȭ
					isChocoHalf2 = true; isChocoNULL2 = false; isCooling = false; isChoco = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isChoco && isChocoHalf2 && isMouseIn(currentMousePosition, chocoShape[2]))
				{ //Ʋ�� ���ڰ� ��ƴ�� ���� �� ���� ������ isChocoFull Ȱ��ȭ
					isChocoFull2 = true; isChocoHalf2 = false; isChoco = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isStick && isChocoFull2 && isMouseIn(currentMousePosition, chocoShape[2]))
				{ //Ʋ�� ���ڰ� ���� �� ���� �� ��Ĺ ������ isChocoOk Ȱ��ȭ
					isChocoOk2 = true; isChocoFull2 = false; isStick = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isChocoOk2 && isCooling && isMouseIn(currentMousePosition, chocoShape[2]))
				{ //Ʋ�� ���ڰ��ڰ� �ϼ��ǰ� �ð����� �ߴٸ� isChocoNull0 Ȱ��ȭ
					isChocoCool2 = true; isChocoOk2 = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}

				//Ʋ 4
				else if (isChoco && isChocoNULL3 && isMouseIn(currentMousePosition, chocoShape[3]))
				{ //Ʋ�� �ƹ��͵� ������ �� ���� ������ isChocoHalf Ȱ��ȭ
					isChocoHalf3 = true; isChocoNULL3 = false; isCooling = false; isChoco = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isChoco && isChocoHalf3 && isMouseIn(currentMousePosition, chocoShape[3]))
				{ //Ʋ�� ���ڰ� ��ƴ�� ���� �� ���� ������ isChocoFull Ȱ��ȭ
					isChocoFull3 = true; isChocoHalf3 = false; isChoco = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isStick && isChocoFull3 && isMouseIn(currentMousePosition, chocoShape[3]))
				{ //Ʋ�� ���ڰ� ���� �� ���� �� ��Ĺ ������ isChocoOk Ȱ��ȭ
					isChocoOk3 = true; isChocoFull3 = false; isStick = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isChocoOk3 && isCooling && isMouseIn(currentMousePosition, chocoShape[3]))
				{ //Ʋ�� ���ڰ��ڰ� �ϼ��ǰ� �ð����� �ߴٸ� isChocoNull0 Ȱ��ȭ
					isChocoCool3 = true; isChocoOk3 = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}

				//Ʋ 5
				else if (isChoco && isChocoNULL4 && isMouseIn(currentMousePosition, chocoShape[4]))
				{ //Ʋ�� �ƹ��͵� ������ �� ���� ������ isChocoHalf Ȱ��ȭ
					isChocoHalf4 = true; isChocoNULL4 = false; isCooling = false; isChoco = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isChoco && isChocoHalf4 && isMouseIn(currentMousePosition, chocoShape[4]))
				{ //Ʋ�� ���ڰ� ��ƴ�� ���� �� ���� ������ isChocoFull Ȱ��ȭ
					isChocoFull4 = true; isChocoHalf4 = false; isChoco = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isStick && isChocoFull4 && isMouseIn(currentMousePosition, chocoShape[4]))
				{ //Ʋ�� ���ڰ� ���� �� ���� �� ��Ĺ ������ isChocoOk Ȱ��ȭ
					isChocoOk4 = true; isChocoFull4 = false; isStick = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isChocoOk4 && isCooling && isMouseIn(currentMousePosition, chocoShape[4]))
				{ //Ʋ�� ���ڰ��ڰ� �ϼ��ǰ� �ð����� �ߴٸ� isChocoNull0 Ȱ��ȭ
					isChocoCool4 = true; isChocoOk4 = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}

				//Ʋ 6
				else if (isChoco && isChocoNULL5 && isMouseIn(currentMousePosition, chocoShape[5]))
				{ //Ʋ�� �ƹ��͵� ������ �� ���� ������ isChocoHalf Ȱ��ȭ
					isChocoHalf5 = true; isChocoNULL5 = false; isCooling = false; isChoco = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isChoco && isChocoHalf5 && isMouseIn(currentMousePosition, chocoShape[5]))
				{ //Ʋ�� ���ڰ� ��ƴ�� ���� �� ���� ������ isChocoFull Ȱ��ȭ
					isChocoFull5 = true; isChocoHalf5 = false; isChoco = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isStick && isChocoFull5 && isMouseIn(currentMousePosition, chocoShape[5]))
				{ //Ʋ�� ���ڰ� ���� �� ���� �� ��Ĺ ������ isChocoOk Ȱ��ȭ
					isChocoOk5 = true; isChocoFull5 = false; isStick = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}
				else if (isChocoOk5 && isCooling && isMouseIn(currentMousePosition, chocoShape[5]))
				{ //Ʋ�� ���ڰ��ڰ� �ϼ��ǰ� �ð����� �ߴٸ� isChocoNull0 Ȱ��ȭ
					isChocoCool5 = true; isChocoOk5 = false;
					pDemoApp->soundOn(pDemoApp, addIngredientSound, FALSE);
				}

				if ((isChocoCool0 || isChocoCool1 || isChocoCool2 || isChocoCool3 || isChocoCool4 || isChocoCool5) && isMouseIn(currentMousePosition, dishRectangle))
				{
					if (isChocoCool0)
					{
						isChocoCool0 = false; isChocoNULL0 = true;
					}
					else if (isChocoCool1)
					{
						isChocoCool1 = false; isChocoNULL1 = true;
					}
					else if (isChocoCool2)
					{
						isChocoCool2 = false; isChocoNULL2 = true;
					}
					else if (isChocoCool3)
					{
						isChocoCool3 = false; isChocoNULL3 = true;
					}
					else if (isChocoCool4)
					{
						isChocoCool4 = false; isChocoNULL4 = true;
					}
					else if (isChocoCool5)
					{
						isChocoCool5 = false; isChocoNULL5 = true;
					}
					chocoCnt++;
					if (orderingChocoCount == chocoCnt)
					{
						isOrdering = false; chocoCnt = 0;
						money += orderingChocoCount * 1000;
						pDemoApp->soundOn(pDemoApp, coinSound, FALSE);
					}
				}

				if (isCooling && isMouseIn(currentMousePosition, dishRectangle))
				{
					isDish = true;
				}

				if (isMouseIn(currentMousePosition, coolingOffRectangle))
				{
					isCoolingOff = !isCoolingOff;
					if (isCoolingOff) 
					{
						cooling_time = time(NULL);
						pDemoApp->soundOn(pDemoApp, coolingSound, FALSE);
						
					}
				}

				InvalidateRect(hwnd, NULL, FALSE);
				break;
			}
			case WM_MOUSEMOVE:
			{
				currentMousePosition.x = LOWORD(lParam);
				currentMousePosition.y = HIWORD(lParam);
				pDemoApp->myCharacter->setDestPosition(currentMousePosition);
				InvalidateRect(hwnd, NULL, FALSE);
				break;
			}


			case WM_PAINT:
			{
				pDemoApp->OnRender();

				// Do not call ValidateRect so that the window will be redrawn.
				//ValidateRect(hwnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			/* ���Ű(t)�� �����̽��� ���� ������ */
			case WM_KEYDOWN:
			{
				input_str = (TCHAR)wParam;
				switch (input_str)
				{
				case 't':
				case 'T':
					isShow = !isShow;
					break;

				case 'i':
				case 'I':
					isAIShow = !isAIShow;
					break;

				case 's':
				case 'S':
				{
					isMute = !isMute;
					if (isMute == true) {
						for (int i = 0; i < 5; i++) {
							pDemoApp->soundManager->stop(i);
						}
					}
					else {
						pDemoApp->soundManager->play(backgroundSound, true);
					}
					break;
				}
				case VK_UP:
				{
					if (currentSound != 100)
						currentSound += 5;
					pDemoApp->SetVolume(pDemoApp->soundManager->pDSBPrimary, currentSound);
					break;
				}
				case VK_DOWN:
				{
					if (currentSound != 0)
						currentSound -= 5;
					pDemoApp->SetVolume(pDemoApp->soundManager->pDSBPrimary, currentSound);
					break;
				}
				}
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_KEYUP:
			{
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
	return result;
}