#include "cpppage.h"
#include "action.h"
#include "database.h"

void adminPassword(Page * page)
{
    if(page->session("auth").empty())
    {
        page->redirect("./admin_login");
        return;
    }

    page->response("error_display", "none");
    page->response("error_msg", "");

    if(page->form()->method() == "POST")
    {
        String old_pass = page->form()->post("old_pass").encode();
        String new_pass = page->form()->post("new_pass").encode();
        if(!old_pass.empty() && !new_pass.empty())
        {
            db->query("select id from user where username='" + page->session("auth") + "' and password=md5('" + old_pass + "')");
            if(db->next())
            {
                db->exec("update user set password=md5('" + new_pass + "') where username='" + page->session("auth") + "'");
                page->redirect("./admin_setting");
                return;
            }
            else
            {
                page->response("error_display", "block");
                page->response("error_msg", "Old password error");
            }
        }
    }

    page->response("html_head", "<script type='text/javascript'>$(function(){$('#menu_setting').addClass('active');});</script>");
    page->render("admin/password.tmpl");
}
