#pragma once

class CImageResource
{
public:
    static CString GetFullPath(PCWSTR filename)
    {
        return FCFile::GetModuleFolder() + L"res\\" + CString(filename);
    }

    // the returned image is 32bpp BGRA
    static void LoadMenuIcon(UINT menu_id, FCImage& img, BOOL apply_color_theme = FALSE);

    static void LoadImageByDPI(UINT res100, UINT res150, UINT res200, FCImage& outimg, REFWICPixelFormatGUID output_format);
};
