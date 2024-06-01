#include "pch.h"
#include "PhoXoSee.h"
#include "view_effect.h"
#include "soft_glow.h"

namespace
{
    class CEffectProgress : public FCProgressObserver
    {
        int   * m_task_valid;
    public:
        CEffectProgress(int* v) : m_task_valid(v) {}
        virtual bool OnProgressUpdate(int nFinishPercentage)
        {
            return (*m_task_valid != 0);
        }
    };
};

CViewEffect::CViewEffect(int* task_valid_to_stop)
{
    if (task_valid_to_stop)
    {
        m_progress_monitor = make_unique<CEffectProgress>(task_valid_to_stop);
    }
}

void CViewEffect::ApplyEffect(IWICBitmapPtr src)
{
    if (theConfig.m_runtime_effect == ViewEnhance::None)
        return;

    CWICLockedPixelBuffer   buf(src);
    if (!buf.IsFormat32bpp())
        return;

    FCImage   tmp;
    tmp.Attach32bppBuffer(buf.m_width, -1 * buf.m_height, buf.m_data);
    ApplyEffect(tmp);
    tmp.Release32bppBuffer();
}

void CViewEffect::ApplyEffect(FCImage& img)
{
    deque<unique_ptr<FCImageEffect>>   effects;
    int   curr = (int)theConfig.m_runtime_effect;
    if (curr & (int)ViewEnhance::Brightness)
    {
        effects.push_back(make_unique<FCEffectBrightnessContrast>(20, 0));
    }
    if (curr & (int)ViewEnhance::Saturation)
    {
        effects.push_back(make_unique<FCEffectHueSaturation>(0, 20));
    }
    if (curr & (int)ViewEnhance::Sharpen)
    {
        effects.push_back(make_unique<FCEffectUNSharpMask>(2, 50, 0));
    }
    if (curr & (int)ViewEnhance::SoftGlow)
    {
        effects.push_back(make_unique<FCEffectSoftGlow>(2, -50, -50));
    }

    for (auto& iter : effects)
    {
        iter->EnableParallelAccelerate(true);
        img.ApplyEffect(*iter, m_progress_monitor.get());
    }
}
