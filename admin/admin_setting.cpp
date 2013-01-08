#include "cpppage.h"
#include "action.h"
#include "database.h"

void adminSetting(Page * page)
{
    if(page->session("auth").empty())
    {
        page->redirect("./admin_login");
        return;
    }

    page->response("title", "");
    page->response("logo", "");
    page->response("keywords", "");
    page->response("description", "");

    if(page->form()->method() == "POST")
    {
        String title = page->form()->post("title").encode();
        String logo = page->form()->post("logo").encode();
        String keywords = page->form()->post("keywords").encode();
        String description = page->form()->post("description").encode();

        String sql = "update settings \
                     set value = case name \
                     when 'web_title' then '" + title + "' \
                     when 'web_logo' then '" + logo + "' \
                     when 'web_keywords' then '" + keywords + "' \
                     when 'web_description' then '" + description + "' \
                     end \
                     where name in ('web_title','web_logo','web_keywords','web_description')";
        db->exec(sql);
    }

    db->query("select name,value from settings where name in ('web_title','web_logo','web_keywords','web_description')");
    while(db->next())
    {
        if(db->getString(0) == "web_title")
            page->response("setting_title", db->getString(1));
        if(db->getString(0) == "web_logo")
            page->response("setting_logo", db->getString(1));
        if(db->getString(0) == "web_keywords")
            page->response("setting_keywords", db->getString(1));
        if(db->getString(0) == "web_description")
            page->response("setting_description", db->getString(1));
    }

    page->response("html_head", "<script type='text/javascript'>$(function(){$('#menu_setting').addClass('active');});</script>");
    page->render("admin/setting.tmpl");
}
