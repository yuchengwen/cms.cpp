#include <cstring>
#include <ctime>
#include "cpppage.h"
#include "database.h"
#include "action.h"
#include "cpplog.h"

// 后台登陆
void adminLogin(Page * page)
{
    // 注销
    if(!page->form()->get("logout").empty())
    {
        if(!page->session("auth").empty())
            page->setSession("auth", "");
        page->redirect("./admin_login");
        return;
    }

    page->response("error_display", "none");
    page->response("error_msg", "");

    // 登陆
    if(page->form()->method() == "GET")
        page->render("admin/login.tmpl");
    else
    {
        String username = page->form()->post("username").encode();
        String pass = page->form()->post("password").encode();
        String remember = page->form()->post("remember");

        db->query("select id from user where username='" + username + "' and password=md5('" + pass + "') and status='Y'");
        if(db->next())
        {
            String id = db->getString(0);
            // 更新最后登陆时间和IP
            db->exec("update user set last_login_time='" + timeToString(time(0)) + "',last_login_ip='" + page->clientAddr() + "' where id=" + id);
            // 保存会话
            if(remember == "1")
                page->setSession("auth", id, 3600 * 24 * 30);
            else
                page->setSession("auth", id);
            page->redirect("./admin_index");
        }
        else
        {
            // 登陆失败，写入日志
            CPP_ERROR("%s try to login as %s:%s", page->clientAddr().data(), username.data(), pass.data());
            page->response("error_display", "block");
            page->response("error_msg", "Username or password error!");
            page->render("admin/login.tmpl");
        }
    }
}
