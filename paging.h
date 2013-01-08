#ifndef _CMS_PAGING_H_
#define _CMS_PAGING_H_

// 数据分页类
class Paging
{
public:
    Paging(int page_index, int page_size, int data_count);

    // 页码
    inline int pageIndex()
    {
        return _page_index;
    }

    // 数据开始索引
    inline int dataIndex()
    {
        return (_page_index - 1) * _page_size;
    }

    // 每一页显示的数据数
    inline int pageSize()
    {
        return _page_size;
    }

    // 页数
    inline int pageCount()
    {
        return _page_count;
    }

private:
    // 页码
    int _page_index;
    // 每一页所显示的内容数目
    int _page_size;
    // 页总数
    int _page_count;
};

#endif
