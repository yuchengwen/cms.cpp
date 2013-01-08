#include <cstdlib>
#include "cpppage.h"
#include "action.h"
#include "database.h"

void node(Page * page)
{
    String id = page->form()->get("node_id").encode();
    int type = 0;
    if(db->query("select node.title,node.content,node.create_time,node.comment_enabled,node.comment_display,node_type.type from node,node_type where node.status='Y' and node.node_type_id=node_type.id and node.id=" + id) && db->next())
    {
        String title = db->getString(0);
        type = db->getInt(5);
        page->response("html_title", title);
        page->response("node_title", title);
        page->response("node_content", db->getString(1));
        page->response("node_create_time", db->getString(2));
    }
    else
    {
        page->notFound();
        return;
    }

    page->response("side_type_list", sideProductType());
    page->response("side_art_list", sideLastAricle(10));
    page->response("side_product_list", sideLastProduct(10));

    if(type == TypePage)
        page->render("node_page.tmpl");
    else
        page->render("node.tmpl");
}
