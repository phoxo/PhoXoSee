#pragma once

class CSysColorScheme
{
public:
    static BOOL IsDarkTheme()
    {
        PCWSTR   sub = L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
        DWORD   type = 0;
        DWORD   val = 0;
        DWORD   len = 4;
        auto   ret = ::SHGetValue(HKEY_CURRENT_USER, sub, L"AppsUseLightTheme", &type, &val, &len);
        if (ret == ERROR_SUCCESS)
        {
            assert(type == REG_DWORD);
            return (val != 1);
        }
        // maybe ret == ERROR_FILE_NOT_FOUND
        return FALSE;
    }

    static void SetWindowDarkTheme(HWND wnd, BOOL enable)
    {
        ::DwmSetWindowAttribute(wnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &enable, sizeof(enable));
    }

    // Öµ¸úGdiplus::Color¼æÈÝ£¬R.BË³Ðò¸úCOLORREFÊÇµßµ¹µÄ
    enum
    {
        DarkBack = 0xFF202020,
        TwitterBlue = 0xFF1DA1F2,
    };

    // get user accent color
//     #include <winrt/Windows.UI.ViewManagement.h>
//     using namespace winrt;
//     using namespace Windows::UI::ViewManagement;
//     UISettings ui_settings{};
//     auto accent_color{ ui_settings.GetColorValue(UIColorType::Accent) };
};
