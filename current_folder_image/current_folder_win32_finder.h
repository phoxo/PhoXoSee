#pragma once

namespace search_folder_image
{
    class CWin32FindFile
    {
    private:
        set<CString>   m_subfolder;
    public:
        deque<CString>   m_image_names;
        int   m_current_index = -1;

    public:
        CWin32FindFile(CString folder, PCWSTR current_file)
        {
            CFileFind   fd;
            BOOL   working = fd.FindFile(folder + L"*");
            while (working)
            {
                working = fd.FindNextFile();
                CString   file_name = fd.GetFileName();
                if (fd.IsDirectory())
                {
                    m_subfolder.insert(file_name);
                }
                else
                {
                    if (IsSupportedImage(file_name) ||
                        (StrCmpI(file_name, current_file) == 0)) // <- 比如把一张jpg后缀改成aaa，拖进来能打开，这里加入队列防止缺失
                    {
                        m_image_names.push_back(file_name);
                    }
                }
            }

            // default sort by filename
            std::sort(m_image_names.begin(), m_image_names.end(), [](const CString& a, const CString& b) {
                return (StrCmpLogicalW(a, b) == -1);
            });
        }

        void MergeExplorerFind(const deque<CString>& explorer_finds, PCWSTR current_name)
        {
            deque<CString>   ok_names;
            for (auto& iter : explorer_finds)
            {
                if (m_subfolder.find(iter) != m_subfolder.end())
                    continue;
                if (IsSupportedImage(iter) ||
                    (StrCmpI(iter, current_name) == 0))
                {
                    ok_names.push_back(iter);
                }
            }
            ASSERT(m_image_names.size() >= ok_names.size()); // explorer可能不显示隐藏文件

            int   pos = CStringHelper::FindNoCase(ok_names, current_name); // 如果打开的explorer里找到了
            if (pos != -1)
            {
                m_current_index = pos;
                m_image_names.swap(ok_names);
            }
            else
            {
                m_current_index = CStringHelper::FindNoCase(m_image_names, current_name); // 没有打开explorer的情况
            }
        }

    private:
        static BOOL IsSupportedImage(PCWSTR file_name)
        {
            if (CWICInstalledCodec::FindContainerFormat(file_name) != CLSID_NULL)
                return TRUE;

            auto   img_type = CImageFileExtParser::GetType(file_name); // TGA, PSD之类系统肯定不支持的
            return (img_type != ImageFormat::IF_UNKNOWN);
        }
    };
}
