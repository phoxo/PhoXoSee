#pragma once

/// Soft glow (32 bit).
class FCEffectSoftGlow : public FCImageEffect
{
private:
    FCImage   m_bak;
    FCEffectBrightnessContrast   m_bc;
    FCEffectGaussianBlur   m_first_blur;

public:
    /**
        Constructor \n
        param radius >= 1 \n
        param -100 <= brightness <= 100 \n
        param -100 <= contrast <= 100
    */
    FCEffectSoftGlow(int radius, int brightness, int contrast) : m_first_blur(radius, true), m_bc(brightness, contrast)
    {
    }

private:
    virtual bool IsSupport(const FCImage& img)
    {
        return (img.ColorBits() == 32) && !img.IsPremultiplied();
    }

    virtual void OnBeforeProcess(FCImage& img)
    {
        m_bak = img;

        m_first_blur.EnableParallelAccelerate(IsParallelAccelerateEnable());
        img.ApplyEffect(m_first_blur);

        m_bc.OnBeforeProcess(img);
    }

    virtual void ProcessPixel(FCImage& img, int x, int y, RGBA32bit* pixel)
    {
        m_bc.ProcessPixel(img, x, y, pixel);

        auto   oldpx = m_bak.GetBits(x, y);
        auto   newpx = (BYTE*)pixel;
        for (int i = 0; i < 3; i++)
        {
            newpx[i] = 255 - (255 - oldpx[i])*(255 - newpx[i]) / 255;
        }
        pixel->a = oldpx[3];
    }
};
