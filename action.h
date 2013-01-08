#ifndef _CMS_ACTION_H_
#define _CMS_ACTION_H_

class Page;
class Database;

// 节点类型
enum
{
    // 文章
    TypeArticle = 1,
    // 页面
    TypePage,
    // 产品
    TypeProduct,
    TypeEnd
};

// 全局数据库对象
extern Database * db;

// 页面
void index(Page * page);
void productList(Page * page);
void articleList(Page * page);
void node(Page * page);
void adminLogin(Page * page);
void adminIndex(Page * page);
void adminUser(Page * page);
void adminType(Page * page);
void adminNode(Page * page);
void adminNodeModify(Page * page);
void adminComment(Page * page);
void adminAttachment(Page * page);
void adminAttachmentDialog(Page * page);
void adminSetting(Page * page);
void adminPassword(Page * page);
void adminEditTmpl(Page * page);
void adminRunSql(Page * page);
void adminFile(Page * page);

String timeToString(time_t t);

// 产品分类边栏
String sideProductType(int type_id = -1);
// 最新文章边栏
String sideLastAricle(int count);
// 最新产品边栏
String sideLastProduct(int count);

#endif
