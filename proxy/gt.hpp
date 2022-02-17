#pragma once
#include <string>
#include "packet.h"

namespace gt {
    extern std::string version;
    extern std::string flag;
    extern std::string packetsend;
    extern std::string macaddr;
    extern std::string pckt;
    extern bool resolving_uid2;
    extern bool antighost;
    extern bool tp_click;

    extern bool connecting;
    extern bool aapbypass;
    extern bool resolving_uid;
    extern bool resolving_uid2;
    extern bool in_game;
    extern bool ghost;
    extern bool devmode;
    extern bool noclip;
    extern bool placetp;
    extern bool debug;
    extern int placex;
    extern int placey;
    void resolve_uid_to_name(std::string text);
    void send_log(std::string text);
    void solve_captcha(std::string text);
    void updateVersion(std::string text);
}
