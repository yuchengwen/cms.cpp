#include "cpppage.h"
#include "action.h"

void index(Page * page)
{
    page->response("html_title", "Home");
    page->render("index.tmpl");
}
