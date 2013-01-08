#include <ctime>
#include "cppstr.h"
#include "database.h"
#include "action.h"

String timeToString(time_t t)
{
    struct tm * timeinfo = localtime(&t);
    char buffer[32] = {0};
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;
}

String sideProductType(int type_id)
{
    String side_type_list(1024);
    db->query(String("select id,name from node_type where status='Y' and type=") + String::number(TypeProduct));
    while(db->next())
        side_type_list += String("<li") + String(db->getInt(0) == type_id ? " class=\"active\"" : "") + "><a href=\"./product_list?type_id=" + db->getString(0) + "\">" + db->getString(1) + "</a></li>";
    return side_type_list;
}

String sideLastAricle(int count)
{
    String side_art_list(1024);
    db->query(String("select node.id,node.title from node,node_type where node.status='Y' and node.node_type_id=node_type.id and node_type.type=") + String::number(TypeArticle) + " order by node.top,node.create_time desc limit 0," + String::number(count));
    while(db->next())
        side_art_list += String("<tr><td><a href=\"./node?node_id=") + db->getString(0) + "\"><i class=\"icon-chevron-right\"></i> " + db->getString(1) + "</a></td></tr>";
    return side_art_list;
}

String sideLastProduct(int count)
{
    String side_product_list(1024);
    db->query(String("select node.id,node.title,node.snapshot from node,node_type where node.status='Y' and node.node_type_id=node_type.id and node_type.type=") + String::number(TypeProduct) + " order by node.top,node.create_time desc limit 0," + String::number(count));
    while(db->next())
        side_product_list += String("<tr><td><a href=\"./node?node_id=") + db->getString(0) + "\"><i class=\"icon-chevron-right\"></i> " + db->getString(1) + "</a></td></tr>";
    return side_product_list;
}
