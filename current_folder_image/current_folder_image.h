#pragma once

class CCurrentFolderImage : public CAsyncTaskQueue
{
private:
    CString   m_folder; /*   WITH backslash, C:\test\   */
    deque<CString>   m_folder_images; // a.jpg, b.jpg ...
    int   m_current_index = -1;

public:
    void SearchFolderImages(PCWSTR current_image_path);

    BOOL GetCurrentImagePosition(int& current_index, int& total_images) const;
    BOOL IsNeighborImageExist(ViewDirection direction) const;
    CString PrepareViewNeighbor(ViewDirection direction) const;
    CString OnAfterImageDeleted(PCWSTR image_path);

private:
    BOOL NewImageInSameFolderUpdate(PCWSTR new_filename);

    virtual void OnExecuteTaskFinish(CAsyncTask* task);
};
