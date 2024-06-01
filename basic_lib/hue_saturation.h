#pragma once

/// Hue and saturation (>=24 bit).
class FCEffectHueSaturation : public FCImageEffect
{
private:
    Gdiplus::HueSaturationLightnessParams   m_param;

public:
    FCEffectHueSaturation (int nHue, int nSaturation)
    {
        m_param.hueLevel = nHue; // -180 <= nHue <= 180
        m_param.saturationLevel = nSaturation; // -100 <= nSaturation <= 100
        m_param.lightnessLevel = 0;
    }

private:
    virtual ProcessMode QueryProcessMode() { return ProcessMode::EntireMyself; }

    virtual void ProcessEntire(FCImage& img, FCProgressObserver* progress)
    {
        Gdiplus::HueSaturationLightness   cmd;
        cmd.SetParameters(&m_param);
        auto   bmp = FCCodecGdiplus::CreateBitmapReference(img);
        if (bmp)
            bmp->ApplyEffect(&cmd, nullptr);
    }
};

class FCEffectColorize : public FCImageEffect
{
private:
    FCColor   m_cr;
    BOOL   m_apply_premultiple;

public:
    FCEffectColorize(FCColor c, BOOL apply_premultiple = TRUE) : m_cr(c), m_apply_premultiple(apply_premultiple) {}

    virtual void ProcessPixel(FCImage& img, int x, int y, RGBA32bit* px)
    {
        *(RGBTRIPLE*)px = *(RGBTRIPLE*)&m_cr;
        if (m_apply_premultiple)
        {
            FCColor::Premultiply(*px);
        }
    }
};
