#ifndef _CMS_PAGING_H_
#define _CMS_PAGING_H_

// ���ݷ�ҳ��
class Paging
{
public:
    Paging(int page_index, int page_size, int data_count);

    // ҳ��
    inline int pageIndex()
    {
        return _page_index;
    }

    // ���ݿ�ʼ����
    inline int dataIndex()
    {
        return (_page_index - 1) * _page_size;
    }

    // ÿһҳ��ʾ��������
    inline int pageSize()
    {
        return _page_size;
    }

    // ҳ��
    inline int pageCount()
    {
        return _page_count;
    }

private:
    // ҳ��
    int _page_index;
    // ÿһҳ����ʾ��������Ŀ
    int _page_size;
    // ҳ����
    int _page_count;
};

#endif
