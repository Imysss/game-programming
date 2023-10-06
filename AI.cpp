#include "hw3.h"
#include <time.h>
using namespace D2D1;

void DemoApp::drawAICharacter(bool isAIShow)
{
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	D2D1::Matrix3x2F Trans = D2D1::Matrix3x2F::Translation(myCharacter->getDestPositon().x, myCharacter->getDestPositon().y);

	LPWSTR str = new TCHAR[100];
	if (Character::isAttacked == true) {
		clearFlag();
		if ((time(NULL) - Character::lastAttackedTime) < 1) {
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(30, 100));
			m_pRenderTarget->FillRectangle(&RectF(0, 0,750, 650), explosion_BitmapBrush);
		}
		else {
			Character::isAttacked = false;
			setRandomBombPosition();
		}
	}
	//BombFalse();

	memset(str, 0, sizeof(str));
	for (int i = 0; i < character.size(); i++)
	{
		switch (character[i]->getColor())
		{
		case 0://Character.Black:
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			swprintf(str, 100, L"Stop");
			break;
		case 1://Character.Red:
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			swprintf(str, 100, L"attacking");
			break;
		case 2://Character.Blue:
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
			swprintf(str, 100, L"following");
			break;
		case 3: //Character.Green
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
			swprintf(str, 100, L"moving");
			break;
		default:
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			break;
		}

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(character[i]->getPosition().x, character[i]->getPosition().y));
		m_pRenderTarget->FillRectangle(&RectF(0, 0, 50, 50), bomb_BitmapBrush);

	
		//적 바깥 주변 원 그리기
		if (isAIShow == true)
		{
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
			m_pRenderTarget->DrawGeometry(pViewRange[i], m_pSceneBrush, 0.3f);
			m_pSceneBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			m_pRenderTarget->DrawGeometry(pAttackRange[i], m_pSceneBrush, 0.3f);
		}
	}


	for (std::vector<Character*>::iterator iter = character.begin(); iter != character.end(); iter++)
	{
		Character* ch = *iter;
		ch->update(myCharacter, 0.01);
	}
	myCharacter->update(myCharacter, 0.01);
}