#pragma once

class CZoomRatioFactory
{
private:
    float   m_fit_view_ratio;
    float   m_double_click_ratio;

public:
    static float CalculateFitViewRatio(CSize view_size, CSize image_size)
    {
        float   dx = view_size.cx / (float)image_size.cx;
        float   dy = view_size.cy / (float)image_size.cy;
        float   ratio_view = (std::min)(dx, dy);
        return (std::min)(ratio_view, 1.0f);
    }

public:
    CZoomRatioFactory(float init_fit_view_ratio, float double_click_ratio)
    {
        m_fit_view_ratio = init_fit_view_ratio;
        m_double_click_ratio = double_click_ratio;
    }

    float GetFitViewRatio() const
    {
        return m_fit_view_ratio;
    }

    float GetRatioByDoubleClick(float current_ratio) const
    {
        BOOL   current_is_fitview = (current_ratio == m_fit_view_ratio);
        return (current_is_fitview ? m_double_click_ratio : m_fit_view_ratio);
    }

    float GetRatioByMouseWheel(float current_ratio, int wheel_delta, int max_zoom_ratio) const
    {
        float   new_ratio = current_ratio * (1 + 0.1f*wheel_delta);
//        float   min_ratio = min(0.05f, m_fit_view_ratio); // 限制最小5%
        return std::clamp(new_ratio, m_fit_view_ratio, (float)max_zoom_ratio);
    }
};
