#include "cpppage.h"
#include "action.h"
#include "database.h"

void adminRunSql(Page * page)
{
    if(page->session("auth").empty())
    {
        page->redirect("./admin_login");
        return;
    }

    page->response("sql_result", "");
    if(page->form()->method() == "POST")
    {
        String sql = page->form()->post("sql");
        if(!sql.empty())
        {
            if(sql.startsWith("insert") || sql.startsWith("update") || sql.startsWith("delete"))
            {
                if(db->exec(sql))
                    page->response("sql_result", "Exec successful");
                else
                    page->response("sql_result", "Exec failed");
            }
            else
                page->response("sql_result", "Sql must start with 'insert','update' or 'delete'");
        }
    }

    page->response("html_head", "<script type='text/javascript'>$(function(){$('#menu_setting').addClass('active');});</script>");
    page->render("admin/run_sql.tmpl");
}
