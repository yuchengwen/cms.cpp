#ifndef _CMS_DATABASE_MYSQL_H_
#define _CMS_DATABASE_MYSQL_H_

#include "database.h"

struct st_mysql;
struct st_mysql_res;

class Mysql : public Database
{
public:
    Mysql(void);
    ~Mysql(void);

    bool open(const String &host, const String &user, const String &passwd, const String &db, unsigned int port);
    void close();
    bool query(const String & sql);
    bool next();
    bool exec(const String & sql);
    int getInt(int col);
    String getString(int col);
    double getDouble(int col);

private:
    struct st_mysql * _conn;
    struct st_mysql_res * _result;
    char ** _row;
};

#endif
