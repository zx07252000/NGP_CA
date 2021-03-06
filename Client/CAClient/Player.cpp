#include "stdafx.h"
#include "Player.h"

CPlayer::CPlayer(Vector2D<float> position)
{
	m_Position = position;

	m_Size = { OBJECT_SIZE, OBJECT_SIZE };
	m_Vel = { PlAYER_SPEED * 2, PlAYER_SPEED * 2};

	m_Dir = Direction::down;

	LoadImages();
	LoadSounds();

	m_State = PlayerState::wait;

	m_Power = 3; 
	m_TimeSum = 0.0f;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Draw(HDC hdc)
{
	m_Images[(int)m_currentAnimation].TransparentBlt(
		hdc, m_Position.x, m_Position.y,
		m_Size.x, m_Size.y,
		//m_AnimationSizes[(int)m_currentAnimation].x,
		//m_AnimationSizes[(int)m_currentAnimation].x,
		m_AnimationIdx * m_AnimationSizes[(int)m_currentAnimation].x, 0, 
		m_AnimationSizes[(int)m_currentAnimation].x,
		m_AnimationSizes[(int)m_currentAnimation].y,
		RGB(255,0,255));
}

void CPlayer::Update(float timeElapsed)
{
	m_PrevPosition = m_Position;
	if (m_State == PlayerState::move)
	{
		if (m_Dir == Direction::left)
			m_Position.x = m_Position.x - (m_Vel.x * timeElapsed);
		if (m_Dir == Direction::right)
			m_Position.x = m_Position.x + (m_Vel.x * timeElapsed);
		if (m_Dir == Direction::up)
			m_Position.y = m_Position.y - (m_Vel.y * timeElapsed);
		if (m_Dir == Direction::down)
			m_Position.y = m_Position.y + (m_Vel.y * timeElapsed);
	}
	Animate(timeElapsed);
}

bool CPlayer::IsCollide(const CGameObject& other)
{
	return false;
}

bool CPlayer::IsCollide(CGameObject* other)
{
	RECT rtThis = this->GetCollisionRect();
	RECT rtOther = other->GetCollisionRect();

	bool leftCondition = rtThis.left < rtOther.right;
	bool rightCondition = rtThis.right > rtOther.left;
	bool topCondition = rtThis.top < rtOther.bottom;
	bool bottomCondition = rtThis.bottom > rtOther.top;

	return (leftCondition &&
		rightCondition &&
		topCondition &&
		bottomCondition);
}

void CPlayer::FixCollision()
{
	m_Position = m_PrevPosition;
}

void CPlayer::ChangeState(PlayerState nextState)
{
	m_State = nextState;
	m_currentAnimation = (PlayerImages)nextState;
	m_AnimationIdx = 0;
}

void CPlayer::Move(Direction dir)
{
	bool cantMove =
		(m_State == PlayerState::die)
		| (m_State == PlayerState::live)
		| (m_State == PlayerState::trap);
	if (cantMove) return;

	switch (dir)
	{
	case Direction::left:
		m_currentAnimation = PlayerImages::left;
		break;
	case Direction::right:
		m_currentAnimation = PlayerImages::right;
		break;
	case Direction::up:
		m_currentAnimation = PlayerImages::up;
		break;
	case Direction::down:
		m_currentAnimation = PlayerImages::down;
		break;
	}
	m_Dir = dir;
	m_State = PlayerState::move;
}

void CPlayer::Stop()
{
	m_State = PlayerState::wait;
	m_AnimationIdx = 0;
}

void CPlayer::LoadImages()
{
	m_Images[0].Load(_T("assets/player/bazzi/down.bmp"));
	m_Images[1].Load(_T("assets/player/bazzi/up.bmp"));
	m_Images[2].Load(_T("assets/player/bazzi/left.bmp"));
	m_Images[3].Load(_T("assets/player/bazzi/right.bmp"));
	m_Images[4].Load(_T("assets/player/bazzi/trap.bmp"));
	m_Images[5].Load(_T("assets/player/bazzi/die.bmp"));
	m_Images[6].Load(_T("assets/player/bazzi/live.bmp"));

	m_AnimationIdx = 0;
	m_currentAnimation = PlayerImages::down;

	m_AnimationSizes[0] = { 64, 76 };
	m_AnimationSizes[1] = { 64, 76 };
	m_AnimationSizes[2] = { 64, 76 };
	m_AnimationSizes[3] = { 64, 76 };
	m_AnimationSizes[4] = { 88, 144 };
	m_AnimationSizes[5] = { 88, 144 };
	m_AnimationSizes[6] = { 88, 144 };

	m_AnimationLen[0] = 8;
	m_AnimationLen[1] = 8;
	m_AnimationLen[2] = 6;
	m_AnimationLen[3] = 6;

	m_AnimationLen[4] = 13;
	m_AnimationLen[5] = 13;
	m_AnimationLen[6] = 5;
}

void CPlayer::LoadSounds()
{
	
}

void CPlayer::Animate(float timeElapsed)
{
	if (m_State == PlayerState::wait) return;

	m_TimeSum += timeElapsed;

	if (m_TimeSum < 0.1f) return;
	m_TimeSum = 0.0f;

	++m_AnimationIdx;
	if (m_AnimationIdx >= m_AnimationLen[(int)m_currentAnimation])
	{
		m_AnimationIdx = 0;
	}
}
