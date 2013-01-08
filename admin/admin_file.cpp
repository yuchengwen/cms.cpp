#include <cstdio>
#include <cstring>
#include "cppplatform.h"
#ifdef CPP_WIN
#include <io.h>
#endif
#include "cpppage.h"
#include "action.h"

void adminFile(Page * page)
{
    if(page->session("auth").empty())
    {
        page->redirect("./admin_login");
        return;
    }

    page->response("html_head", "<script type='text/javascript'>$(function(){$('#menu_setting').addClass('active');});</script>");
    page->render("admin/file.tmpl");
}
