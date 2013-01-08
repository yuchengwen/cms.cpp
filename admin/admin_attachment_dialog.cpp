#include <cstdio>
#include "cpppage.h"
#include "action.h"
#include "database.h"

#define COLUMN_COUNT 4

void adminAttachmentDialog(Page * page)
{
    if(page->session("auth").empty())
    {
        page->redirect("./admin_login");
        return;
    }

    int i = 0;
    String file_list(1024);
    db->query("select url,description from attachment where url like '%.jpg' or url like '%.JPG' or url like '%.png' or url like '%.PNG' or url like '%.gif' or url like '%.GIF' or url like '%.jpeg' or url like '%.JPEG' or url like '%.bmp' or url like '%.BMP' order by create_time desc limit 0,12");
    while(db->next())
    {
        if(i % COLUMN_COUNT == 0)
            file_list += "<ul class=\"thumbnails\">";
        file_list += "<li class=\"span3\"><div style=\"cursor:pointer;\"><img style=\"width:110px;height:100px;\" src=\"" + db->getString(0) + "\" alt=\"" + db->getString(1) + "\" class=\"thumbnail\" onclick=\"$('#snapshot').val($(this).attr('src'));\" /><p style=\"text-align:center\">" + db->getString(1) + "</p></div></li>";
        if(i % COLUMN_COUNT == COLUMN_COUNT - 1)
            file_list += "</ul>";
        i++;
    }
    if(i % COLUMN_COUNT != 0)
        file_list += "</ul>";

    printf("Content-type: text/html; charset=utf-8\r\n\r\n<div class=\"row-fluid\">%s</div>", file_list.data());
}
