#pragma once

class CFileSaveAgent
{
private:
    CString   m_dest_path;
    CString   m_temp_file;

public:
    CFileSaveAgent(PCWSTR dest_path) : m_dest_path(dest_path)
    {
        CString   ext(PathFindExtension(dest_path));
        m_temp_file = FCFile::GetTempFolder();
        m_temp_file.AppendFormat(L"oxo%d_%d%s", GetTickCount(), FCMath::Rand(), ext);
    }

    ~CFileSaveAgent()
    {
        if (PathFileExists(m_temp_file))
        {
            DeleteFile(m_temp_file);
        }
    }

    const CString& GetAgentFilename() const { return m_temp_file; }

    BOOL ActualReplace()
    {
        if (!PathFileExists(m_temp_file))
            return FALSE;

        FCFile::RemoveReadOnlyAttribute(m_dest_path);
        return CopyFile(m_temp_file, m_dest_path, FALSE);
    }
};
