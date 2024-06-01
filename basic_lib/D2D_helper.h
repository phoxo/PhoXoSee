#pragma once

class CD2DHelper
{
public:
    static void BindDCAndBeginDraw(ID2D1DCRenderTarget* render, HDC dc)
    {
        CSize   bmp_size = FCImageDrawDC::GetBitmapSize((HBITMAP)GetCurrentObject(dc, OBJ_BITMAP));
        if (render && bmp_size.cx)
        {
            render->BindDC(dc, CRect(CPoint(), bmp_size));
            render->BeginDraw();
            render->SetTransform(D2D1::IdentityMatrix());
        }
    }

    static ID2D1DCRenderTargetPtr CreateDCRenderTarget(D2D1_RENDER_TARGET_TYPE type);

    static ID2D1BitmapPtr CreateBitmapFromWicBitmap(ID2D1RenderTarget* render, IWICBitmapSource* src_bmp)
    {
        auto   src = CWICFunc::ConvertFormat(src_bmp, WICPremultiplied32bpp);
        ID2D1BitmapPtr   ret;
        if (render && src)
        {
            auto   hr = render->CreateBitmapFromWicBitmap(src, &ret);
            ASSERT(hr == S_OK); // 如果打开硬件加速，图像过大创建会失败返回参数参数无效，可能是内存不够引起的
        }
        ASSERT(ret);
        return ret;
    }

    static void DrawSVGToBitmap(IWICBitmap* bmp, IStream* svg_str);
};
