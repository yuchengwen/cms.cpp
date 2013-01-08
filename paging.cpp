#include <cstdlib>
#include "paging.h"

Paging::Paging(int page_index, int page_size, int data_count)
{
    _page_index = 0;
    _page_size = 0;
    _page_count = 0;
    if(data_count)
    {
        _page_index = page_index;
        _page_size = page_size;

        if(data_count % _page_size)
            _page_count = data_count / _page_size + 1;
        else
            _page_count = data_count / _page_size;
    
        if(_page_index < 1)
            _page_index = 1;
        if(_page_index > _page_count)
            _page_index = _page_count;
    }
}
