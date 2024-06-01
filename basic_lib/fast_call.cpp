#include "pch.h"
#include "fast_call.h"

void CFastCall::PostMainWndCommand(int cmd)
{
    ::PostMessage(*AfxGetMainWnd(), WM_COMMAND, MAKEWPARAM(cmd, 0), 0);
}
