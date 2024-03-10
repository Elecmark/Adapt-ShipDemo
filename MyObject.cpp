// MyObject.cpp: implementation of the CMyObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShipDemo.h"
#include "MyObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMyObject::CMyObject(CPoint ptPos)
{
    m_ptPos = ptPos;
    m_bEnemy = false;
}

CMyObject::CMyObject()
{
	m_bEnemy = false;
}

CMyObject::~CMyObject()
{

}
