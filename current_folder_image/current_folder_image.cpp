#include "pch.h"
#include "PhoXoSee.h"
#include "MainDlg.h"
#include "current_folder_image.h"
#include "opened_explorer_sort.h"
#include "current_folder_win32_finder.h"

using namespace search_folder_image;

namespace
{
    class CSearchFolderImagesTask : public CAsyncTask
    {
    private:
        CString   m_folder;
        CString   m_current_filename;

    public:
        unique_ptr<CWin32FindFile>   m_result;

        CSearchFolderImagesTask(PCWSTR folder, PCWSTR current_file)
        {
            m_folder = folder;
            m_current_filename = current_file;
        }

    private:
        virtual void Execute()
        {
            m_result = make_unique<CWin32FindFile>(m_folder, m_current_filename);
            COpenedExplorerSort   explorer_find(m_folder, m_is_valid);
            if (m_is_valid) // 是正常结束，不是被user中断的
            {
                m_result->MergeExplorerFind(explorer_find.m_found_files, m_current_filename);
            }
        }
    };
}

void CCurrentFolderImage::SearchFolderImages(PCWSTR current_image_path)
{
    CString   new_filename = PathFindFileName(current_image_path);
    CPath   path = current_image_path;
    path.RemoveFileSpec();
    path.AddBackslash();

    if (GetRunningTask().size())
    {
        BlockWaitAndDiscardAllTask();
    }
    else
    {
        if ((StrCmpI(m_folder, path) == 0) && NewImageInSameFolderUpdate(new_filename))
        {
            theApp.GetPreloadCache().AddPreLoadTask(m_folder, m_current_index, m_folder_images);
            ((CMainDlg*)AfxGetMainWnd())->OnFolderImagesSearchFinish();
            return;
        }
    }

    m_folder = path.m_strPath;
    m_folder_images.clear();
    m_folder_images.push_back(new_filename);
    m_current_index = 0;

    AddAsyncTask(new CSearchFolderImagesTask(m_folder, new_filename));
}

BOOL CCurrentFolderImage::GetCurrentImagePosition(int& current_index, int& total_images) const
{
    current_index = m_current_index;
    total_images = (int)m_folder_images.size();
    return total_images && (current_index != -1);
}

CString CCurrentFolderImage::PrepareViewNeighbor(ViewDirection direction) const
{
    if (IsNeighborImageExist(direction))
    {
        int   offset = ((direction == ViewDirection::Next) ? 1 : -1);
        int   idx = m_current_index + offset;
        if (theConfig.m_view_folder_loop)
        {
            int   total = (int)m_folder_images.size();
            if (idx < 0)
                idx = total - 1;
            else if (idx >= total)
                idx = 0;
        }

        theConfig.m_view_direction = direction; // 更新预读取方向
        return m_folder + m_folder_images[idx];
    }
    return L"";
}

BOOL CCurrentFolderImage::IsNeighborImageExist(ViewDirection direction) const
{
    // 正在遍历目录返回false，不能前后翻页
    if (GetRunningTask().size())
        return FALSE;
    int   total = (int)m_folder_images.size();
    if (total <= 1)
        return FALSE;

    if (theConfig.m_view_folder_loop)
        return TRUE;

    int   offset = ((direction == ViewDirection::Next) ? 1 : -1);
    int   idx = m_current_index + offset;
    return (idx >= 0) && (idx < total);
}

BOOL CCurrentFolderImage::NewImageInSameFolderUpdate(PCWSTR new_filename)
{
    int   pos = CStringHelper::FindNoCase(m_folder_images, new_filename);
    if (pos != -1)
    {
        m_current_index = pos;
        return TRUE;
    }
    return FALSE;
}

CString CCurrentFolderImage::OnAfterImageDeleted(PCWSTR image_path)
{
    if (GetRunningTask().size())
    {
        BlockWaitAndDiscardAllTask();
    }

    int   pos = CStringHelper::FindNoCase(m_folder_images, PathFindFileName(image_path));
    if (pos != -1)
    {
        m_folder_images.erase(m_folder_images.begin() + pos);
    }

    if (m_folder_images.size())
    {
        m_current_index = std::clamp(m_current_index, 0, (int)m_folder_images.size() - 1);
        return m_folder + m_folder_images[m_current_index];
    }

    m_folder = L"";
    m_folder_images.clear();
    m_current_index = -1;
    return L"";
}

void CCurrentFolderImage::OnExecuteTaskFinish(CAsyncTask* task)
{
    CWin32FindFile   & find = *((CSearchFolderImagesTask*)task)->m_result;
    if (find.m_current_index != -1)
    {
        m_current_index = find.m_current_index;
        m_folder_images = find.m_image_names;

        theApp.GetPreloadCache().AddPreLoadTask(m_folder, m_current_index, m_folder_images);
        ((CMainDlg*)AfxGetMainWnd())->OnFolderImagesSearchFinish();
    }
    else { ASSERT(FALSE); }
}
