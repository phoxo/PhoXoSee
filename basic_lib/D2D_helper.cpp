#include "pch.h"
#include "PhoXoSee.h"
#include "D2D_helper.h"
#include <d2d1_3.h>

namespace
{
    _COM_SMARTPTR_TYPEDEF(ID2D1DeviceContext5, __uuidof(ID2D1DeviceContext5));
    _COM_SMARTPTR_TYPEDEF(ID2D1SvgDocument, __uuidof(ID2D1SvgDocument));

    D2D1_RENDER_TARGET_PROPERTIES BuildProperties(D2D1_RENDER_TARGET_TYPE type)
    {
        auto   pixel_fmt = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
        return D2D1::RenderTargetProperties(type, pixel_fmt);
    }

    ID2D1DeviceContext5Ptr CreateWicBitmapRenderTarget(IWICBitmap* bmp)
    {
        ID2D1RenderTargetPtr   ret;
        if (bmp) // 传给D2D NULL竟然会crash...
        {
            auto   prop = BuildProperties(D2D1_RENDER_TARGET_TYPE_SOFTWARE);
            theApp.GetD2DFactory()->CreateWicBitmapRenderTarget(bmp, &prop, &ret);
        }
        return ret; // <== 隐式转换
    }
}

ID2D1DCRenderTargetPtr CD2DHelper::CreateDCRenderTarget(D2D1_RENDER_TARGET_TYPE type)
{
    auto   prop = BuildProperties(type);
    ID2D1DCRenderTargetPtr   ret;
    theApp.GetD2DFactory()->CreateDCRenderTarget(&prop, &ret);
    return ret;
}

void CD2DHelper::DrawSVGToBitmap(IWICBitmap* bmp, IStream* svg_str)
{
    // from Windows 10 1703, build 15063. 版本太老返回NULL
    auto   dc5 = CreateWicBitmapRenderTarget(bmp);
    if (dc5 && svg_str)
    {
        ID2D1SvgDocumentPtr   svg;
        dc5->CreateSvgDocument(svg_str, CD2DSizeF(CWICFunc::GetBitmapSize(bmp)), &svg);
        if (svg)
        {
            dc5->BeginDraw();
            dc5->DrawSvgDocument(svg);
            dc5->EndDraw();
        }
    }
}
