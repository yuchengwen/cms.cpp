#include "cppstr.h"
#include "database_mysql.h"
#ifdef CPP_WIN
#include <Windows.h>
#endif
#include <cstdlib>
#include <cstring>
#include "cpplog.h"
#include "mysql.h"

Mysql::Mysql(void)
{
    _conn = mysql_init(0);
    _result = 0;
}

Mysql::~Mysql(void)
{
    close();
}

bool Mysql::open(const String &host, const String &user, const String &passwd, const String &db, unsigned int port)
{
    if(mysql_real_connect(_conn, host.data(), user.data(), passwd.data(), db.data(), port, 0, 0))
        return true;
    else
    {
        CPP_ERROR("Open mysql failed: %s", mysql_error(_conn));
        return false;
    }
}

void Mysql::close()
{
    if(_result)
    {
        mysql_free_result(_result);
        _result = 0;
    }
    if(_conn)
    {
        mysql_close(_conn);
        _conn = 0;
    }
}

bool Mysql::query(const String & sql)
{
    if(_result)
        mysql_free_result(_result);

    if(mysql_query(_conn, sql.data()) != 0)
    {
        CPP_ERROR("Query error: %s", sql.data());
        return false;
    }
    _result = mysql_store_result(_conn);
    if(!_result)
    {
        CPP_ERROR("Query error: %s", sql.data());
        return false;
    }
    return true;
}

bool Mysql::next()
{
    if((_row = mysql_fetch_row(_result)))
        return true;
    else
        return false;
}

bool Mysql::exec(const String & sql)
{
    if(mysql_query(_conn, sql.data()) == 0)
        return true;
    else
    {
        CPP_ERROR("Query error: %s", sql.data());
        return false;
    }
}

int Mysql::getInt(int col)
{
	if(_row[col])
		return atoi(_row[col]);
	else
		return 0;
}

String Mysql::getString(int col)
{
    if(_row[col])
		return _row[col];
	else
		return "";
}

double Mysql::getDouble(int col)
{
    if(_row[col])
		return atof(_row[col]);
	else
		return 0.0;
}
