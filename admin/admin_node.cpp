#include <cstdlib>
#include "cpppage.h"
#include "action.h"
#include "database.h"
#include "paging.h"

void adminNode(Page * page)
{
    if(page->session("auth").empty())
    {
        page->redirect("./admin_login");
        return;
    }

    // 删除节点
    String action = page->form()->get("action");
    if(action == "delete")
    {
        String id = page->form()->get("id").encode();
        if(!id.empty())
            db->exec(String("delete from node where id=") + id);
    }

    // 获取搜索参数
    String key = page->form()->get("key_word").encode();
    String type = page->form()->get("type").encode();
    String author = page->form()->get("author").encode();
    String enabled = page->form()->get("enabled").encode();
    String disabled = page->form()->get("disabled").encode();

    String type_option;
    db->query("select id,name from node_type where status='Y'");
    while(db->next())
    {
        String type_id = db->getString(0);
        type_option += "<option value=\"" + type_id + "\"" + (type_id == type ? " selected=\"selected\"" : "") + ">" + db->getString(1) + "</option>";
    }

    String user_option;
    db->query("select id,nickname from user where status='Y'");
    while(db->next())
    {
        String user_id = db->getString(0);
        user_option += "<option value=\"" + user_id + "\"" + (user_id == author ? " selected=\"selected\"" : "") + ">" + db->getString(1) + "</option>";
    }

    // 数据分页处理
    int page_index = page->form()->get("page").toLong();
    int data_count = 0;
    if(db->query("select count(*) from node") && db->next())
        data_count = db->getInt(0);
    Paging paging(page_index, 15, data_count);
    page->response("preview_page", String::number(paging.pageIndex() - 1));
    page->response("next_page", String::number(paging.pageIndex() + 1));
    page->response("last_page", String::number(paging.pageCount()));

    String node_list(1024);
    String sql = "select node.id,node.title,node_type.name,user.nickname,node.create_time,node.status,node_type.type from node,node_type,user where node.node_type_id=node_type.id and node.user_id=user.id";
    if(!key.empty())
        sql += String(" and node.title like '%") + key + "%'";
    if(!type.empty() && type != "-1")
        sql += String(" and node.node_type_id=") + type;
    if(!author.empty() && author != "-1")
        sql += String(" and node.user_id=") + author;

    if(enabled == "1" && disabled != "1")
        sql += " and node.status='Y'";
    else if(enabled != "1" && disabled == "1")
        sql += " and node.status='N'";
    sql += " order by node.top desc,node.create_time desc limit " + String::number(paging.dataIndex()) + "," + String::number(paging.pageSize());
    if(db->query(sql))
    {
        while(db->next())
        {
            String id = db->getString(0);
            node_list += "<tr><td>" + id + "</td><td>" + db->getString(1) + "</td><td>" + db->getString(2) + "</td><td>" + db->getString(3) + "</td><td>" + db->getString(4);
            if(db->getString(5) == "Y")
                node_list += "</td><td><span class=\"label label-success\">Enabled</span></td>";
            else
                node_list += "</td><td><span class=\"label label-warning\">Disabled</span></td>";
            node_list += "<td><a title=\"Edit\" href=\"./admin_node_modify?action=update&id=" + id + "\"><i class=\"icon-pencil\"></i></a> <a title=\"View\" href=\"./node?node_id=" + id + "\" target=\"_black\"><i class=\"icon-search\"></i></a> <a title=\"Delete\" href=\"?action=delete&id=" + id + "\" onclick=\"return confirm('Delete current node ?');\"><i class=\"icon-trash\"></i></a></td></tr>";
        }
    }

    page->response("html_head", "<script type='text/javascript'>$(function(){$('#menu_node').addClass('active');});</script>");
    page->response("key_word", key.decode());
    page->response("type_option", type_option);
    page->response("user_option", user_option);
    page->response("node_list", node_list);
    if(enabled == "1")
        page->response("enabled", "checked=\"checked\"");
    else
        page->response("enabled", "");
    if(disabled == "1")
        page->response("disabled", "checked=\"checked\"");
    else
        page->response("disabled", "");
    page->render("admin/node.tmpl");
}
