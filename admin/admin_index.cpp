#include "cpppage.h"
#include "action.h"
#include "database.h"
#include "cpplog.h"

void adminIndex(Page * page)
{
    String id = page->session("auth");
    // 没有登录，跳转
    if(id.empty())
    {
        page->redirect("./admin_login");
        return;
    }

    if(db->query("select nickname,last_login_time,last_login_ip from user where id=" + id) && db->next())
    {
        page->response("nickname", db->getString(0));
        page->response("last_login_time", db->getString(1));
        page->response("last_login_ip", db->getString(2));
        page->render("admin/index.tmpl");
    }
    else
    {
        CPP_ERROR("Can't get user infomation");
        page->redirect("./admin_login");
    }
}
