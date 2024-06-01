#pragma once

class CDisplayAreaLocator
{
private:
    const CSize   m_view_size;
    CSize   m_image_zoomed_size;
    CRect   m_display_area_on_view; // only change as the zoom changed
    CPoint   m_max_offset; // 可以理解为滚动条的最大滚动位置
    CRect   m_rect_on_zoomed_image; // left, top可以理解为滚动条的位置

public:
    CDisplayAreaLocator(CSize view_size) : m_view_size(view_size)
    {
    }

    void Reset(CSize zoomed_size)
    {
        m_image_zoomed_size = zoomed_size;
        CSize   diff = m_view_size - zoomed_size;
        m_display_area_on_view.IntersectRect(CRect(CPoint(), m_view_size), CRect(CPoint(diff.cx/2, diff.cy/2), zoomed_size));

        m_max_offset.x = ((diff.cx >= 0) ? 0 : (-diff.cx));
        m_max_offset.y = ((diff.cy >= 0) ? 0 : (-diff.cy));
        UpdateViewOffset(Gdiplus::PointF(), CPoint());
    }

    CSize GetImageZoomedSize() const
    {
        return m_image_zoomed_size;
    }

    CRect GetWholeImageOnViewArea() const
    {
        CPoint   pt = ZoomedImageToView(CPoint(0, 0));
        return CRect(pt, m_image_zoomed_size);
    }

    void UpdateViewOffset(Gdiplus::PointF anchor_on_zoomed_image, CPoint anchor_on_view)
    {
        m_rect_on_zoomed_image = CRect(CPoint(), m_image_zoomed_size);
        if (m_max_offset.x)
        {
            m_rect_on_zoomed_image.left = std::clamp((int)round(anchor_on_zoomed_image.X) - anchor_on_view.x, 0L, m_max_offset.x);
            m_rect_on_zoomed_image.right = m_rect_on_zoomed_image.left + m_view_size.cx;
        }
        if (m_max_offset.y)
        {
            m_rect_on_zoomed_image.top = std::clamp((int)round(anchor_on_zoomed_image.Y) - anchor_on_view.y, 0L, m_max_offset.y);
            m_rect_on_zoomed_image.bottom = m_rect_on_zoomed_image.top + m_view_size.cy;
        }
    }

    CSize OnViewDragged(CSize move_delta)
    {
        CRect   old_area = m_rect_on_zoomed_image;
        m_rect_on_zoomed_image.OffsetRect(-move_delta);
        BoundInImage();
        return old_area.TopLeft() - m_rect_on_zoomed_image.TopLeft();
    }

    CPoint ZoomedImageToView(CPoint pt) const
    {
        pt.x += (m_max_offset.x ? (-m_rect_on_zoomed_image.left) : m_display_area_on_view.left);
        pt.y += (m_max_offset.y ? (-m_rect_on_zoomed_image.top) : m_display_area_on_view.top);
        return pt;
    }

    CPoint ViewToZoomedImage(CPoint pt) const
    {
        pt.x -= (m_max_offset.x ? (-m_rect_on_zoomed_image.left) : m_display_area_on_view.left);
        pt.y -= (m_max_offset.y ? (-m_rect_on_zoomed_image.top) : m_display_area_on_view.top);
        return pt;
    }

private:
    void BoundInImage()
    {
        int   dx = 0, dy = 0;
        if (m_rect_on_zoomed_image.left < 0)
            dx = -m_rect_on_zoomed_image.left;
        else if (m_rect_on_zoomed_image.right > m_image_zoomed_size.cx)
            dx = m_image_zoomed_size.cx - m_rect_on_zoomed_image.right;

        if (m_rect_on_zoomed_image.top < 0)
            dy = -m_rect_on_zoomed_image.top;
        else if (m_rect_on_zoomed_image.bottom > m_image_zoomed_size.cy)
            dy = m_image_zoomed_size.cy - m_rect_on_zoomed_image.bottom;

        m_rect_on_zoomed_image.OffsetRect(dx, dy);
    }
};