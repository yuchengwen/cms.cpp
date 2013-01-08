#ifndef _CMS_DATABASE_H_
#define _CMS_DATABASE_H_

#include "cppstr.h"

class Database
{
public:
    Database(void);
    virtual ~Database(void){};

    // 打开数据库
    virtual bool open(const String &host, const String &user, const String &passwd, const String &db, unsigned int port) = 0;
    // 关闭数据库
    virtual void close() = 0;

    // 执行SELECT查询
    virtual bool query(const String & sql) = 0;

    // 按行获取SELECT结果
    virtual bool next() = 0;

    // 执行next()后按列获取查询结果
    virtual int getInt(int col) = 0;
    virtual String getString(int col) = 0;
    virtual double getDouble(int col) = 0;

    // 执行非SELECT语句
    virtual bool exec(const String & sql) = 0;
};

#endif
