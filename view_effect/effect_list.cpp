#include "pch.h"
#include "effect_list.h"
#include "soft_glow.h"

namespace
{
    enum
    {
        ITEM_HEIGHT = 64,
        EFFECT_COUNT = 4,
    };

    CRect CalculateWindowRect(CRect button_on_screen)
    {
        CSize   wnd_size;
        wnd_size.cx = CDPICalculator::Cast(230);
        wnd_size.cy = EFFECT_COUNT * CDPICalculator::Cast(ITEM_HEIGHT) + 2;

        CRect   wnd_rect(CPoint(button_on_screen.left, 0), wnd_size);
        if (theConfig.m_toolbar_dock == ToolbarDockStyle::Bottom)
            wnd_rect.MoveToY(button_on_screen.top - wnd_rect.Height());
        else
            wnd_rect.MoveToY(button_on_screen.bottom);
        return wnd_rect;
    }

    class CToggleButton : public CWidgetItemBitmapButton
    {
    private:
        CEffectList   & m_parent;
    public:
        CToggleButton(ViewEnhance effect, CEffectList& parent) : CWidgetItemBitmapButton((int)effect), m_parent(parent)
        {
            ReloadState();
        }

        void ReloadState()
        {
            SetState((int)theConfig.m_runtime_effect & GetID());
        }

        void SetState(BOOL turn_on)
        {
            FCImage   img = (turn_on ? m_parent.m_on : m_parent.m_off);
            AttachBitmap(img.Detach());
        }
    };

    class CIconImage : public CWidgetItemImage
    {
    public:
        CIconImage(FCImageEffect& effect, FCImage img)
        {
            img.ApplyEffect(effect);
            img.ApplyEffect(FCEffectPremultiply());
            AttachBitmap(img.Detach());
        }
    };

    class CTitleText : public CWidgetItemText
    {
    public:
        CTitleText(int k, const shared_ptr<CFont>& font)
        {
            SetFont(font);
            SetText(CLanguageText::Get(L"ViewEnhance", k));
            SetFormat(DT_VCENTER | DT_SINGLELINE);
            SetColor(theConfig.GetProperTextColor());
        }
    };

    class CClistItemLayout : public CWidgetLayout
    {
        void LayoutItem(CRect itemrect, CWidgetItem& toggle, CWidgetItem& img, CWidgetItem& text)
        {
            itemrect.left += CDPICalculator::Cast(6);
            CSize   sz = toggle.GetRectOnCanvas().Size();
            CRect   rc = CRect(itemrect.TopLeft(), sz);
            rc.OffsetRect(0, (itemrect.Height() - sz.cy) / 2);
            toggle.SetRectOnCanvas(rc);
            itemrect.left = rc.right + 12;

            sz = img.GetRectOnCanvas().Size();
            rc = CRect(itemrect.TopLeft(), sz);
            rc.OffsetRect(0, (itemrect.Height() - sz.cy) / 2);
            img.SetRectOnCanvas(rc);
            itemrect.left = rc.right + CDPICalculator::Cast(10);

            text.SetRectOnCanvas(itemrect);
        }

        virtual void LayoutWidget(CWnd& wnd, std::deque<CWidgetItemPtr>& child_items)
        {
            CRect   rc = FCWnd::GetClientRect(wnd);
            rc.DeflateRect(1, 1);
            rc.bottom = rc.top + CDPICalculator::Cast(ITEM_HEIGHT);

            size_t   cnt = child_items.size() / 3;
            for (size_t i = 0; i < cnt; i++)
            {
                LayoutItem(rc, *child_items[i * 3], *child_items[i * 3 + 1], *child_items[i * 3 + 2]);
                rc.OffsetRect(0, rc.Height());
            }
        }
    };
}

CEffectList::CEffectList()
{
    auto   font = make_shared<CFont>();
    font->CreatePointFont(90, FCFontManager::GetDefaultFontFaceName());

    CSequenceImage::LoadFile(CImageResource::GetFullPath(L"turnon.png"), m_on, WICPremultiplied32bpp, 3);
    CSequenceImage::LoadFile(CImageResource::GetFullPath(L"turnoff.png"), m_off, WICPremultiplied32bpp, 3);

    FCImage   icon;
    CSequenceImage::LoadFile(CImageResource::GetFullPath(L"effect_sample.png"), icon, WICNormal32bpp, 1);

    AddWidget(new CToggleButton(ViewEnhance::Brightness, *this));
    AddWidget(new CIconImage(FCEffectBrightnessContrast(38, 0), icon));
    AddWidget(new CTitleText(1, font));

    AddWidget(new CToggleButton(ViewEnhance::Saturation, *this));
    AddWidget(new CIconImage(FCEffectHueSaturation(0, 100), icon));
    AddWidget(new CTitleText(2, font));

    AddWidget(new CToggleButton(ViewEnhance::Sharpen, *this));
    AddWidget(new CIconImage(FCEffectUNSharpMask(2, 80, 0), icon));
    AddWidget(new CTitleText(3, font));

    AddWidget(new CToggleButton(ViewEnhance::SoftGlow, *this));
    AddWidget(new CIconImage(FCEffectSoftGlow(2, -30, -30), icon));
    AddWidget(new CTitleText(4, font));

    SetWidgetLayout(new CClistItemLayout);
}

void CEffectList::Create(CWnd* parent, CRect button_on_screen)
{
    m_toolbar = *parent;
    CWnd::CreateEx(WS_EX_TOOLWINDOW, AfxRegisterWndClass(0), NULL, WS_POPUP | WS_CLIPCHILDREN | WS_VISIBLE, CalculateWindowRect(button_on_screen), parent, 0);
    SetFocus();
}

void CEffectList::OnClickWidget(CWidgetItem& widget)
{
    int   id = widget.GetID();
    if (!id)
        return;

    int   & effect = (int&)theConfig.m_runtime_effect;
    if (effect & id)
    {
        effect = effect & ~id;
    }
    else
    {
        effect = effect | id;
    }
    ((CToggleButton&)widget).ReloadState();
    CFastCall::PostMainWndCommand(ID_NOTIFY_RELOAD_VIEW);
}

void CEffectList::DrawWidgetWindowBack(CDC& dc, CRect update_on_dc)
{
    CRect   rc = FCWnd::GetClientRect(*this);
    ::FillRect(dc, rc, theConfig.m_runtime_canvas_back);
    ::FrameRect(dc, rc, theConfig.GetProperBorderBrush());
}

LRESULT CEffectList::WindowProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_KILLFOCUS)
    {
        m_toolbar.PostMessage(WM_COMMAND, MAKEWPARAM(ID_CLOSE_EFFECT_LIST, 0));
    }
    return __super::WindowProc(msg, wParam, lParam);
}

BOOL CEffectList::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_MOUSEWHEEL)
        return 1;
    if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
    {
        m_toolbar.PostMessage(WM_COMMAND, MAKEWPARAM(ID_CLOSE_EFFECT_LIST, 0));
        return 1;
    }
    return __super::PreTranslateMessage(pMsg);
}
