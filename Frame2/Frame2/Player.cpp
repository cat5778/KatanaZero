#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
	: m_bIsJump(false),
	m_fJumpForce(0.f),
	m_fJumpAcc(0.f),
	m_ePreState(STATE_END),
	m_eCurState(STATE_END)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
	m_tInfo.fX = 400.f;
	m_tInfo.fY = 300.f;
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	m_fSpeed = 5.f;
	m_fAngle = 90.f;	// Degree	

	m_fJumpForce = 20.f;

	m_eCurState = STATE_IDLE;
	m_ePreState = m_eCurState;
	m_bIsDead = false;
	m_tFrame.dwFrameStart = 0;
	m_tFrame.dwFrameCount = 4;
	m_tFrame.dwFrameY = 0;
	m_tFrame.dwFrameSpeed = 200; // 0.2초 간격
	m_tFrame.dwOldTime = GetTickCount();

	m_wstrImageKey = L"Player_DOWN";
}

int CPlayer::Update()
{

	KeyInput();
	//ChangeState();
	return NO_EVENT;
}

void CPlayer::Render(HDC hdc)
{

	CGameObject::UpdateRect();
	Rectangle(hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	//HDC hMemDC = CBmpMgr::GetInstance()->GetMemDC(m_wstrImageKey);
	//NULL_CHECK(hMemDC);

	// Source DC에 그려진 비트맵을 Dest DC로 복사하는 함수. 이 때 지정한 색상을 제거할 수 있다.
	//GdiTransparentBlt(hdc,
	//	m_tRect.left - (int)g_fScrollX,
	//	m_tRect.top,
	//	(int)m_tInfo.fCX,
	//	(int)m_tInfo.fCY,
	//	hMemDC,
	//	m_tFrame.dwFrameStart * (int)m_tInfo.fCX,
	//	m_tFrame.dwFrameY * (int)m_tInfo.fCY,
	//	(int)m_tInfo.fCX,
	//	(int)m_tInfo.fCY,
	//	RGB(0, 0, 0));
	//	
	MoveToEx(hdc, m_tInfo.fX - g_fScrollX, m_tInfo.fY, nullptr);
	if (g_tMouseInfo.bStart)
	{
		cout << g_tMouseInfo.bStart << endl;

		LineTo(hdc, g_tMouseInfo.ptStart.x, g_tMouseInfo.ptStart.y);
		Rectangle(hdc, m_tHitBox.left, m_tHitBox.top, m_tHitBox.right, m_tHitBox.bottom);

	}
}

void CPlayer::Release()
{
}

void CPlayer::KeyInput()
{

	if (CKeyManager::GetInstance()->KeyPressing(KEY_A))
	{
		m_tInfo.fX -= m_fSpeed;
		m_wstrImageKey = L"Player_LEFT";
		m_eCurState = STATE_RUN;
	}
	else if (CKeyManager::GetInstance()->KeyPressing(KEY_D))
	{
		m_tInfo.fX += m_fSpeed;
		m_wstrImageKey = L"Player_RIGHT";
		m_eCurState = STATE_RUN;
	}
	else if (CKeyManager::GetInstance()->KeyPressing(KEY_W))
	{
		m_tInfo.fY -= m_fSpeed;
		m_wstrImageKey = L"Player_UP";
		m_eCurState = STATE_RUN;
	}
	else if (CKeyManager::GetInstance()->KeyPressing(KEY_S))
	{
		m_tInfo.fY += m_fSpeed;
		m_wstrImageKey = L"Player_DOWN";
		m_eCurState = STATE_RUN;
	}
	else if (CKeyManager::GetInstance()->KeyPressing(KEY_SPACE))
	{
		cout << "space" << endl;
		m_tInfo.fY += m_fSpeed;
		m_wstrImageKey = L"Player_DOWN";
		m_eCurState = STATE_RUN;
	}
	else
		m_eCurState = STATE_IDLE;

}

void CPlayer::Jump()
{
	// 지형 충돌
	float fY = m_tInfo.fY; //fY <= 바닥 높이
	//bool bIsColl = CLineMgr::GetInstance()->CollisionLine(this, &fY);
	if (m_bIsJump)//상승
	{
		m_fLeftVal = m_fJumpForce * m_fJumpAcc;
		m_fRightVal = GRAVITY * m_fJumpAcc * m_fJumpAcc * 0.5f;
		m_tInfo.fY -= m_fLeftVal - m_fRightVal;
		m_fJumpAcc += 0.3f;

		if (m_tInfo.fY > fY)
		{
			m_tInfo.fY = fY - m_tInfo.fCY*0.5f;
			m_fJumpAcc = 0;
		}
	}
	else//하강
	{
		//cout << "PosY= "<<m_tInfo.fPosY <<"바닥= " <<fY - m_tInfo.fCY*0.5f << endl;

		if (m_tInfo.fY >= fY - m_tInfo.fCY*0.5f - 11)//바닥보다 아래에잇을때
		{
			m_tInfo.fY = fY - m_tInfo.fCY*0.5f;
			m_fJumpAcc = 0;
		}
		else //위에 
		{

			//cout << "dd" << fRightVal<<endl;
			m_fRightVal = GRAVITY * m_fJumpAcc * m_fJumpAcc * 0.5f;
			m_fJumpAcc += 0.3f;
			m_tInfo.fY += m_fRightVal;

		}
	}
}

void CPlayer::ScrollOffset()
{
}

void CPlayer::Animate()
{
}

void CPlayer::ChangeState()
{
	// FSM (Finite State Machine, 유한 상태 기계)
	// 오브젝트는 한번에 하나의 상태만 가질 수 있다.
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE_IDLE:
			m_tFrame.dwFrameStart = 0;
			m_tFrame.dwFrameCount = 10;
			m_tFrame.dwFrameY = 0;
			m_tFrame.dwFrameSpeed = 200; // 0.2초 간격
			m_tFrame.dwOldTime = GetTickCount();
			break;
		case STATE_RUN:
			m_tFrame.dwFrameStart = 0;
			m_tFrame.dwFrameCount = 6;
			m_tFrame.dwFrameY = 1;
			m_tFrame.dwFrameSpeed = 200; // 0.2초 간격
			m_tFrame.dwOldTime = GetTickCount();
			break;
		}

		m_ePreState = m_eCurState;
	}
}
