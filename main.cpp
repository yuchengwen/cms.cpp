#include <map>
#include <cstdlib>
#include "cppstr.h"
#include "cpppage.h"
#include "action.h"
#include "database_mysql.h"

Database * db = 0;

int main()
{
    // 路由表, 使一个程序能逻辑上实现多个页面
    std::map<String, void(*)(Page*)> handle;
    handle["index"] = index;
    handle["product_list"] = productList;
    handle["article_list"] = articleList;
    handle["node"] = node;
    handle["admin_login"] = adminLogin;
    handle["admin_index"] = adminIndex;
    handle["admin_user"] = adminUser;
    handle["admin_type"] = adminType;
    handle["admin_node"] = adminNode;
    handle["admin_node_modify"] = adminNodeModify;
    handle["admin_comment"] = adminComment;
    handle["admin_attachment"] = adminAttachment;
    handle["admin_attachment_dialog"] = adminAttachmentDialog;
    handle["admin_setting"] = adminSetting;
    handle["admin_password"] = adminPassword;
    handle["admin_edit_tmpl"] = adminEditTmpl;
    handle["admin_run_sql"] = adminRunSql;
    handle["admin_file"] = adminFile;

    // 获取数据库连接参数
    String db_host(getenv("DATABASE_HOST"));
    String db_name(getenv("DATABASE_NAME"));
    String db_user(getenv("DATABASE_USER"));
    String db_password(getenv("DATABASE_PASSWORD"));
    int db_port = atoi(getenv("DATABASE_PORT"));

    // 打开数据库
    db = new Mysql;
    db->open(db_host, db_user, db_password, db_name, db_port);
    // MySQL需要执行下面语句避免数据库中文乱码
    db->exec("set names 'utf8'");

    // 设置模版加载目录
    Page::setTmplDir("./tmpl/");
    // 临时文件存放目录
    Page::setTempDir("./cache/");
    // 文件上传目录
    Page::setUploadDir("/uploads/");

    // 初始化页面对象
    Page page;
    // 页面中可以使用变量html_head在HTML头中插入额外的CSS和JS
    page.response("html_head", "");

    String path = page.pathInfo(0);
    if(!path.startsWith("admin_"))
    {
        db->query("select name,value from settings where name in ('web_title','web_logo','web_keywords','web_description')");
        while(db->next())
        {
            if(db->getString(0) == "web_title")
                page.response("web_title", db->getString(1));
            if(db->getString(0) == "web_logo")
                page.response("web_logo", db->getString(1));
            if(db->getString(0) == "web_keywords")
                page.response("web_keywords", db->getString(1));
            if(db->getString(0) == "web_description")
                page.response("web_description", db->getString(1));
        }
    }

    if(path.empty())
        index(&page);
    else if(handle.find(path) != handle.end())
        handle[path](&page);
    else
        page.notFound();

    // 关闭数据库
    db->close();
    delete db;

    return 0;
}
