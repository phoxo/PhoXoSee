#pragma once

class CZoomRatio
{
private:
    struct RATIO_DETAIL
    {
        float   v;
        float   x;
        float   y;
    };

private:
    const CSize   m_actual_size;
    RATIO_DETAIL   m_current_ratio{ 1,1,1 };

public:
    CZoomRatio(CSize actual_size) : m_actual_size(actual_size)
    {
    }

    CSize GetActualSize() const
    {
        return m_actual_size;
    }

    float GetCurrentRatio() const
    {
        return m_current_ratio.v;
    }

    CSize SetCurrentRatio(float new_ratio)
    {
        int   w = (std::max)((int)(m_actual_size.cx * new_ratio), 1);
        int   h = (std::max)((int)(m_actual_size.cy * new_ratio), 1);
        m_current_ratio.v = new_ratio;
        m_current_ratio.x = w / (float)m_actual_size.cx;
        m_current_ratio.y = h / (float)m_actual_size.cy;
        return CSize(w, h);
    }

    Gdiplus::PointF ToActual(CPoint zoomed_position) const
    {
        float   x = zoomed_position.x / m_current_ratio.x;
        float   y = zoomed_position.y / m_current_ratio.y;
        return Gdiplus::PointF(x, y);
    }

    Gdiplus::PointF ToZoomed(Gdiplus::PointF actual_position) const
    {
        actual_position.X *= m_current_ratio.x;
        actual_position.Y *= m_current_ratio.y;
        return actual_position;
    }
};
