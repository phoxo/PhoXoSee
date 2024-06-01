#pragma once

class CDialogHelper
{
public:
    static void SetDialogAndChildFont(HWND dlg, HFONT font)
    {
        SendMessage(dlg, WM_SETFONT, (WPARAM)font, TRUE);
        EnumChildWindows(dlg, enum_set_font, (LPARAM)font);
    }

    static void DDX_Text_No_Tip(CDataExchange* pDX, int nIDC, int& t, int tDefault)
    {
        if (pDX->m_bSaveAndValidate)
        {
            CString   s;
            pDX->m_pDlgWnd->GetDlgItemText(nIDC, s);
            t = (s.GetLength() ? StrToInt(s) : tDefault);
        }
        else
        {
            DDX_Text(pDX, nIDC, t);
        }
    }

    static int AddComboString(CWnd* combo_ctrl, const CString& text)
    {
        if (!combo_ctrl)
            return 0;

        deque<CString>   vec;
        CStringHelper::SplitTextByToken(text, vec, L"|");
        for (auto& iter : vec)
        {
            ((CComboBox*)combo_ctrl)->AddString(iter);
        }
        return (int)vec.size();
    }

private:
    static BOOL CALLBACK enum_set_font(HWND hwnd, LPARAM lParam)
    {
        SendMessage(hwnd, WM_SETFONT, lParam, TRUE);
        return TRUE;
    }
};
