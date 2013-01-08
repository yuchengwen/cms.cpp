#include "cpppage.h"
#include "action.h"

void adminComment(Page * page)
{
    page->response("html_head", "<script type='text/javascript'>$(function(){$('#menu_comment').addClass('active');});</script>");
    page->render("admin/comment.tmpl");
}
