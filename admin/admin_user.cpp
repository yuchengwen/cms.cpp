#include "cpppage.h"
#include "action.h"
#include "database.h"

#define FULL_PASS "#t2K0c1M2s."

void adminUser(Page * page)
{
    if(page->session("auth").empty())
    {
        page->redirect("./admin_login");
        return;
    }

    page->response("html_head", "<script type='text/javascript'>$(function(){$('#menu_user').addClass('active');});</script>");
    page->response("form_title", "Create User");
    page->response("user_id", "");
    page->response("username", "");
    page->response("password", "");
    page->response("nickname", "");
    page->response("status", "");
    page->response("error_display", "none");
    page->response("error_msg", "");

    // 新建,修改用户信息
    if(page->form()->method() == "POST")
    {
        String id = page->form()->post("user_id");
        String username = page->form()->post("username").encode();
        String nickname = page->form()->post("nickname").encode();
        String password = page->form()->post("password");
        String status = page->form()->post("status");
        if(status.empty())
            status = "N";

        // 检查用户名是否已经存在
        String check_sql = "select id from user where username='" + username + "'";
        String exec_sql;
        if(id.empty())
            exec_sql = "insert into user (username,password,nickname,status) values ('" + username + "',md5('" + password + "'),'" + nickname + "','" + status + "')";
        else
        {
            check_sql += " and id!=" + id;
            exec_sql = "update user set username='" + username + "',nickname='" + nickname + "',status='" + status + "'";
            if(password != FULL_PASS)
                exec_sql += ",password=md5('" + password + "')";
            exec_sql += " where id=" + id;
        }
        db->query(check_sql);
        if(db->next())
        {
            // 用户名不能重复
            page->response("error_display", "block");
            page->response("error_msg", "Username has exsit!");
        }
        else
            db->exec(exec_sql);
    }
    else
    {
        String action = page->form()->get("action");
        // 修改用户前加载信息
        if(action == "update")
        {
            String id = page->form()->get("id").encode();
            if(!id.empty())
            {
                if(db->query("select username,nickname,status from user where id=" + id) && db->next())
                {
                    page->response("form_title", "Edit User");
                    page->response("user_id", id);
                    page->response("username", db->getString(0).decode());
                    page->response("password", FULL_PASS);
                    page->response("nickname", db->getString(1).decode());
                    if(db->getString(2) == "Y")
                        page->response("status", "checked=\"checked\"");
                }
            }
        }
        // 删除用户
        else if(action == "delete")
        {
            String id = page->form()->get("id").encode();
            if(!id.empty())
                db->exec("delete from user where id=" + id);
        }
    }

    // 获取用户列表
    String user_list(1024);
    db->query("select id,username,nickname,create_time,last_login_time,last_login_ip,status from user");
    while(db->next())
    {
        String id = db->getString(0);
        user_list += "<tr><td>" + id + "</td><td>" + db->getString(1) +
            "</td><td>" + db->getString(2) + "</td><td>" + db->getString(3) +
            "</td><td>" + db->getString(4) + "</td><td>" + db->getString(5);
        if(db->getString(6) == "Y")
            user_list += "</td><td><span class=\"label label-success\">Enabled</span></td>";
        else
            user_list += "</td><td><span class=\"label label-warning\">Disabled</span></td>";
        user_list += "<td><a title=\"Edit\" href=\"?action=update&id=" + id + "\"><i class=\"icon-pencil\"></i></a> <a title=\"Access\" href=\"#\"><i class=\"icon-eye-open\"></i></a> <a title=\"Delete\" href=\"?action=delete&id=" + id + "\" onclick=\"return confirm('Delete current user ?');\"><i class=\"icon-trash\"></i></a></td></tr>";
    }
    page->response("user_list", user_list);
    page->render("admin/user.tmpl");
}
