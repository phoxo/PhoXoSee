#pragma once

class CWin32System
{
public:
    static CString GetFileVersion(PCWSTR file_path);

    static void OpenFolderAndSelectFile(PCWSTR file_path);
    static void DeleteFileToRecycle(PCWSTR file_path, FILEOP_FLAGS flags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_ALLOWUNDO);
    static void DeleteDirectoryTree(PCWSTR dir);
    static void SetWallpaper(PCWSTR image_path, DWORD style);

    static void CreatePrintImageThread(CString image_file);

    static void DisableWindowTransition(HWND wnd, BOOL disable);
    static void SetTitleColorThemeByConfig(HWND wnd);
    static void AdministratorRunSelf(PCWSTR param);
};

class CAutoHICON
{
public:
    HICON   p = NULL;
    ~CAutoHICON() { if (p) { DestroyIcon(p); } }

    CAutoHICON() {}
    CAutoHICON(int res_id, int output_size)
    {
        LoadIconWithScaleDown((HINSTANCE)&__ImageBase, MAKEINTRESOURCE(res_id), output_size, output_size, &p);
    }

    static HICON CreateIcon(const FCImage& img)
    {
        if (!img)
            return NULL;
        assert(!img.IsPremultiplied());
        CImageList   il;
        il.Create(img.Width(), img.Height(), ILC_COLOR32, 1, 1);
        il.Add(CBitmap::FromHandle(img), RGB(0, 0, 0));
        return il.ExtractIcon(0);
    }
};
