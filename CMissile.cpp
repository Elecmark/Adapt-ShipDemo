#include "stdafx.h"
#include "ShipDemo.h"
#include "CMissile.h"
#include "MyObject.h"
#include <gdiplus.h>
using namespace Gdiplus;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CImageList CMissile::m_Images;

CMissile::CMissile(CPoint ptPos) : CMyObject(ptPos)
{
    CRect GetRect(); // ��ȡ��������ײ��
    m_ptPos = ptPos;
    LoadImage();
}

CRect CMissile::GetRect() const {
    // ���赼���Ĵ�С��һ������
    // ���Ը���ʵ��������������С
    const int MissileWidth = 30;
    const int MissileHeight = 10;
    return CRect(m_ptPos.x, m_ptPos.y, m_ptPos.x + MissileWidth, m_ptPos.y + MissileHeight);
}


CMissile::~CMissile()
{
    DeleteImage();
}

bool CMissile::Draw(CDC* pDC, bool bPause)
{
    m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
    return false;
}

void CMissile::Move()
{
    // ���赼�����ƶ�������Ļ��Χ����Ҫɾ����
    m_ptPos.y += 5;
    if (m_ptPos.y > GAME_HEIGHT) {
        // ��Ҫ����һ����־���Ա�����ѭ����ɾ�������������
        SetToDelete(true);
    }
}

BOOL CMissile::LoadImage()
{
    // ��ʼ�� GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // ����Դ���� PNG ͼƬ
    HRSRC hResource = ::FindResource(NULL, MAKEINTRESOURCE(IDB_MISSILE), _T("PNG"));

    if (!hResource) return FALSE;
    DWORD imageSize = ::SizeofResource(NULL, hResource);
    if (!imageSize) return FALSE;
    const void* pResourceData = ::LockResource(::LoadResource(NULL, hResource));
    if (!pResourceData) return FALSE;

    HGLOBAL hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
    if (hBuffer)
    {
        void* pBuffer = ::GlobalLock(hBuffer);
        if (pBuffer)
        {
            CopyMemory(pBuffer, pResourceData, imageSize);
            IStream* pStream = NULL;
            if (::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream) == S_OK)
            {
                Bitmap* bitmap = Bitmap::FromStream(pStream);
                if (bitmap)
                {
                    if (bitmap->GetLastStatus() == Ok)
                    {
                        HBITMAP hBitmap;
                        bitmap->GetHBITMAP(Color(0, 0, 0), &hBitmap);
                        CBitmap bmp;
                        bmp.Attach(hBitmap);

                        // ����ͼ���б�
                        if (m_Images.Create(30, 10, ILC_COLOR24 | ILC_MASK, 1, 1))
                        {
                            m_Images.Add(&bmp, RGB(255, 0, 255)); // ͸��ɫ
                        }

                        delete bitmap;
                    }
                }
                pStream->Release();
            }
            ::GlobalUnlock(hBuffer);
        }
        ::GlobalFree(hBuffer);
    }

    GdiplusShutdown(gdiplusToken);

    return TRUE;
}



void CMissile::SetToDelete(bool flag)
{
    m_toDelete = flag;
}

void CMissile::DeleteImage()
{
    m_Images.DeleteImageList();
}
