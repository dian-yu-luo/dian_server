#include "../sql_connection_pool.h"
#include <map>
#include <string>
using std::map;
using std::string;
/* 测试不带raii的时候如何进行连接 */
void base_test_connection_pool()
{

    /* 如果不带raii,就是从 */
    int m_close_log = 0;
    map<string, string> users;

    connection_pool *connPool;
    MYSQL *mysql = NULL;
    connPool = connection_pool::GetInstance();
    connPool->init("101.43.238.71", "root", "liangn", "yourdb", 3306, 8, 1);
    mysql = connPool->GetConnection();

    /* ------------------------------------------------------------------------------------------------ */

    if (mysql_query(mysql, "SELECT username,passwd FROM user"))
    {
        LOG_ERROR("SELECT error:%s\n", mysql_error(mysql));
    }

    // 从表中检索完整的结果集
    MYSQL_RES *result = mysql_store_result(mysql);

    // 返回结果集中的列数
    int num_fields = mysql_num_fields(result);

    // 返回所有字段结构的数组
    MYSQL_FIELD *fields = mysql_fetch_fields(result);

    // 从结果集中获取下一行，将对应的用户名和密码，存入map中
    while (MYSQL_ROW row = mysql_fetch_row(result))
    {
        string temp1(row[0]);
        string temp2(row[1]);
        users[temp1] = temp2;
        cout << temp1 << " " << temp2 << "\n";
    }
    /* ------------------------------------------------------------------------------------------------ */

    connPool->ReleaseConnection(mysql);
    connPool->DestroyPool();
}

void base_test_connection_pool_with_raii()
{
    MYSQL *mysql = NULL;
    connection_pool *connPool;
    connPool = connection_pool::GetInstance();
    connPool->init("101.43.238.71", "root", "liangn", "yourdb", 3306, 8, 1);
    connectionRAII mysqlcon(&mysql, connPool);
    /* 他的好处就是析构的时候不用手动的管理了,重要的点就是,我也不知道什么时候会有意外的退出
    正常的情况下都不大可能在最后一行return,这个时候我就可以直接不用每次都想着release了
    什么把sql 实例还给连接池就不用我管了
     */
}
int main(int argc, char const *argv[])
{
    base_test_connection_pool();
}