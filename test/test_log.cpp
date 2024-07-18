#include "../log.h"
int main(int argc, char const *argv[])
{
    int m_close_log = 0;
    Log::get_instance()->init("./ServerLog", 1, 2000, 800000, 800);
    LOG_INFO("梁 ");
    LOG_DEBUG("liaaaa");
    return 0;
}
