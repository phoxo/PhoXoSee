#include "pch.h"
#import <wiaaut.dll>
#include "win32_system.h"
#pragma comment (lib, "Version.lib")
#pragma comment(lib, "Dwmapi.lib")

namespace
{
    VS_FIXEDFILEINFO* LoadFileVersion(PCWSTR filepath, std::vector<BYTE>& buf)
    {
        DWORD   bufsize = GetFileVersionInfoSize(filepath, NULL);
        if (bufsize)
        {
            buf.resize(bufsize);
            ::GetFileVersionInfo(filepath, 0, bufsize, buf.data());

            VS_FIXEDFILEINFO   * info = nullptr;
            UINT   len = 0;
            if (VerQueryValue(buf.data(), L"\\", (void**)&info, &len))
                return info; // info解析buf里的内容
        }
        return nullptr;
    }
}

CString CWin32System::GetFileVersion(PCWSTR file_path)
{
    std::vector<BYTE>   buf;
    auto   info = LoadFileVersion(file_path, buf);
    if (info)
    {
        CString   ver;
        ver.Format(L"%d.%d.%d.%d", (int)HIWORD(info->dwFileVersionMS), (int)LOWORD(info->dwFileVersionMS),
            (int)HIWORD(info->dwFileVersionLS), (int)LOWORD(info->dwFileVersionLS));
        return ver;
    }
    return L"";
}

void CWin32System::OpenFolderAndSelectFile(PCWSTR file_path)
{
    auto   idl = ILCreateFromPath(file_path);
    if (idl)
    {
        SHOpenFolderAndSelectItems(idl, 0, NULL, 0);
        ILFree(idl);
    }
}

void CWin32System::DeleteFileToRecycle(PCWSTR file_path, FILEOP_FLAGS flags)
{
    if (!PathFileExists(file_path))
        return;

    std::wstring   buf = file_path;
    buf.append(2, '\0');

    SHFILEOPSTRUCT   op = { 0 };
    op.pFrom = buf.c_str();
    op.wFunc = FO_DELETE;
    op.fFlags = flags;
    ::SHFileOperation(&op);
}

void CWin32System::DeleteDirectoryTree(PCWSTR dir)
{
    std::wstring   folder = dir;
    folder.append(2, '\0');

    SHFILEOPSTRUCT   op = { 0 };
    op.pFrom = folder.c_str();
    op.wFunc = FO_DELETE;
    op.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;
    SHFileOperation(&op);
}

void CWin32System::SetWallpaper(PCWSTR image_path, DWORD style)
{
    CComPtr<IActiveDesktop>   desktop;
    desktop.CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER);
    if (!desktop)
        return;

    COMPONENTSOPT   co = { sizeof(COMPONENTSOPT), TRUE, TRUE };
    desktop->SetDesktopItemOptions(&co, 0);
    desktop->SetWallpaper(image_path, 0);
    WALLPAPEROPT   wp = { sizeof(WALLPAPEROPT), style };
    desktop->SetWallpaperOptions(&wp, 0);
    desktop->ApplyChanges(AD_APPLY_ALL);
}

namespace
{
    void WIAPopPrintModalDialog(LPCTSTR image_file)
    {
        try
        {
            WIA::ICommonDialogPtr   printDlg;
            printDlg.CreateInstance(L"WIA.CommonDialog");

            WIA::IVectorPtr   imgList;
            imgList.CreateInstance(L"WIA.Vector");
            _variant_t   n1(image_file);
            imgList->Add(&n1, 0);

            _variant_t   n2(imgList.GetInterfacePtr());
            printDlg->ShowPhotoPrintingWizard(&n2);
        }
        catch (_com_error&) { ASSERT(FALSE); }
    }

    unsigned __stdcall print_image_thread(void* lpParameter)
    {
        FCAutoInitializeCOM   COM_init;
        CComBSTR   path;
        path.Attach((BSTR)lpParameter);
        WIAPopPrintModalDialog(path);
        return 0;
    }
};

void CWin32System::CreatePrintImageThread(CString image_file)
{
    auto   h = (HANDLE)_beginthreadex(NULL, 0, print_image_thread, image_file.AllocSysString(), 0, NULL);
    ::CloseHandle(h);
}

void CWin32System::DisableWindowTransition(HWND wnd, BOOL disable)
{
    ::DwmSetWindowAttribute(wnd, DWMWA_TRANSITIONS_FORCEDISABLED, &disable, sizeof(disable));
}

void CWin32System::SetTitleColorThemeByConfig(HWND wnd)
{
    switch (theConfig.m_title_theme)
    {
    case TitleColorTheme::Light:
        CSysColorScheme::SetWindowDarkTheme(wnd, FALSE);
        return;
    case TitleColorTheme::Dark:
        CSysColorScheme::SetWindowDarkTheme(wnd, TRUE);
        return;
    }

    // 其他设置默认跟随系统
    CSysColorScheme::SetWindowDarkTheme(wnd, CSysColorScheme::IsDarkTheme());
}

void CWin32System::AdministratorRunSelf(PCWSTR param)
{
    CString   exe = FCFile::GetModulePath();
    ::ShellExecute(NULL, L"runas", exe, param, NULL, SW_SHOWNORMAL);
}
