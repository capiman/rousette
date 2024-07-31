/*
 * Copyright (C) 2016-2021 CESNET, https://photonics.cesnet.cz/
 *
 * Written by Jan Kundrát <jan.kundrat@cesnet.cz>
 *
*/

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <inttypes.h>
//MM #include <spdlog/sinks/systemd_sink.h>
//MM #include <spdlog/sinks/ansicolor_sink.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <spdlog/spdlog.h>
#include <sysrepo-cpp/Session.hpp>
#include "restconf/Server.h"

namespace {
/** @short Is stderr connected to journald? Not thread safe. */
bool is_journald_active()
{
    return false;

#if 0 //MM    
    const auto stream = ::getenv("JOURNAL_STREAM");
    if (!stream) {
        return false;
    }
    uintmax_t dev;
    uintmax_t inode;
    if (::sscanf(stream, "%" SCNuMAX ":%" SCNuMAX, &dev, &inode) != 2) {
        return false;
    }
    struct stat buf;
    if (fstat(STDERR_FILENO, &buf)) {
        return false;
    }
    return static_cast<uintmax_t>(buf.st_dev) == dev && static_cast<uintmax_t>(buf.st_ino) == inode;
#endif // 0 MM
}

/** @short Provide better levels, see https://github.com/gabime/spdlog/pull/1292#discussion_r340777258 */
template<typename Mutex>
//MM class journald_sink : public spdlog::sinks::systemd_sink<Mutex> {
class journald_sink {
public:
    journald_sink()
    {
#if 0 //MM
        this->syslog_levels_ = {/* spdlog::level::trace      */ LOG_DEBUG,
              /* spdlog::level::debug      */ LOG_INFO,
              /* spdlog::level::info       */ LOG_NOTICE,
              /* spdlog::level::warn       */ LOG_WARNING,
              /* spdlog::level::err        */ LOG_ERR,
              /* spdlog::level::critical   */ LOG_CRIT,
              /* spdlog::level::off        */ LOG_ALERT};
#endif // 0 MM        
    }
};
}

int main(int argc [[maybe_unused]], char* argv [[maybe_unused]] [])
{
    if (is_journald_active()) {
        //MM auto sink = std::make_shared<journald_sink<std::mutex>>();
        //MM auto logger = std::make_shared<spdlog::logger>("rousette", sink);
        //MM spdlog::set_default_logger(logger);
    }
    //MM spdlog::set_level(spdlog::level::trace);

    /* We will parse URIs using boost::spirit's alnum/alpha/... matchers which are locale-dependent.
     * Let's use something stable no matter what the system is using
     */
#if 0 //MM -> TODO: This runs into exception on target, for time being, comment it out
    if (!std::setlocale(LC_CTYPE, "C.UTF-8")) {
        throw std::runtime_error("Could not set locale C.UTF-8");
    }
#endif // 0 MM    

    //MM auto conn = sysrepo::Connection{};
    //MM auto server = rousette::restconf::Server{conn, "::1", "10080"};

    printf("I am in main of rousette Pos 1\n");

    signal(SIGTERM, [](int) {});
    signal(SIGINT, [](int) {});
    pause();

    printf("I am in main of rousette Pos 2\n");

    return 0;
}
