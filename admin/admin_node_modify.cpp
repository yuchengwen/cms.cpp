#include "cpppage.h"
#include "action.h"
#include "database.h"

// 加载超文本编辑器
#define TINY_MCE "<script type=\"text/javascript\" src=\"/tiny_mce/tiny_mce.js\"></script><script type=\"text/javascript\">\
tinyMCE.init({\
mode : \"textareas\",\
theme : \"advanced\",\
skin : \"o2k7\",\
plugins : \"autolink,lists,pagebreak,style,layer,table,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,media,searchreplace,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,inlinepopups\",\
theme_advanced_buttons1 : \"bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,styleselect,formatselect,fontselect,fontsizeselect\",\
theme_advanced_buttons2 : \"cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,code,|,insertdate,inserttime,preview,|,forecolor,backcolor\",\
theme_advanced_buttons3 : \"tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,iespell,media,advhr,|,ltr,rtl,|,fullscreen\",\
theme_advanced_toolbar_location : \"top\",\
theme_advanced_toolbar_align : \"left\",\
theme_advanced_statusbar_location : \"bottom\",\
theme_advanced_resizing : true\
});\
</script>"

void adminNodeModify(Page * page)
{
    if(page->session("auth").empty())
    {
        page->redirect("./admin_login");
        return;
    }

    if(page->form()->method() == "POST")
    {
        String id = page->form()->post("node_id");
        String title = page->form()->post("title").encode();
        String type = page->form()->post("type");
        String snapshot = page->form()->post("snapshot");
        String content = page->form()->post("content").filterForSQL();
        String status = page->form()->post("status");
        String comment_enabled = page->form()->post("comment_enabled");
        String comment_display = page->form()->post("comment_display");
        String top = page->form()->post("top");
        if(status.empty())
            status = "N";
        if(comment_enabled.empty())
            comment_enabled = "N";
        if(comment_display.empty())
            comment_display = "N";
        if(top.empty())
            top = "N";

        if(id.empty())
            db->exec("insert into node (title,snapshot,content,node_type_id,user_id,top,comment_enabled,comment_display,status) values ('" + title + "','" + snapshot + "','" + content + "'," + type + "," + page->session("auth") + ",'" + top + "','" + comment_enabled + "','" + comment_display + "','" + status + "')");
        else
            db->exec("update node set title='" + title + "',snapshot='" + snapshot + "',content='" + content + "',node_type_id=" + type + ",top='" + top + "',comment_enabled='" + comment_enabled + "',comment_display='" + comment_display + "',status='" + status + "' where id=" + id);
        page->redirect("./admin_node");
        return;
    }

    page->response("form_title", "Create Content");
    page->response("node_id", "");
    page->response("title", "");
    page->response("snapshot", "");
    page->response("content", "");
    page->response("top", "");
    page->response("comment_enabled", "");
    page->response("comment_display", "");
    page->response("status", "");

    String update_type_id;
    String action = page->form()->get("action");
    if(action == "update")
    {
        String id = page->form()->get("id").encode();
        if(!id.empty())
        {
            if(db->query("select title,snapshot,content,node_type_id,top,comment_enabled,comment_display,status from node where id=" + id) && db->next())
            {
                update_type_id = db->getString(3);
                page->response("form_title", "Edit Content");
                page->response("node_id", id);
                page->response("title", db->getString(0).decode());
                page->response("snapshot", db->getString(1));
                page->response("content", db->getString(2));
                if(db->getString(4) == "Y")
                    page->response("top", "checked=\"checked\"");
                if(db->getString(5) == "Y")
                    page->response("comment_enabled", "checked=\"checked\"");
                if(db->getString(6) == "Y")
                    page->response("comment_display", "checked=\"checked\"");
                if(db->getString(7) == "Y")
                    page->response("status", "checked=\"checked\"");
            }
        }
    }

    String type_option;
    db->query("select id,name from node_type where status='Y'");
    while(db->next())
    {
        String type_id = db->getString(0);
        type_option += "<option value=\"" + type_id + "\"" + (update_type_id !=  type_id ? "" : " selected=\"selected\"") + ">" + db->getString(1) + "</option>";
    }
    page->response("type_option", type_option);
    page->response("html_head", String(TINY_MCE) + "<script type='text/javascript'>$(function(){$('#menu_node').addClass('active');});</script>");
    page->render("admin/node_modify.tmpl");
}
