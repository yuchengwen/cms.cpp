#include <cstdlib>
#include "cpppage.h"
#include "action.h"
#include "database.h"
#include "paging.h"

// 每行的产品数
#define COLUMN_COUNT 4

void productList(Page * page)
{
    // 数据分页处理
    int page_index = page->form()->get("page").toLong();
    int data_count = 0;
    if(db->query("select count(*) from node,node_type where node.status='Y' and node.node_type_id=node_type.id and node_type.type=" + String::number(TypeProduct)) && db->next())
        data_count = db->getInt(0);
    Paging paging(page_index, 16, data_count);

    int type_id = page->form()->get("type_id").toLong();
    String sql = "select node.id,node.title,node.snapshot from node,node_type where node.status='Y' and node.node_type_id=node_type.id and node_type.type=" + String::number(TypeProduct);
    if(type_id > 0)
        sql += " and node.node_type_id=" + String::number(type_id);
    sql += " order by node.top,node.create_time desc limit " + String::number(paging.dataIndex()) + "," + String::number(paging.pageSize());

    int i = 0;
    String product_list(1024);
    if(db->query(sql))
    {
        while(db->next())
        {
            String title = db->getString(1);
            if(i == 0)
                product_list += "<div class=\"row-fluid\">";
            if(i % COLUMN_COUNT == 0)
                product_list += "<ul class=\"thumbnails\">";
            product_list += "<li class=\"span3\"><div><a href=\"./node?node_id=" + db->getString(0) + "\" class=\"thumbnail\"><img src=\"" + db->getString(2) + "\" alt=\"" + title + "\" /><p style=\"text-align:center\">" + title + "</p></a></div></li>";
            if(i % COLUMN_COUNT == COLUMN_COUNT - 1)
                product_list += "</ul>";
            i++;
        }

        // 最后一行没有满
        if(i % COLUMN_COUNT != 0)
            product_list += "</ul>";

        if(i > 0)
        {
            String str = String::number(type_id);
            product_list += "</div><ul class=\"pager\"><li><a href=\"?type_id=" + str + "&page=1\">First</a></li><li><a href=\"?type_id=" + str + "&page=" + String::number(paging.pageIndex() - 1) + "\">Preview</a></li><li><a href=\"?type_id=" + str + "&page=" + String::number(paging.pageIndex() + 1) + "\">Next</a></li><li><a href=\"?type_id=" + str + "&page=" + String::number(paging.pageCount()) + "\">Last</a></li></ul>";
        }
        else
            product_list = "<div class=\"alert alert-error\" style=\"text-align:center\">No product</div>";
    }



    if(type_id > 0 && db->query("select name from node_type where id=" + String::number(type_id)) && db->next())
        page->response("html_title", db->getString(0));
    else
        page->response("html_title", "Prodect Center");
    page->response("node_list", product_list);
    page->response("side_type_list", sideProductType(type_id));
    page->response("side_art_list", sideLastAricle(10));
    page->response("side_product_list", sideLastProduct(10));
    page->render("node_list.tmpl");
}
