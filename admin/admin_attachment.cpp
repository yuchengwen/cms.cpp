#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "cppplatform.h"
#ifdef CPP_WIN
#include <io.h>
#endif
#include "cpppage.h"
#include "cppform.h"
#include "action.h"
#include "database.h"
#include "cpplog.h"
#include "paging.h"

void adminAttachment(Page * page)
{
    if(page->session("auth").empty())
    {
        page->redirect("./admin_login");
        return;
    }

    page->response("html_head", "<script type='text/javascript'>$(function(){$('#menu_attachment').addClass('active');});</script>");
    page->response("error_display", "none");
    page->response("error_msg", "");

    if(page->form()->method() == "POST")
    {
        String description = page->form()->post("description").encode();
        Status status = page->form()->uploadStatus("file");
        if(status == Oversize)
        {
            page->response("error_display", "block");
            page->response("error_msg", "File size must small then " + String::number(Form::maxUpdateSize()) + " KB");
        }
        else if(status == Saved)
        {
            page->response("error_display", "block");
            page->response("error_msg", "File has saved");
        }
        else if(status == Failed)
        {
            page->response("error_display", "block");
            page->response("error_msg", "Upload file failed");
        }
        else
        {
            String url = page->saveUploadFile("file");
            if(!url.empty())
                db->exec("insert into attachment (description,url,size,user_id) values ('" + description + "','" + url + "'," + String::number(page->form()->uploadSize("file")) + "," + page->session("auth") + ")");
        }
    }
    else
    {
        // 删除附件
        String action = page->form()->get("action");
        if(action == "delete")
        {
            String id = page->form()->get("id").encode();
            if(!id.empty())
            {
                // 先删除文件
                if(db->query("select url from attachment where id=" + id) && db->next())
                {
                    String file = page->documentRoot() + db->getString(0);
                    if(access(file.data(), 0) != 0 || remove(file.data()) != 0)
                        CPP_ERROR("Remove file failed: %s", file.data());
                }
                db->exec(String("delete from attachment where id=") + id);
            }
        }
    }

    // 数据分页处理
    int page_index = page->form()->get("page").toLong();
    int data_count = 0;
    if(db->query("select count(*) from attachment") && db->next())
        data_count = db->getInt(0);
    Paging paging(page_index, 15, data_count);
    page->response("preview_page", String::number(paging.pageIndex() - 1));
    page->response("next_page", String::number(paging.pageIndex() + 1));
    page->response("last_page", String::number(paging.pageCount()));

    String file_list;
    db->query("select attachment.id,attachment.url,attachment.description,attachment.size,attachment.create_time,user.nickname from attachment,user where attachment.user_id=user.id order by attachment.create_time desc limit " + String::number(paging.dataIndex()) + "," + String::number(paging.pageSize()));
    while(db->next())
    {
        file_list += "<tr><td>" + db->getString(0) + "</td><td>" + db->getString(2) + "</td><td>" + db->getString(1) + "</td><td>" + db->getString(3) + "</td><td>" + db->getString(4) + "</td><td>" + db->getString(5) + "</td><td><a title=\"Download\" href=\"" + db->getString(1) + "\"><i class=\"icon-download-alt\"></i></a> <a title=\"Delete\" href=\"?action=delete&id=" + db->getString(0) + "\" onclick=\"return confirm('Delete current file ?');\"><i class=\"icon-trash\"></i></a></td></tr>";
    }
    page->response("file_list", file_list);
    page->render("admin/attachment.tmpl");
}
