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
    // �ҵ���ǰ�򿪵���Դ������������Ŀ¼���ռ������ļ����ļ���
    class COpenedExplorerSort
    {
    public:
        deque<CString>   m_found_files; // folder������ʾ���ļ����ļ��У������ز���ʾ�����г���

        COpenedExplorerSort(CString folder, BOOL& continue_enum);
    };
}
