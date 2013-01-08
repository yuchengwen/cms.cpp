#include <map>
#include <cstdlib>
#include "cpppage.h"
#include "action.h"
#include "database.h"

static map<int, String> _type;

String typeName(int value)
{
    if(_type.find(value) != _type.end())
        return _type[value];
    else
        return "-";
}

void adminType(Page * page)
{
    if(page->session("auth").empty())
    {
        page->redirect("./admin_login");
        return;
    }

    // 初始化节点类型
    _type[TypeArticle] = "Article";
    _type[TypePage] = "Page";
    _type[TypeProduct] = "Product";

    page->response("html_head", "<script type='text/javascript'>$(function(){$('#menu_type').addClass('active');});</script>");
    page->response("form_title", "Create Type");
    page->response("type_id", "");
    page->response("type_name", "");
    page->response("snapshot", "");
    page->response("status", "");
    page->response("error_display", "none");
    page->response("error_msg", "");

    String update_id;
    int update_parent_id = -1;
    int update_type = -1;

    if(page->form()->method() == "POST")
    {
        String id = page->form()->post("type_id");
        String type_name = page->form()->post("type_name").encode();
        String snapshot = page->form()->post("snapshot");
        String type = page->form()->post("type");
        String parent_id = page->form()->post("parent");
        String status = page->form()->post("status");
        if(status.empty())
            status = "N";

        if(id.empty())
            db->exec("insert into node_type (name,snapshot,type,parent_id,status) values ('" + type_name + "','" + snapshot + "'," + type +"," + parent_id + ",'" + status + "')");
        else
            db->exec("update node_type set name='" + type_name + "',snapshot='" + snapshot + "',type=" + type + ",parent_id=" + parent_id + ",status='" + status + "' where id=" + id);
    }
    else
    {
        String action = page->form()->get("action");
        // 修改前加载数据
        if(action == "update")
        {
            update_id = page->form()->get("id").encode();
            if(!update_id.empty())
            {
                if(db->query("select name,snapshot,type,parent_id,status from node_type where id=" + update_id) && db->next())
                {
                    page->response("form_title", "Edit Type");
                    page->response("type_id", update_id);
                    page->response("type_name", db->getString(0).decode());
                    page->response("snapshot", db->getString(1));
                    if(db->getString(4) == "Y")
                        page->response("status", "checked=\"checked\"");

                    update_type = db->getInt(2);
                    update_parent_id = db->getInt(3);
                }
            }
        }
        // 删除类型
        else if(action == "delete")
        {
            String id = page->form()->get("id").encode();
            if(!id.empty())
            {
                // 检查该分类中是否有内容
                if(db->query("select id from node where node_type_id=" + id) && db->next())
                {
                    page->response("error_display", "block");
                    page->response("error_msg", "This type is not empty");
                }
                else
                    db->exec("delete from node_type where id=" + id);
            }
        }
    }

    String type_option;
    for(int i = TypeArticle; i < TypeEnd; i++)
        type_option += "<option value=\"" + String::number(i) + "\"" + String(update_type == i ? " selected=\"selected\"" : "") + ">" + _type[i] +"</option>";

    String parent_option;
    String type_list(1024);
    String sql = "select t1.id,t1.name,t1.type,t2.name,t1.status from node_type t1, node_type t2 where t1.parent_id=t2.id \
                 UNION \
                 select id,name,type,'-',status from node_type where parent_id=-1";
    db->query(sql);
    while(db->next())
    {
        String id = db->getString(0);
        String status = db->getString(4);
        type_list += "<tr><td>" + id + "</td><td>" + db->getString(1) + "</td><td>" + typeName(db->getInt(2)) + "</td><td>" + db->getString(3);
        if(status == "Y")
            type_list += "</td><td><span class=\"label label-success\">Enabled</span></td>";
        else
            type_list += "</td><td><span class=\"label label-warning\">Disabled</span></td>";
        type_list += "<td><a title=\"Edit\" href=\"?action=update&id=" + id + "\"><i class=\"icon-pencil\"></i></a> <a title=\"View\" href=\"./admin_node?type=" + id + "\"><i class=\"icon-search\"></i></a> <a title=\"Delete\" href=\"?action=delete&id=" + id + "\" onclick=\"return confirm('Delete current type ?');\"><i class=\"icon-trash\"></i></a></td></tr>";

        if(id != update_id && status == "Y")
            parent_option += "<option value=\"" + id + "\"" + (update_parent_id == id.toLong() ? " selected=\"selected\"" : "") + ">" + db->getString(1) + "</option>";
    }
    page->response("type_option", type_option);
    page->response("parent_option", parent_option);
    page->response("type_list", type_list);
    page->render("admin/type.tmpl");
}
