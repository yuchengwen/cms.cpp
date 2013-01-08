#include <cstdlib>
#include "cpppage.h"
#include "cppform.h"
#include "action.h"
#include "database.h"
#include "paging.h"

void articleList(Page * page)
{
    // 数据分页处理
    int page_index = page->form()->get("page").toLong();
    int data_count = 0;
    if(db->query("select count(*) from node,node_type where node.status='Y' and node.node_type_id=node_type.id and node_type.type=" + String::number(TypeArticle)) && db->next())
        data_count = db->getInt(0);
    Paging paging(page_index, 16, data_count);

    String type_id = page->form()->get("type_id").encode();
    String sql = "select node.id,node.title,node.create_time from node,node_type where node.status='Y' and node.node_type_id=node_type.id and node_type.type=" + String::number(TypeArticle);
    if(!type_id.empty())
        sql += " and node.node_type_id=" + type_id;
    sql += " order by node.top,node.create_time desc limit " + String::number(paging.dataIndex()) + "," + String::number(paging.pageSize());

    int i = 0;
    String article_list(1024);
    if(db->query(sql))
    {
        while(db->next())
        {
            if(i == 0)
                article_list += "<table class=\"table table-striped\"><thead><tr><th>Title</th><th style=\"width:150px;\">Time</th></tr></thead><tbody>";
            article_list += "<tr><td><a href=\"./node?node_id=" + db->getString(0) + "\">" + db->getString(1) + "</a></td><td>" + db->getString(2) + "</td></tr>";
            i++;
        }
        if(i > 0)
            article_list += "</body></table><ul class=\"pager\"><li><a href=\"?page=1\">First</a></li><li><a href=\"?page=" + String::number(paging.pageIndex() - 1) + "\">Preview</a></li><li><a href=\"?page=" + String::number(paging.pageIndex() + 1) + "\">Next</a></li><li><a href=\"?page=" + String::number(paging.pageCount()) + "\">Last</a></li></ul>";
        else
            article_list = "<div class=\"alert alert-error\" style=\"text-align:center\">No content</div>";
    }

    if(!type_id.empty() && db->query("select name from node_type where id=" + type_id) && db->next())
        page->response("html_title", db->getString(0));
    else
        page->response("html_title", "Article Center");
    page->response("node_list", article_list);
    page->response("side_type_list", sideProductType());
    page->response("side_art_list", sideLastAricle(10));
    page->response("side_product_list", sideLastProduct(10));
    page->render("node_list.tmpl");
}
