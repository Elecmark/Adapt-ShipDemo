// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "ShipDemo.h"
#include "ChildView.h"

#include "Explosion.h"
#include "MyObject.h"
#include "Score.h"
#include "MyShip.h"
#include "Submarine.h"
#include "CMissile.h"
#include "typeinfo"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SHIP_VERT_POS		20
#define SKY_COLOR			RGB(0, 255, 255)
#define SEA_COLOR			RGB(0, 128, 255)
/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
	// load image list;
	CExplosion::LoadImage();
	CMyShip::LoadImage();
	CSubmarine::LoadImage();

	// create my ship object, this is the 1st object in object list
	m_Objects.AddTail(new CMyShip(CPoint((GAME_WIDTH - SHIP_WIDTH) / 2, SHIP_VERT_POS)));

	m_bGamePause = false;
}

CChildView::~CChildView()
{
	// free memory

	// delete all objects
	POSITION pos = m_Objects.GetHeadPosition();

	while (pos != NULL)
	{
		delete m_Objects.GetNext(pos);
	}
	m_Objects.RemoveAll();

	// clear image lists
	CExplosion::DeleteImage();
	CMyShip::DeleteImage();
	CSubmarine::DeleteImage();
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_GAMEPAUSE, OnGamepause)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//	cs.dwExStyle |= WS_EX_CLIENTEDGE;  // I remove this line
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

// repaint window so easily
	dc.BitBlt(0, 0, GAME_WIDTH, GAME_HEIGHT, &m_VirtualDC, 0, 0, SRCCOPY);

	POSITION pos;
	for (pos = m_Objects.GetHeadPosition(); pos != NULL;)
	{
		CMyObject* pObject = (CMyObject*)m_Objects.GetNext(pos);
		if (pObject != NULL)
		{
			pObject->Draw(&dc, m_bGamePause);
		}
	}

	// Do not call CWnd::OnPaint() for painting messages
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

// create virtual back buffer 
	CClientDC dc(this);
	if (!m_VirtualBitmap.CreateCompatibleBitmap(&dc, GAME_WIDTH, GAME_HEIGHT)) {
		AfxMessageBox(_T("Failed to create virtual bitmap!"));
		return -1;
	}

	if (!m_VirtualDC.CreateCompatibleDC(&dc)) {
		AfxMessageBox(_T("Failed to create virtual DC!"));
		return -1;
	}
	m_VirtualDC.SelectObject(&m_VirtualBitmap);

	// create loop
	SetTimer(100, 30, NULL);
	return 0;
}

void CChildView::OnTimer(UINT nIDEvent)
{
	if (m_bGamePause)
		return;

	// �����µ�Ǳͧ
	static int nCreator = random(50) + 20;
	if (nCreator == 0)
	{
		m_Objects.AddTail(new CSubmarine(random(2) % 2 ? true : false, random(GAME_HEIGHT - SHIP_VERT_POS - SHIP_HEIGHT * 2 - 30) + SHIP_VERT_POS + SHIP_HEIGHT + 30, random(4), random(4) + 3));
		nCreator = random(50) + 10;
	}

	// ���Ʊ���
	m_VirtualDC.FillSolidRect(0, 0, GAME_WIDTH, SHIP_VERT_POS + SHIP_HEIGHT - 5, SKY_COLOR);
	m_VirtualDC.FillSolidRect(0, SHIP_VERT_POS + SHIP_HEIGHT - 5, GAME_WIDTH, GAME_HEIGHT, SEA_COLOR);


















	// ���¶���״̬������
	POSITION pos1, pos2;
	for (pos1 = m_Objects.GetHeadPosition(); (pos1 != NULL);)
	{
		pos2 = pos1; // ���浱ǰ�����λ��
		CMyObject* pObj = (CMyObject*)m_Objects.GetNext(pos1);

		// ���pObj��Ǳͧ���ߵ���������������Ĵ����д�����
		// �Ȼ������ж���
		pObj->Draw(&m_VirtualDC, m_bGamePause);

		// ��� pObj �ǵ���������Ƿ���Ǳͧ������ײ
		if (typeid(*pObj) == typeid(CMissile))
		{
			CMissile* pMissile = (CMissile*)pObj;
			pMissile->Move(); // �ƶ�����

			POSITION pos3, pos4;
			for (pos3 = m_Objects.GetHeadPosition(); (pos3 != NULL);)
			{
				pos4 = pos3; // �������Ҫ��ɾ����Ǳͧ��λ��
				CMyObject* pOtherObj = (CMyObject*)m_Objects.GetNext(pos3);
				if (pOtherObj != pObj && typeid(*pOtherObj) == typeid(CSubmarine))
				{
					CSubmarine* pSub = (CSubmarine*)pOtherObj;
					// ���������Ǳͧ�ľ����ཻ
					if (pMissile->GetRect().IntersectRect(pMissile->GetRect(), pSub->GetRect()))
					{
						// ��ӱ�ը�͵÷�
						m_Objects.AddTail(new CExplosion(pSub->GetPos()));
						m_Objects.AddTail(new CScore(pSub->GetPos(), pSub->GetType() + 1, RGB(random(255), random(255), random(255))));

						// ɾ��Ǳͧ
						m_Objects.RemoveAt(pos4);
						delete pSub;

						// ɾ������
						m_Objects.RemoveAt(pos2);
						delete pMissile;

						// ������ǰѭ������Ϊ����ɾ���˵���������Ĵ��벻Ӧ���ٷ�����
						goto nextIteration;
					}
				}
			}
		}
	nextIteration:
		continue; // �����ⲿѭ������һ������
	}


















	// ��ʾ������������Ϣ
	m_VirtualDC.SetBkMode(TRANSPARENT);
	m_VirtualDC.SetTextColor(RGB(255, 0, 0));
	CString string;
	string.Format("Total objects: %d", m_Objects.GetCount());
	m_VirtualDC.TextOut(10, 10, string);
	m_VirtualDC.SetTextAlign(TA_CENTER);
	m_VirtualDC.TextOut(GAME_WIDTH / 2, GAME_HEIGHT / 2, "Click submarine to kill it");
	m_VirtualDC.SetTextAlign(TA_LEFT);

	// ������Ļ
	CClientDC dc(this);
	dc.BitBlt(0, 0, GAME_WIDTH, GAME_HEIGHT, &m_VirtualDC, 0, 0, SRCCOPY);

	nCreator--;

}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//	// TODO: Add your message handler code here and/or call default
	//	POSITION pos1, pos2;
	//	CSubmarine* pSub;
	//
	//
	//
	//// check all objects to find out enemy
	//	for(pos1=m_Objects.GetHeadPosition(); (pos2=pos1)!=NULL;)
	//   	{
	//		if(((CMyObject*)m_Objects.GetNext(pos1))->IsEnemy())  // object is a submarine
	//		{
	//			pSub = (CSubmarine*)m_Objects.GetAt(pos2);  // save for deletion
	//			if(pSub->GetRect().PtInRect(point))
	//			{
	//				// here we first create explosion object to make score always above that explosion
	//				m_Objects.AddTail(new CExplosion(pSub->GetPos()));
	//				m_Objects.AddTail(new CScore(pSub->GetPos(), pSub->GetType()+1, RGB(random(255), random(255), random(255))));
	//				m_Objects.RemoveAt(pos2);
	//				delete pSub;
	//			}
	//		}
	//	}
	//	
	////	CWnd ::OnLButtonDown(nFlags, point);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_bGamePause)
		return;

	CMyShip* pShip = (CMyShip*)m_Objects.GetHead();
	CPoint pt = pShip->GetPos();

	if (nChar == VK_LEFT || nChar == 0x41) // turn left
	{
		if (pt.x > 0)
			pShip->SetPos(pt.x - 5, pt.y); // ����ȷ�� x �����С
	}
	else if (nChar == VK_RIGHT || nChar == 0x44) // turn right
	{
		if (pt.x < GAME_WIDTH - SHIP_WIDTH)
			pShip->SetPos(pt.x + 5, pt.y); // ����ȷ�� x ��������
	}
	else if (nChar == VK_SPACE || nChar == VK_RETURN) // ʹ�ÿո����س�����Ͷ������
	{
		CPoint shipPos = pShip->GetPos();
		CMissile* pMissile = new CMissile(CPoint(shipPos.x, SHIP_VERT_POS));
		m_Objects.AddTail(pMissile);
		return;
	}




	//	CWnd ::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
	KillTimer(100);
}

void CChildView::OnGamepause()
{
	// TODO: Add your command handler code here
	m_bGamePause = !m_bGamePause;  // toggle pause status
	CString strTitle;
	strTitle.LoadString(AFX_IDS_APP_TITLE);
	AfxGetMainWnd()->SetWindowText(m_bGamePause ? strTitle + _T(" - ��ͣ") : strTitle);
}
