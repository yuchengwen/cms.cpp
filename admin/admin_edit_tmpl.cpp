#include <cstdio>
#include <cstring>
#include "cpppage.h"
#include "action.h"
#include "database.h"
#include "cppplatform.h"
#ifdef CPP_WIN
#include <io.h>
#endif

void adminEditTmpl(Page * page)
{
    if(page->session("auth").empty())
    {
        page->redirect("./admin_login");
        return;
    }

    page->response("tmpl_name", "");
    page->response("tmpl_content", "");

    if(page->form()->method() == "POST")
    {
        String file = page->form()->post("tmpl_name");
        if(!file.empty())
        {
            String content = page->form()->post("content");
            FILE * tmpl = fopen(file.data(), "w");
            if(tmpl)
            {
                fputs(content.data(), tmpl);
                fclose(tmpl);
                page->response("tmpl_name", "Save file successful");
                page->response("tmpl_content", "Save file successful");
            }
        }
    }

    String file = page->form()->get("file");
    if(!file.empty())
    {
        file = "./tmpl/" + file + ".tmpl";
        if(access(file.data(), 0) == 0)
        {
            FILE * tmpl = fopen(file.data(), "r");
            if(tmpl)
            {
                String html(4096);
                char buf[512] = {0};
                while(fgets(buf, sizeof(buf), tmpl))
                {
                    html += buf;
                    memset(buf, 0, sizeof(buf));
                }
                page->response("tmpl_name", file);
                page->response("tmpl_content", html);

                fclose(tmpl);
            }
        }
    }

    page->response("html_head", "<script type='text/javascript'>$(function(){$('#menu_setting').addClass('active');});</script>");
    page->render("admin/tmpl_edit.tmpl");
}
