#pragma once

#include "MyObject.h"
#include <afxdisp.h>

class CMissile : public CMyObject
{
public:
    CMissile(CPoint ptPos);
    virtual ~CMissile();
    void SetToDelete(bool flag);
    virtual bool Draw(CDC* pDC, bool bPause);
    void Move();
    CRect GetRect() const;
private:
    bool m_toDelete;
    static CImageList m_Images;
    static BOOL LoadImage();
    static void DeleteImage();
};
