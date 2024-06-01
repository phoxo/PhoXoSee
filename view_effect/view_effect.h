#pragma once
 
class CViewEffect
{
private:
    unique_ptr<FCProgressObserver>   m_progress_monitor;

public:
    CViewEffect(int* task_valid_to_stop = nullptr);

    void ApplyEffect(IWICBitmapPtr src);
    void ApplyEffect(FCImage& img);
};