/*
    Copyright (C) =PhoXo= Fu Li

    Author   :  Fu Li
    Create   :  2021 Apr.16
    Home     :  http://www.phoxo.com
    Mail     :  atphoxo@gmail.com

    This file is part of PhoXo

    This code is licensed under the terms of the MIT license.
*/
#pragma once

namespace search_folder_image
{
    // 找到当前打开的资源管理器，给定目录，收集所有文件和文件夹
    class COpenedExplorerSort
    {
    public:
        deque<CString>   m_found_files; // folder所有显示的文件和文件夹，如隐藏不显示不会列出来

        COpenedExplorerSort(CString folder, BOOL& continue_enum);
    };
}
