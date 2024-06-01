#pragma once

class CWICLockedPixelBuffer
{
public:
    UINT   m_width = 0;
    UINT   m_height = 0;
    IWICBitmapLockPtr   m_lock;
    WICPixelFormatGUID   m_format = GUID_NULL;
    UINT   m_stride = 0;
    UINT   m_buf_size = 0;
    BYTE   * m_data = NULL;

public:
    CWICLockedPixelBuffer(IWICBitmapPtr src)
    {
        try
        {
            src->GetSize(&m_width, &m_height);
            WICRect   rc = { 0, 0, (INT)m_width, (INT)m_height };
            src->Lock(&rc, WICBitmapLockRead | WICBitmapLockWrite, &m_lock);
            m_lock->GetPixelFormat(&m_format);
            m_lock->GetStride(&m_stride);
            m_lock->GetDataPointer(&m_buf_size, &m_data);
        }
        catch (_com_error&) {}
        ASSERT(m_data);
    }

    BOOL IsFormat32bpp() const
    {
        return (m_data && (CWICFunc::GetBitsPerPixel(m_format) == 32));
    }
};
