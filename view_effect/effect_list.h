#pragma once

class CEffectList : public CWidgetWindow
{
private:
    CWindow   m_toolbar;

public:
    FCImage   m_on, m_off;

    CEffectList();

    void Create(CWnd* parent, CRect button_on_screen);

private:
    virtual void OnClickWidget(CWidgetItem& widget);
    virtual void DrawWidgetWindowBack(CDC& dc, CRect update_on_dc);
    virtual LRESULT WindowProc(UINT msg, WPARAM wParam, LPARAM lParam);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
