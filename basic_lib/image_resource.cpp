#include "pch.h"

namespace
{
    FCImage   g_menus;

    void ApplyColorTheme(FCImage& img)
    {
        BOOL   sys_dark = ((theConfig.m_title_theme == TitleColorTheme::FollowSystem) && CSysColorScheme::IsDarkTheme());
        if (sys_dark || (theConfig.m_title_theme == TitleColorTheme::Dark))
        {
            img.ApplyEffect(FCEffectColorize(Color::White, FALSE));
        }
    }
}

void CImageResource::LoadMenuIcon(UINT menu_id, FCImage& img, BOOL apply_color_theme)
{
    if (!g_menus)
    {
        LoadImageByDPI(IDR_PNG_MENU_ICO_100, 0, IDR_PNG_MENU_ICO_200, g_menus, WICNormal32bpp);
    }

    int   cmd_list[] =
    {
        ID_APP_OPEN_ABOUT,
        ID_RUN_SEARCH_IMAGE,
        IDCANCEL,
        ID_SET_DEFAULT_IMAGE_VIEWER,
        ID_IMAGE_PASTE_IMAGE,
        ID_IMAGE_COPY_CLIPBOARD,
        ID_VIEW_SHOW_EXIF,
        ID_VIEW_DELETE_FILE,
        ID_VIEW_IMAGE_RESIZE,
        ID_IMAGE_FILE_OPEN,
        ID_IMAGE_FILE_SAVEAS,
        ID_IMAGE_OPEN_FOLDER,
        ID_IMAGE_PRINT,
        ID_APP_OPEN_SETTING
    };

    CRect   rc(0, 0, g_menus.Height(), g_menus.Height());
    for (auto iter : cmd_list)
    {
        if (iter == menu_id)
        {
            FCImageHandle::GetRegion(g_menus, rc, img);
            if (apply_color_theme)
            {
                ApplyColorTheme(img);
            }
            break;
        }
        rc.OffsetRect(rc.Width(), 0);
    }
}

void CImageResource::LoadImageByDPI(UINT res100, UINT res150, UINT res200, FCImage& outimg, REFWICPixelFormatGUID output_format)
{
    UINT   resid = res200;
    int   dpi = CDPICalculator::g_current_dpi();
    if (dpi == USER_DEFAULT_SCREEN_DPI)
        resid = res100;
    else if (dpi <= 144) // 1.5x
        resid = (res150 ? res150 : res200);

    BOOL   need_scale = TRUE;
    if ((dpi == 96) || (dpi == 192))
        need_scale = FALSE;
    if ((dpi == 144) && res150)
        need_scale = FALSE;

    IStreamPtr   stm = FCBaseHelper::LoadResource(resid, L"PNG");
    if (!need_scale)
    {
        FCCodecWIC::LoadStream(stm, outimg, output_format);
    }
    else
    {
        
        CSequenceImage   loader(stm, (resid == res150) ? 144 : 192);
        loader.Load(outimg, output_format);
    }
}
