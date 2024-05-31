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
                        (StrCmpI(file_name, current_file) == 0)) // <- �����һ��jpg��׺�ĳ�aaa���Ͻ����ܴ򿪣����������з�ֹȱʧ
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
            ASSERT(m_image_names.size() >= ok_names.size()); // explorer���ܲ���ʾ�����ļ�

            int   pos = CStringHelper::FindNoCase(ok_names, current_name); // ����򿪵�explorer���ҵ���
            if (pos != -1)
            {
                m_current_index = pos;
                m_image_names.swap(ok_names);
            }
            else
            {
                m_current_index = CStringHelper::FindNoCase(m_image_names, current_name); // û�д�explorer�����
            }
        }

    private:
        static BOOL IsSupportedImage(PCWSTR file_name)
        {
            if (CWICInstalledCodec::FindContainerFormat(file_name) != CLSID_NULL)
                return TRUE;

            auto   img_type = CImageFileExtParser::GetType(file_name); // TGA, PSD֮��ϵͳ�϶���֧�ֵ�
            return (img_type != ImageFormat::IF_UNKNOWN);
        }
    };
}
