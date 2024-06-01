#pragma once

enum class ViewDirection
{
    Next,
    Previous,
};

enum class ToolbarDockStyle
{
    Top = 0,
    Bottom = 1,
};

enum class MouseWheelAction
{
    TurnPage = 0,
    Zoom = 1,
};

enum class DoubleClickAction
{
    Zoom2X = 0,
    Zoom100 = 1,
};

enum class GPSMapProvider
{
    Bing = 0,
    Google = 1,
};

enum class ViewEnhance
{
    None = 0,
    Brightness = 0x1,
    Saturation = 0x10,
    Sharpen = 0x100,
    SoftGlow = 0x1000,
};

enum class TitleColorTheme
{
    FollowSystem = 0,
    Light = 1,
    Dark = 2,
};

enum class TurnPageArrow
{
    Big = 0,
    Small = 1,
};

_COM_SMARTPTR_TYPEDEF(ID2D1DCRenderTarget, __uuidof(ID2D1DCRenderTarget));
_COM_SMARTPTR_TYPEDEF(ID2D1RenderTarget, __uuidof(ID2D1RenderTarget));
_COM_SMARTPTR_TYPEDEF(ID2D1Bitmap, __uuidof(ID2D1Bitmap));
_COM_SMARTPTR_TYPEDEF(ID2D1DeviceContext, __uuidof(ID2D1DeviceContext));

#define  MSG_CREATE_ZOOM_KEY_FRAME   (WM_APP + 501)
#define  MSG_REMOVE_LOADING_TIP       (WM_APP + 502)
#define  MSG_CREATE_FIT_VIEW          (WM_APP + 506)
#define  MSG_LOAD_ANIMATION          (WM_APP + 507)
#define  MSG_SELECT_COLOR_THEME       (WM_APP + 508)
#define  MSG_POST_LOAD_FIRST          (WM_APP + 510)
#define  MSG_FORWARD_COMMAND_TO_EXIST    (WM_APP + 511)
#define  MSG_OPEN_FOUND_IMAGE    (WM_APP + 512)
