#include "pch.h"
#include "opened_explorer_sort.h"

namespace
{
    _COM_SMARTPTR_TYPEDEF(IFolderView, __uuidof(IFolderView));

    CString GetFolderViewPath(IFolderViewPtr view)
    {
        if (view)
        {
            IPersistFolder2Ptr   folder;
            view->GetFolder(IID_PPV_ARGS(&folder));
            if (folder)
            {
                CComHeapPtr<ITEMIDLIST_ABSOLUTE>   idl;
                folder->GetCurFolder(&idl);

                CPath   addr_path;
                SHGetPathFromIDList(idl, addr_path.m_strPath.GetBuffer(MAX_PATH));
                addr_path.m_strPath.ReleaseBuffer();
                addr_path.Canonicalize();
                addr_path.AddBackslash();
                return addr_path;
            }
        }
        return L"";
    }

    // maybe throw exception
    void EnumFolderFiles(IFolderViewPtr view, deque<CString>& found_files, BOOL& continue_enum)
    {
        IShellFolderPtr   shell_folder;
        view->GetFolder(IID_PPV_ARGS(&shell_folder));
        int   item_count = 0;
        view->ItemCount(SVGIO_ALLVIEW, &item_count);
        for (int i = 0; i < item_count; i++)
        {
            if (!continue_enum)
                break;

            CComHeapPtr<ITEMID_CHILD>   idl;
            view->Item(i, &idl);
            STRRET   str = { 0 };
            if (shell_folder->GetDisplayNameOf(idl, SHGDN_INFOLDER | SHGDN_FORPARSING, &str) == S_OK)
            {
                CComHeapPtr<WCHAR>   fname;
                StrRetToStr(&str, idl, &fname);
                found_files.push_back(CString(fname));
            }
        }
    }
}

namespace search_folder_image
{
    COpenedExplorerSort::COpenedExplorerSort(CString folder, BOOL& continue_enum)
    {
        IShellWindowsPtr   shwnds;
        shwnds.CreateInstance(CLSID_ShellWindows);
        if (!shwnds)
        {
            ASSERT(FALSE); return;
        }

        long   wnd_count = 0;
        shwnds->get_Count(&wnd_count);
        for (long i = 0; i < wnd_count; i++)
        {
            try
            {
                IDispatchPtr   tmp1;
                IShellBrowserPtr   tmp2;
                IShellViewPtr   active_view;
                shwnds->Item(CComVariant(i), &tmp1);
                if (!tmp1)
                    continue;

                IUnknown_QueryService(tmp1, SID_STopLevelBrowser, IID_PPV_ARGS(&tmp2));
                tmp2->QueryActiveShellView(&active_view);

                if (GetFolderViewPath(active_view).CompareNoCase(folder) == 0)
                {
                    EnumFolderFiles(active_view, m_found_files, continue_enum);
                    break;
                }
            }
            catch (_com_error&) { ASSERT(false); }
        }
    }
}
