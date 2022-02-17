#include "events.h"
#include <iostream>
#include "gt.hpp"
#include "proton/hash.hpp"
#include "proton/rtparam.hpp"
#include "proton/variant.hpp"
#include "server.h"
#include "utils.h"
#include "print.h"
#include <fstream>
#include <chrono>
#include <thread>
#include <future>
#include "dialog.h"
#include "math.h"
#include "discord_webhook.h" //not working

using namespace events::out;
using namespace std;

bool autobanfire = false;
bool autorespull = false;
bool akick = false;
bool apull = false;
bool fakeblink = false;
bool aban = false;
bool automsg = false;
bool somurucu = false;
bool fastvend = false;
bool passforce = false;
bool vault = false;
int xP, yP;
int eff_flags1 = 0;
int eff_flags2 = 0;
int pwd = 0;
bool dropwitem = false;
//random drop
string randomdrop = "0";


//Player1
string setpos1 = "0"; //x |
string setpos2 = "0"; //y |
//Player2
string setpos3 = "0"; //x2 |
string setpos4 = "0"; //y2 |
//MyPos
int mypos1 = 0; //x
int mypos2 = 0; //y
//Display1
int dbox1 = 0; //x1
int dbox2 = 0; //y1
//Display2
int dbox3 = 0; //x2
int dbox4 = 0; //y2

string msgspam = "0";
string expectcount = "0";
void force()
{
    Sleep(200);
    fastdrop = false;
}

void TpToPos(int x, int y)
{
    vector2_t pos;
    pos.m_x = x * 32;
    pos.m_y = y * 32;

    variantlist_t varlist{ "OnSetPos" };
    varlist[1] = pos;
    g_server->m_world.local.pos = pos;
    g_server->send(true, varlist, g_server->m_world.local.netid, -1);
}


void DropItem(int itemid, int count)
{
    // cant give. Make it yourself
}

int intLength(int i) {
    int l = 0;
    for (; i; i /= 10) l++;
    return l == 0 ? 1 : l;
}

void logsaccount(string text) {
    string filename("acc.txt");
    fstream file;
    file.open(filename, std::ios_base::app | std::ios_base::in);
    if (file.is_open())
        file << text << endl;
}

std::string random_string(size_t length) {
    auto randchar = []() -> char {
        const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

void force2()
{
    // make it yourself
}

void trashitem(int itemid, int count)
{
    //make it yourself
}






void Blink()
{
   // ?
}

void force3() {
// ?
}

bool events::out::variantlist(gameupdatepacket_t* packet) {
    variantlist_t varlist{};
    varlist.serialize_from_mem(utils::get_extended(packet));
    PRINTS("varlist: %s\n", varlist.print().c_str());
    return false;
}
bool events::out::pingreply(gameupdatepacket_t* packet) {
    //since this is a pointer we do not need to copy memory manually again
    packet->m_vec2_x = 1000.f;  //gravity
    packet->m_vec2_y = 250.f;   //move speed
    packet->m_vec_x = 64.f;     //punch range
    packet->m_vec_y = 64.f;     //build range
    packet->m_jump_amount = 2;  //for example unlim jumps set it to high which causes ban
    packet->m_player_flags = 0; //effect flags. good to have as 0 if using mod noclip, or etc.
    return false;
}
bool find_command(std::string chat, std::string name) {
    bool found = chat.find("/" + name) == 0;
    if (found)
        gt::send_log("`6" + chat);
    return found;
}
bool events::out::worldoptions(std::string option)
{
    std::string username = "all";
    for (auto& player : g_server->m_world.players) {
        auto name_2 = player.name.substr(2); //remove color
        std::transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
        if (name_2.find(username)) {
            auto& bruh = g_server->m_world.local;
            if (option == "pull")
            {
                string plyr = player.name.substr(2).substr(0, player.name.length() - 4);
                if (plyr != bruh.name.substr(2).substr(0, player.name.length() - 4))
                {
                    g_server->send(false, "action|input\n|text|/pull " + plyr);
                }
            }
            if (option == "kick")
            {
                string plyr = player.name.substr(2).substr(0, player.name.length() - 4);
                if (plyr != bruh.name.substr(2).substr(0, player.name.length() - 4))
                {
                    g_server->send(false, "action|input\n|text|/kick " + plyr);

                }
            }
            if (option == "ban")
            {
                string plyr = player.name.substr(2).substr(0, player.name.length() - 4);
                if (plyr != bruh.name.substr(2).substr(0, player.name.length() - 4))
                {
                    g_server->send(false, "action|input\n|text|/ban " + plyr);

                }
            }
        }
    }

    return true;
}
bool events::out::generictext(std::string packet) {

    PRINTS("Generic text: %s\n", packet.c_str());
    auto& world = g_server->m_world;
    rtvar var = rtvar::parse(packet);
    if (!var.valid())
        return false;
    if (packet.find("roulette2|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("te2|") + 4, packet.size());
            std::string number = aaa.c_str();
            autopull = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("fastdrop|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("rop|") + 4, packet.size());
            std::string number = aaa.c_str();
            fastdrop = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    //autohosting | Sahyui # 1337
    if (packet.find("buttonClicked|setpos1") != -1)
    {
    }
    if (packet.find("buttonClicked|boxsetpos1") != -1)
    {
    }


    if (packet.find("buttonClicked|boxsetpos2") != -1)
    {
    }
    if (packet.find("buttonClicked|setpos2") != -1)
    {
    }


    if (packet.find("fasttrash|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("ash|") + 4, packet.size());
            std::string number = aaa.c_str();
            fasttrash = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");
        }
    }
    if (packet.find("visualspin|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("pin|") + 4, packet.size());
            std::string number = aaa.c_str();
            visualspin = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");
        }
    }
    if (packet.find("fastmode|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("ode|") + 4, packet.size());
            std::string number = aaa.c_str();
            fastmmode = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");
        }
    }

    if (packet.find("autotax|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("tax|") + 4, packet.size());
            std::string number = aaa.c_str();
            taxsystem = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("wltroll1|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("ll1|") + 4, packet.size());
            std::string number = aaa.c_str();
            wltroll = stoi(number);
            if (wltroll == true)
            {
                gt::send_log("`9worldlock`` trolling mode is now `2on");
            }
            else
            {
                gt::send_log("`9worldlock`` trolling mode is now `4off");
            }
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }

    if (packet.find("taxamount|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("unt|") + 4, packet.size());
            std::string number = aaa.c_str();
            yuzde = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("roulette5") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("te5|") + 4, packet.size());
            std::string number = aaa.c_str();
            while (!number.empty() && isspace(number[number.size() - 1]))
                number.erase(number.end() - (76 - 0x4B));
            ruletsayi = std::stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("dicespeed|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("eed|") + 4, packet.size());
            std::string number = aaa.c_str();
            events::out::dicespeed = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");
        }
    }
    if (packet.find("worldbanmod|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("mod|") + 4, packet.size());
            std::string number = aaa.c_str();
            worldbanjoinmod = stoi(number);
            if (worldbanjoinmod == true)
            {
                gt::send_log("`9Successfuly started checking entered for `#@Moderators `9and `#@Guardians`9, please wait...");
            }
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("wrenchpull|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("ull|") + 4, packet.size());
            std::string number = aaa.c_str();
            while (!number.empty() && isspace(number[number.size() - 1]))
                number.erase(number.end() - (76 - 0x4B));
            wrenchpull = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("autohosts|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("sts|") + 4, packet.size());
            std::string number = aaa.c_str();
            events::out::autohosts = stoi(number);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("countryzz|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("yzz|") + 4, packet.size());
            while (!aaa.empty() && isspace(aaa[aaa.size() - 1]))
                aaa.erase(aaa.end() - (76 - 0x4B));
            variantlist_t varlist{ "OnTextOverlay" };

            if (packet.find("buttonClicked|countrylist") != -1)
            {
                std::string paket;
                paket =
                    "\nadd_label_with_icon|big|Country List|left|3394|"
                    "\nadd_spacer|small"
                    "\nadd_textbox|`otr: Turkey|left|2480|"
                    "\nadd_textbox|`oen: USA|left|2480|"
                    "\nadd_textbox|`okr: Korean|left|2480|"
                    "\nadd_textbox|`oid: Indonesia|left|2480|"
                    "\nadd_textbox|`oaf: Afghanistan|left|2480|"
                    "\nadd_textbox|`oal: Albania|left|2480|"
                    "\nadd_textbox|`odz: Algeria|left|2480|"
                    "\nadd_textbox|`oas: American Samoa|left|2480|"
                    "\nadd_textbox|`oad: Andorra|left|2480|"
                    "\nadd_textbox|`oao: Angola|left|2480|"
                    "\nadd_textbox|`oai: Anguilla|left|2480|"
                    "\nadd_textbox|`oaq: Antarctica|left|2480|"
                    "\nadd_textbox|`oag: Antigua and Barbuda|left|2480|"
                    "\nadd_textbox|`oar: Argentina|left|2480|"
                    "\nadd_textbox|`oam: Armenia|left|2480|"
                    "\nadd_textbox|`oth: Thailand|left|2480|"
                    "\nadd_textbox|`oes: Spain|left|2480|"
                    "\nadd_textbox|`oso: Somalia|left|2480|"
                    "\nadd_textbox|`ose: Sweden|left|2480|"
                    "\nadd_quick_exit|"
                    "\nend_dialog|end|Cancel|Okay|";
                variantlist_t liste{ "OnDialogRequest" };
                liste[1] = paket;
                g_server->send(true, liste);
            }
            else
            {
                country = aaa.c_str();

                varlist[1] = "Your country Successfuly changed!, Disconnecting";
                g_server->send(true, varlist);
                gt::flag = country;
                gt::send_log("Disconnected?! Will attempt to reconnect..");//lolbruh
                g_server->disconnectsr(true);
            }
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }

    if (packet.find("buttonClicked|bypass_safevault") != -1) {
        int vaultx = std::stoi(packet.substr(packet.find("tilex|") + 6, packet.length() - packet.find("tilex|") - 1));
        int vaulty = std::stoi(packet.substr(packet.find("tiley|") + 6, packet.length() - packet.find("tiley|") - 1));

        posx = vaultx;
        posy = vaulty;
        Dialog bpsv;

        bpsv.addSmallText("X : " + std::to_string(vaultx));
        bpsv.addSmallText("Y : " + std::to_string(vaulty));
        bpsv.endDialog("options_menu_v", "Apply", "Cancel");
        gt::send_log("`9Pos X, Pos Y applied!. Please open vault mode. /vault");
        return false;
    }

    if (packet.find("buttonClicked|optionzzz") != -1)
    {
        try
        {
            Dialog a;
            a.addLabelWithIcon("Options Page", 758, LABEL_BIG);
            a.addSpacer(SPACER_SMALL);
            a.addCheckbox("roulette2", "Enable Auto Pull", autopull);
            a.addCheckbox("wrenchpull", "Enable Wrench " + mode, wrenchpull);
            a.addCheckbox("fastdrop", "Enable Fast Drop", fastdrop);
            a.addCheckbox("fasttrash", "Enable Fast Trash", fasttrash);
            a.addCheckbox("dicespeed", "Enable Dice Speed", events::out::dicespeed);
            a.addCheckbox("worldbanmod", "Exit world when mod joins", worldbanjoinmod);
            a.addCheckbox("visualspin", "Enable Visual Spin", visualspin);
            a.addCheckbox("autohosts", "Enable Show X,Y Position", autohosts);
            a.addCheckbox("wltroll1", "Enable World Lock Troll", wltroll);
            a.addInputBox("roulette5", "Number:", to_string(ruletsayi), 2);
            a.addInputBox("saveworld", "Save World:", saveworld, 12);
            a.endDialog("end", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }
    if (packet.find("xvault111|") != -1) {
        std::string aaa = packet.substr(packet.find("111|") + 4, packet.size());
        try
        {
            posx = std::stoi(aaa.c_str());
            gt::send_log("posx: " + std::to_string(posx));
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `9Please enter numbers only.");

        }
    }


    if (packet.find("2vault222|") != -1) {
        std::string aaa = packet.substr(packet.find("222|") + 4, packet.size());
        while (!aaa.empty() && isspace(aaa[aaa.size() - 1]))
            aaa.erase(aaa.end() - (76 - 0x4B));
        try
        {
            posy = std::stoi(aaa.c_str());
            gt::send_log("posy: " + std::to_string(posy));
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `9Please enter numbers only.");
        }
    }
    if (packet.find("saveworld|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("rld|") + 4, packet.size());
            while (!aaa.empty() && isspace(aaa[aaa.size() - 1]))
                aaa.erase(aaa.end() - (76 - 0x4B));
            events::out::saveworld = aaa;
        }
        catch (exception a)
        {

        }
    }

    if (packet.find("buttonClicked|iso11pul1") != -1) {
        mode = "`5Pull";//bannj

    }
    if (packet.find("buttonClicked|bannj") != -1) {

        mode = "`4Ban";

    }
    if (packet.find("buttonClicked|kicj") != -1) {

        mode = "`4Kick";

    }
    if (packet.find("iditemzz1|") != -1) {
        std::string aaa = packet.substr(packet.find("zz1|") + 4, packet.size());
        while (!aaa.empty() && isspace(aaa[aaa.size() - 1]))
            aaa.erase(aaa.end() - (76 - 0x4B));
        std::string number = aaa.c_str();
        try
        {
            iditemm = std::stoi(aaa.c_str());

        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `9Please enter numbers only.");

        }
    }
    if (packet.find("buttonClicked|killall") != -1)
    {
        worldoptions("kick");

    }
    if (packet.find("buttonClicked|banall") != -1)
    {

        worldoptions("ban");

    }
    if (packet.find("buttonClicked|pullall") != -1)
    {
        worldoptions("pull");
    }
    if (packet.find("buttonClicked|ubaworld") != -1)
    {
        g_server->send(false, "action|input\n|text|/uba");

    }
    if (wrenchpull == true) {
        if (packet.find("action|wrench") != -1) {
            g_server->send(false, packet);

            std::string str = packet.substr(packet.find("netid|") + 6, packet.length() - packet.find("netid|") - 1);
            std::string gta5 = str.substr(0, str.find("|"));
            if (mode == "`5Pull")
            {
                g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + gta5 + "|\nnetID|" + gta5 + "|\nbuttonClicked|pull");

            }
            if (mode == "`4Kick")
            {
                g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + gta5 + "|\nnetID|" + gta5 + "|\nbuttonClicked|kick");

            }
            if (mode == "`4Ban")
            {
                g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + gta5 + "|\nnetID|" + gta5 + "|\nbuttonClicked|worldban");

            }
            variantlist_t varlist{ "OnTextOverlay" };
            varlist[1] = "Successfuly " + mode + "`` netID: " + gta5;
            g_server->send(true, varlist);
            return true;
        }
    }
    if (var.get(0).m_key == "action" && var.get(0).m_value == "input") {
        if (var.size() < 2)
            return false;
        if (var.get(1).m_values.size() < 2)
            return false;

        if (!world.connected)
            return false;

        auto chat = var.get(1).m_values[1];

        if (find_command(chat, "name ")) { //ghetto solution, but too lazy to make a framework for commands.
            std::string name = "``" + chat.substr(6) + "``";
            variantlist_t va{ "OnNameChanged" };
            va[1] = name;
            g_server->send(true, va, world.local.netid, -1);
            gt::send_log("name set to: " + name);
            return true;
        }
        //yarrakcode
        else if (find_command(chat, "ppos1"))
        {
            auto& bruh = g_server->m_world.local;

            int playerx = bruh.pos.m_x / 32;
            int playery = bruh.pos.m_y / 32;
            
            int setpos1 = playerx;
            int setpos2 = playery;

            gt::send_log("Positions applied");

            return true;
        }
        else if (find_command(chat, "ppos2")) {

            auto& bruh = g_server->m_world.local;

            int playerx = bruh.pos.m_x / 32;
            int playery = bruh.pos.m_y / 32;

            int setpos3 = playerx;
            int setpos4 = playery;

            gt::send_log("Positions applied");

            return true;
        }
        else if (find_command(chat, "dpos1")) {

            auto& bruh = g_server->m_world.local;
           
            int playerx = bruh.pos.m_x / 32;
            int playery = bruh.pos.m_y / 32;

            int dbox1 = playerx;
            int dbox2 = playery;

            gt::send_log("Positions applied");

            return true;
        }
        else if (find_command(chat, "dpos2")) {

            auto& bruh = g_server->m_world.local;

            int playerx = bruh.pos.m_x / 32;
            int playery = bruh.pos.m_y / 32;

            dbox3 = playerx;
            dbox4 = playery;

           

            gt::send_log("Positions applied");

            return true;
        }
        else if (find_command(chat, "autohost")) {
            std::string replaced = chat.substr(4);
            int number1 = std::stoi(replaced);
            int number2 = std::stoi(replaced.substr(intLength(number1)));

            gt::send_log("Teleported");
            return true;

        }
        else if (find_command(chat, "vault")) {
            vault = !vault;
            if (vault)
                gt::send_log("`9Bypass Safe : `2Enabled");
            else
                gt::send_log("`9Bypass Safe : `4Disabled");
            return true;

        }

        else if (find_command(chat, "tp "))
        {
            std::string replaced = chat.substr(4);
            int number1 = std::stoi(replaced);
            int number2 = std::stoi(replaced.substr(intLength(number1)));
            gt::send_log("Teleporting to X: " + std::to_string(number1) + " Y: " + std::to_string(number2));
            TpToPos(number1, number2);
            return true;
        }



        else if (find_command(chat, "myflag ")) {
            std::string world123 = chat.substr(8);

            std::string mn = world123 + "|showGuild|";
            gt::send_log("`4[Emin] `w : Your name set to Master");

            variantlist_t mentor{ "OnCountryState" };
            mentor[1] = mn.c_str();
            g_server->send(true, mentor, g_server->m_world.local.netid, -1);
            gt::send_log("`#[Visual] `w : Your world setted to -" + world123);
            return true;

        }
        else if (find_command(chat, "master")) {
            std::string mn = "az|showGuild|master";
            gt::send_log("`4[Emin] `w : Your name set to Master");

            variantlist_t mentor{ "OnCountryState" };
            mentor[1] = mn.c_str();
            g_server->send(true, mentor, g_server->m_world.local.netid, -1);
            return true;
        }
        else if (find_command(chat, "fireban")) {
            autobanfire = !autobanfire;
            if (autobanfire)
                gt::send_log("`9 Auto Ban Fire : `2Enabled");
            else
                gt::send_log("`9 Auto Ban Fire : `4Disabled");
            return true;
        }
        else if (find_command(chat, "devmode")) {
            gt::devmode = !gt::devmode;
            if (gt::devmode)
                gt::send_log("`9 Dev Mode : `2 Enabled");
            else
                gt::send_log("`9 Dev Mode : `4 Disabled");
            return true;
        }
        else if (find_command(chat, "find "))
        {
            gt::send_log("soon");
        }
        else if (find_command(chat, "dropall")) {
            for (server::Item& item : server::inventory) {
                g_server->send(false, "action|drop\nitemID|" + std::to_string(item.id));
                g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + std::to_string(item.id) + "\ncount|" + std::to_string(item.count) + "\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
            return true;
        }
        else if (find_command(chat, "trashall")) {
            for (server::Item& item : server::inventory) {
                g_server->send(false, "action|trash\nitemID|" + std::to_string(item.id));
                g_server->send(false, "action|dialog_return\ndialog_name|trash_item\nitemID|" + std::to_string(item.id) + "\ncount|" + std::to_string(item.count) + "\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
            return true;
        }

        else if (find_command(chat, "inv")) {
            for (server::Item& item : server::inventory) {
                gt::send_log("`9Current Your inventory Items `cid: `4" + std::to_string(item.id) + " `cCount: `4" + std::to_string(item.count));
            }
            return true;
        }
        else if (find_command(chat, "rw")) {
            string randomworld = utils::random(utils::random(8, 24));
            g_server->send(false, "action|join_request\nname|" + randomworld, 3);
            return true;
        }

        else if (find_command(chat, "flag ")) {
            int flag = atoi(chat.substr(6).c_str());
            variantlist_t va{ "OnGuildDataChanged" };
            va[1] = 1;
            va[2] = 2;
            va[3] = flag;
            va[4] = 3;
            g_server->send(true, va, world.local.netid, -1);
            gt::send_log("flag set to item id: " + std::to_string(flag));
            return true;
        }
        else if (find_command(chat, "ghost")) {
            gt::ghost = !gt::ghost;
            if (gt::ghost)
                gt::send_log("Ghost is now enabled.");
            else
                gt::send_log("Ghost is now disabled.");
            return true;
        }
        else if (find_command(chat, "casino")) {
            Dialog casino;
            casino.addLabelWithIcon("Auto Tax System", 758, LABEL_BIG);
            casino.addSpacer(SPACER_SMALL);
            if (taxsystem == true)
            {
                casino.addCheckbox("autotax", "Enable Auto Tax", CHECKBOX_SELECTED);
            }
            else
            {
                casino.addCheckbox("autotax", "Enable Auto Tax", CHECKBOX_NOT_SELECTED);

            }
            if (fastmmode == true)
            {
                casino.addCheckbox("fastmode", "Fast Drop Mode", CHECKBOX_SELECTED);
            }
            else
            {
                casino.addCheckbox("fastmode", "Fast Drop Mode", CHECKBOX_NOT_SELECTED);

            }
            casino.addInputBox("taxamount", "Tax Amount: %", std::to_string(yuzde), 2);
            casino.addQuickExit();
            casino.endDialog("end", "OK", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = casino.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        /*else if (find_command(chat, "options")) {
            Dialog a;
            a.addLabelWithIcon("Options Page", 758, LABEL_BIG);
            a.addSpacer(SPACER_SMALL);
            a.addCheckbox("roulette2", "Enable Auto Pull", autopull);
            a.addCheckbox("wrenchpull", "Enable Wrench " + mode, wrenchpull);
            a.addCheckbox("fastdrop", "Enable Fast Drop", fastdrop);
            a.addCheckbox("fasttrash", "Enable Fast Trash", fasttrash);
            a.addCheckbox("dicespeed", "Enable Dice Speed", events::out::dicespeed);
            a.addCheckbox("worldbanmod", "Exit world when mod joins", worldbanjoinmod);
            a.addCheckbox("visualspin", "Enable Visual Spin", visualspin);
            a.addCheckbox("autohosts", "Enable Show X,Y Position", autohosts);
            a.addCheckbox("wltroll1", "Enable World Lock Troll", wltroll);
            a.addInputBox("roulette5", "Number:", to_string(ruletsayi), 2);
            a.addInputBox("saveworld", "Save World:", saveworld, 12);
            a.endDialog("end", "Okey", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);

            return true;
        }*/
        else if (find_command(chat, "wrench")) {
            std::string paket;
            paket =
                "add_label_with_icon|big|Wrench Mode: " + mode + "|left|2480| \nadd_spacer|small\n"
                "\nadd_button|kicj|`4Kick``|noflags|0|0|"
                "\nadd_button|bannj|`4Ban``|noflags|0|0|"
                "\nadd_button|iso11pul1|`5Pull``|noflags|0|0|"
                "\nend_dialog|end|Cancel|Okay|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);
            return true;
        }
        if (autopull == true) {
            if (packet.find("/options") != -1) {
            }
            else {
                if (packet.find("text|") != -1) {
                    std::string str = packet.substr(packet.find("text|") + 5, packet.length() - packet.find("text|") - 1);
                    g_server->send(false, "action|input\n|text|/pull " + str);
                    return true;
                }
            }
            return true;
        }
        else if (find_command(chat, "country")) {
            std::string paket;
            paket =
                "add_label_with_icon|big|Country|left|3394| \nadd_spacer|small\n"
                "\nadd_text_input|countryzz|CountryID|" +
                country +
                "|2|"
                "\nadd_button|countrylist|`wCountry List``|noflags|0|0|"
                "\nadd_quick_exit|"
                "\nend_dialog|end|Cancel|Okay|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);

            return true;
        }
        else if (find_command(chat, "server")) {
            std::string paket;
            paket =
                "add_label_with_icon|big|Server Settings|left|32|\nadd_spacer|small\n"
                "\nadd_text_input|serverip|Server IP|" +
                g_server->m_server +
                "|15|"
                "\nadd_text_input|serverport|Server Port|" +
                std::to_string(g_server->m_port) +
                "|5|"
                "\nend_dialog|end|Cancel|Okay|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);

            return true;
        }
       

        else if (find_command(chat, "setuid ")) {
            string chateyazilan = chat.substr(7);
            if (chateyazilan.find("owner") != -1)
            {
                uidz = stoi(owneruid);
                gt::send_log("`9Target UID Successfuly set to owner uid `#" + to_string(uidz));
                return true;
            }
            if (chateyazilan.find("current") != -1 || chateyazilan.find("Current") != -1 || chateyazilan.find("CURRENT") != -1)
            {
                uidz = stoi(currentuid);
                gt::send_log("`9Target UID Successfuly set to current uid `#" + to_string(uidz));
            }
            else
            {
                try
                {
                    uidz = stoi(chat.substr(7));
                    gt::send_log("`9Target UID Successfuly set to `#" + to_string(uidz));
                }
                catch (exception a)
                {
                    gt::send_log("`4Critical Error: `9Please enter numbers only.");
                }


            }
            return true;
        }
        else if (find_command(chat, "wlt")) {
            if (wltroll == false)
            {
                wltroll = true;
                gt::send_log("`9worldlock`` trolling mode is now `2on");
            }
            else
            {
                gt::send_log("`9worldlock`` trolling mode is now `4off");
                wltroll = false;
            }
            return true;
        }
        else if (find_command(chat, "dicemode")) {
            if (dicemod == false)
            {
                dicemod = true;
                gt::send_log("`9AntiSpeed`` mode is now `2on");
            }
            else
            {
                gt::send_log("`9AntiSpeed`` mode is now `4off");
                dicemod = false;
            }
            return true;
        }
        else if (find_command(chat, "crash")) {
            if (events::out::type2 > 2)
            {
                if (world.name.find("ACE") != -1)
                {
                    gt::send_log("`9You cant do in this world sorry :( if you have a problem pm " + dc);
                    return true;
                }
                else
                {
                    gt::send_log("`9Please use VPN For doesn't get `#SHADOW `4BAN!");
                    g_server->send(false, "action|input\n|text|`eCrashed by " + dc);
                    //for (auto& player : g_server->m_world.players) {
                    //    gameupdatepacket_t packet{ 0 };
                    //    packet.m_type = PACKET_SET_ICON_STATE;
                    //    packet.m_packet_flags = 8;
                    //    packet.m_int_data = utils::random(0, 2);
                    //    packet.m_vec2_x = utils::random(0, 2);
                    //    packet.m_vec_x = utils::random(0, 2);
                    //    packet.m_netid = player.netid;
                    //    for (int i = 0; i < 999; i++) {
                    //        g_server->send(false, NET_MESSAGE_GAME_PACKET, (uint8_t*)&packet, sizeof(gameupdatepacket_t));
                    //    }
                    //   
                    //}
                }
            }
            else
            {
                gt::send_log("`9You dont have permissions. You need `4Buyer+`` Role.");
                return true;

            }
        }
        else if (find_command(chat, "whitelist")) {
            Dialog whitelist;
            whitelist.addLabelWithIcon("Whitelist", 3394, LABEL_BIG);
            whitelist.addTextBox("Whitelisted Player List");
            whitelist.addQuickExit();
            whitelist.endDialog("end", "Okay", "Close");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = whitelist.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        else if (find_command(chat, "teleport ")) {
            std::string name = chat.substr(10);
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                std::transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
                if (name_2.find(name) == 0) {
                    gt::send_log("Teleporting to " + player.name);
                    variantlist_t varlist{ "OnSetPos" };
                    varlist[1] = player.pos;
                    g_server->m_world.local.pos = player.pos;
                    g_server->send(true, varlist, g_server->m_world.local.netid, -1);
                    break;
                }
            }
            return true;
        }
        else if (find_command(chat, "logs")) {
            Dialog Logs;
            Logs.addLabelWithIcon("Logs Page", 5016, LABEL_BIG);
            Logs.addSpacer(SPACER_SMALL);
            if (gt::ghost == true)
            {
                Logs.addTextBox("Ghost Mode``: `2ON");

            }
            else
            {
                Logs.addTextBox("Ghost Mode``: `4OFF");

            }
            if (autobanfire) {
                Logs.addTextBox("Autobanfire``: `2ON");

            }
            else {
                Logs.addTextBox("Autobanfire``: `2OFF");

            }
            Logs.endDialog("end", "OK", "");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = Logs.finishDialog();
            g_server->send(true, liste);
            return true;
        }
        else if (find_command(chat, "setx ")) {
            if (events::out::type2 > 2)
            {
                try
                {
                    setx = std::stoi(chat.substr(6));
                    gt::send_log("Set tileX to `5" + std::to_string(setx) + "`` successfuly");
                }
                catch (exception a)
                {
                    gt::send_log("`4Critical Error: `9Please enter numbers only.");
                }
            }
            return true;
        }
        else if (find_command(chat, "sety ")) {
            if (events::out::type2 > 2)
            {
                try
                {
                    sety = std::stoi(chat.substr(6));
                    gt::send_log("Set tileY to `5" + std::to_string(sety) + "`` successfuly");
                }
                catch (exception a)
                {
                    gt::send_log("`4Critical Error: `9Please enter numbers only.");

                }
            }

            return true;
        }
        else if (find_command(chat, "setid ")) {
            if (events::out::type2 > 2)
            {
                try
                {
                    id = std::stoi(chat.substr(6));
                    gt::send_log("Set DoorID to `5" + std::to_string(id) + "`` successfuly");
                }
                catch (exception a)
                {
                    gt::send_log("`4Critical Error: `9Please enter numbers only.");

                }
            }
            return true;
        }
        else if (find_command(chat, "bypassdoor")) {
            if (events::out::type2 > 2)
            {
                g_server->send(false, "action|dialog_return\ndialog_name|door_edit\ntilex|" + std::to_string(setx) + "|\ntiley|" + std::to_string(sety) + "|\ndoor_name|SrMotion\ndoor_target|SrMotion:123\ndoor_id|" + std::to_string(id) + "\ncheckbox_locked|1");
                gt::send_log("Hacking the door of ID of tileX `5" + std::to_string(setx) + " ``and tileY `5" + std::to_string(sety));
                gt::send_log("Please do /logs to check your `2ID");
            }

            return true;
        }
        else if (find_command(chat, "bypasspath")) {
            if (events::out::type2 > 2)
            {
                g_server->send(false, "action|dialog_return\ndialog_name|sign_edit\ntilex|" + std::to_string(setx) + "|\ntiley|" + std::to_string(sety) + "|\nsign_text|" + std::to_string(id));
                gt::send_log("Hacking the door of ID of tileX `5" + std::to_string(setx) + " ``and tileY `5" + std::to_string(sety));
                gt::send_log("Please do /logs to check your `2ID");
            }
            return true;
        }
        else if (find_command(chat, "visdice"))
        {
            if (events::out::visdice == false)
            {
                events::out::visdice = true;
                gt::send_log("`9Dice Visual Mode `#Enabled");
            }
            else
            {
                events::out::visdice = false;
                gt::send_log("`9Dice Visual Mode `4Disabled");

            }
            return true;
        }
        else if (find_command(chat, "dset "))
        {
            try
            {
                events::out::sayi = std::stoi(chat.substr(5)) - 1;
                gt::send_log("`9Number set to `#" + chat.substr(5));
            }
            catch (std::exception)
            {
                gt::send_log("Critical error: use number");
            }
            return true;
        }
        else if (find_command(chat, "save")) {

            g_server->send(false, "action|join_request\nname|" + saveworld, 3);
            return true;
        }
        else if (find_command(chat, "warp ")) {
            g_server->send(false, "action|join_request\nname|" + chat.substr(5), 3);
            gt::send_log("`9Warping to `#" + chat.substr(5));
            return true;
        }
        else if (find_command(chat, "skin ")) {

            int skin = atoi(chat.substr(5).c_str());
            variantlist_t va{ "OnChangeSkin" };
            va[1] = skin;
            g_server->send(true, va, world.local.netid, -1);
            return true;
        }

        else if (find_command(chat, "uid ")) {
            std::string name = chat.substr(5);
            gt::send_log("resolving uid for " + name);
            g_server->send(false, "action|input\n|text|/ignore /" + name);
            g_server->send(false, "action|friends");
            gt::resolving_uid2 = true;
            return true;
        }
        else if (find_command(chat, "world")) {

            std::string paket;
            paket =
                "\nadd_label_with_icon|big|World Commands|left|32|"
                "\nadd_spacer|small"
                "\nadd_button|killall|`4Kill All``|noflags|0|0|"
                "\nadd_button|banall|`4Ban All``|noflags|0|0|"
                "\nadd_button|pullall|`5Pull All``|noflags|0|0|"
                "\nadd_button|ubaworld|`5Unban World``|noflags|0|0|"
                "\nadd_textbox|`4IMPORTANT: `oDiscord: SrMotion#0069.|left|2480|"
                "\nadd_quick_exit|"
                "\nend_dialog|end|Cancel|Okay|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);
            return true;
        }
        else if (find_command(chat, "rs")) {
            std::vector<uint32_t> Skinlist;
            Skinlist = { 1348237567, 1685231359, 2022356223, 2190853119, 2527912447, 2864971775, 3033464831, 3370516479 };

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            string color = "action|setSkin\ncolor|" + std::to_string(Skinlist.at(utils::random(0, 7)));
            gt::send_log("`9Changed");
            g_server->send(false, color);
            return true;
        }
    /*    else if (find_command(chat, "blind")) {
        force3;
         }*/


        //yarrakprox
       //
        else if (find_command(chat, "myinfo")) { 
        gt::send_log("Current world : " + world.name);
        gt::send_log("Your GrowID : " + name);
        gt::send_log("Your Password : " + pass);
        return true;
}
        else if (find_command(chat, "passforce")) {
            if (passforce == false) {
                passforce = true;
                gt::send_log("`9Passforce  : `2Enabled");
            }
            else {
                passforce = false;
                gt::send_log("`9Passforce : `4Disabled");
                pwd = 0;
            }
            return true;
        }
        else if (find_command(chat, "start "))
    {
    std::string pwd = chat.substr(7);
    gt::send_log("`9Start number setted to : `2" + pwd);
    return true;
 }


        else if (find_command(chat, "suckme")) {
            if (somurucu == false) {
                somurucu = true;
                gt::send_log("`9Sucker Mode : `2Enabled");
            }
            else {
                somurucu = false;
                gt::send_log("`9Sucker Mode  : `4Disabled");
            }
        }



        else if (find_command(chat, "fastvend")) {
            if (fastvend == false) {
                fastvend = true;
                gt::send_log("`9Vend Mode : `2Fast");
            }
            else {
                fastvend = false;
                gt::send_log("`9Vend  Mode  : `4Normal");
            }
        }

        else if (find_command(chat, "banauto")) {
            if (aban == false) {
                aban = true;
                gt::send_log("`9Auto Ban when someone join world : `2Enabled");
            }
            else {
                aban = false;
                gt::send_log("`9Auto Ban when someone join world : `4Disabled");
            }
            return true;
        }
        else if (find_command(chat, "kickauto")) {
            if (aban == false) {
                aban = true;
                gt::send_log("`9Auto kick when someone join world : `2Enabled");
            }
            else {
                aban = false;
                gt::send_log("`9Auto kick when someone join world : `4Disabled");
            }
            return true;
        }
        else if (find_command(chat, "pullauto")) {
            if (aban == false) {
                aban = true;
                gt::send_log("`9Auto pull when someone join world : `2Enabled");
            }
            else {
                aban = false;
                gt::send_log("`9Auto pull when someone join world : `4Disabled");
            }
            return true;
        }

        else if (find_command(chat, "respull")) {
            if (autorespull == false) {
                autorespull = true;
                gt::send_log("`9Auto Respawn When Pulled : `2Enabled");
            }
            else {
                autorespull = false;
                gt::send_log("`9Auto Respawn When Pulled : `4Disabled");
            }
            return true;
        }

        else if (find_command(chat, "legend")) {
            variantlist_t va{ "OnNameChanged" };
            va[1] = "``" + name + " of Legend``";
            g_server->send(true, va, world.local.netid, -1);
            gt::send_log("name set to: " + name + " of Legend");
            return true;
        }
        else if (find_command(chat, "doctor")) {
            std::string pkt;
            pkt = "az|showGuild|doctor";
            gt::send_log("Your name setted Doctor");
            variantlist_t va{ "OnNameChanged" };
            va[1] = "Dr." + name;
            g_server->send(true, va, world.local.netid, -1);
        }
        else if (find_command(chat, "maxlevel")) {
            std::string pkt;
            pkt = "az|showGuild|maxLevel";
            gt::send_log("Your name setted MaxLevel");
            variantlist_t va{ "OnNameChanged" };
            va[1] = "Dr." + name;
            g_server->send(true, va, world.local.netid, -1);
        }
        else if (find_command(chat, "textmsg "))
        {
            msgspam = chat.substr(9);
            gt::send_log("`9MSG Spam Text Set To: `2" + msgspam);
            return true;
        }
        else if (find_command(chat, "count ")) {
            expectcount = chat.substr(7);
            gt::send_log("`9Vend buy count setted to : " + expectcount);
            return true;
        }

        //buywl.dl
        else if (find_command(chat, "buywl")) {
        g_server->send(false, "action|buy\nitem|world_lock", 2);
        return true;
        }
        else if (find_command(chat, "buy10wl")) {
        g_server->send(false, "action|buy\nitem|world_lock_10_pack", 2);
        return true;
        }
        else if (find_command(chat, "buysl")) {
        g_server->send(false, "action|buy\nitem|small_lock", 2);

        return true;
        }
        else if (find_command(chat, "buyhl")) {
        g_server->send(false, "action|buy\nitem|huge_lock", 2);
        return true;
        }
        else if (find_command(chat, "buybl")) {
        g_server->send(false, "action|buy\nitem|big_lock", 2);
        return true;
        }
        //drop
        else if (find_command(chat, "dropwl "))
        {
        std::string clr = chat.substr(8);
        DropItem(242, std::stoi(clr));
        gt::send_log("`9Dropped " + clr + " `8WL");

        return true;
        }
        else if (find_command(chat, "dropdl "))
        {
        std::string clr = chat.substr(8);
        DropItem(1796, std::stoi(clr));
        gt::send_log("`9Dropped " + clr + " `1DL");
        return true;
        }
        else if (find_command(chat, "dropbgl "))
        {
        std::string clr = chat.substr(9);
        DropItem(7188, std::stoi(clr));
        gt::send_log("`9Dropped " + clr + " `1BGL");
        return true;
        }
 
        else if (find_command(chat, "fdrop")) {
        if (dropwitem == false) {
            dropwitem = true;
            gt::send_log("`9Fast drop with count :" + randomdrop + " `2Enabled");
        }
        else {
            dropwitem = false;
            gt::send_log("`9Fast drop with count :" + randomdrop + " `4Disabled");
        }

}
        else if (find_command(chat, "fcount ")) {
        randomdrop = chat.substr(8);
        gt::send_log("Fast drop amount setted to : " + randomdrop);
  return true;
        }

        else if (find_command(chat, "msgstart")) {
            automsg = !automsg;
            if (automsg)
                gt::send_log("`9 Auto MSG : `2Enabled");
            else
                gt::send_log("`9 Auto MSG : `4Disabled");
            return true;
        }
        else if (find_command(chat, "antighost")) {
            gt::antighost = !gt::antighost;
            if (gt::antighost)
                gt::send_log("`9 Anti Ghost : `2 Enabled");
            else
                gt::send_log("`9 Anti Ghost : `4 Disabled");
            return true;
        }
        else if (find_command(chat, "fakelag")) {
            fakeblink = !fakeblink;
            if (fakeblink)
                gt::send_log("`9Fake Lag : `2 Enabled");
            else
                gt::send_log("`9Fake Lag : `4 Disabled");
            return true;
        }
        else if (find_command(chat, "noclip")) {
        gt::noclip = !gt::noclip;
        if (gt::noclip)
            gt::send_log("`9No clip : `2 Enabled");
        else
            gt::send_log("`9No clip : `4 Disabled");
        return true;


}



//
        else if (find_command(chat, "ca ")) {
        std::string account = chat.substr(4);
        std::string password = "Emin2004.";
        std::string emailformat = "@gmail.com";
        std::string ramdomcuy = chat.substr(4);
        std::string randomcuy = account;
        gt::send_log("`2Trying to create account with name: `2" + randomcuy);
        g_server->send(false, "action|growid");
        g_server->send(false, "action|dialog_return\ndialog_name|growid_apply\nlogon|" + randomcuy + "\npassword|" + password + "\npassword_verify|lmfao#1234\nemail|" + randomcuy + emailformat);
        logsaccount("GrowID: " + randomcuy + ",Password: " + password);
        gt::send_log("GrowID: " + randomcuy + ",Password: " + password + "        You can check acc.txt too!");
        return true;
        }
        else if (find_command(chat, "ra")) {

            std::string password = "sahyui1337.";
            std::string emailformat = "@gmail.com";
            std::string randomcuy = random_string(18); //18 character you can modify it to anything < 18
            gt::send_log("`2Trying to create account with name: `2" + randomcuy);
            g_server->send(false, "action|growid");
            g_server->send(false, "action|dialog_return\ndialog_name|growid_apply\nlogon|" + randomcuy + "\npassword|" + password + "\npassword_verify|lmfao#1234\nemail|" + randomcuy + emailformat);
            logsaccount("GrowID: " + randomcuy + ",Password: " + password);
            gt::send_log("GrowID: " + randomcuy + ",Password: " + password + "        You can check acc.txt too!");
            return true;
        }

        else if (find_command(chat, "proxy")) {
            std::string paket;
            paket =
                "\nadd_label_with_icon|big|Proxy Commands|left|32|"
                "\nadd_textbox|`9/fireban `#(Autoban fire starter)|left|2480"
                "\nadd_textbox|`9/dropall `#(Drop all items)|left|2480"
                "\nadd_textbox|`9/trashall `#(Trash items)|left|2480"
                "\nadd_textbox|`9/devmode `#(Dev mode)|left|2480"
                "\nadd_textbox|`9/noclip `#(No clip)|left|2480"
                "\nadd_textbox|`9/ghost `#(Ghost Mode) |left|2480"
                "\nadd_textbox|`9/fakelag `#(Fake Lag) |left|2480"
                "\nadd_textbox|`9/antighost `#(Anti Ghost)|left|2480"
                "\nadd_textbox|`9/teleport (name) `#(Teleport to name) |left|2480"
                "\nadd_textbox|`9/tp x y `#(Teleport to X,Y)|left|2480"
                "\nadd_textbox|`9/rw `#(Random world)|left|2480"
                "\nadd_textbox|`9/ra `#(Random account create)|left|2480" 
                "\nadd_textbox|`9/rs `#(Random skin)|left|2480"
                "\nadd_textbox|`9/respull `#(Auto respawn when pulled)|left|2480"
                "\nadd_textbox|`9/pullauto `#(Autopull when someone join world) |left|2480"
                "\nadd_textbox|`9/kickauto `#(Autokick when someone join world) |left|2480"
                "\nadd_textbox|`9/banauto `#(Autoban when someone join world) |left|2480"
                "\nadd_textbox|`9/textmsg (text) `#(Set spam text to MSG)|left|2480"
                "\nadd_textbox|`9/startmsg `#(Automsg start)|left|2480"
                "\nadd_textbox|`9/legend |left|2480"
                "\nadd_textbox|`9/master |left|2480"
                "\nadd_textbox|`9/doctor |left|2480"
                "\nadd_textbox|`9/maxlevel |left|2480"
                "\nadd_textbox|`9/suckme `#(Sucker mode)|left|2480"
                "\nadd_textbox|`9/wrench `#(Wrench mode)|left|2480"
                "\nadd_textbox|`9/inv `#(Show all inventory with id)|left|2480"
                "\nadd_textbox|`9/flag (id) `#(Change flag to id)|left|2480"
                "\nadd_textbox|`9/myflag (Country) (Turkey - TR))|left|2480"
                "\nadd_textbox|`9/count (count) `#(Set buy count to fast vend)|left|2480"
                "\nadd_textbox|`9/fastvend `#(Buy from vend without dialog)|left|2480"
                "\nadd_textbox|`9/start (number) `#(Set start number for passforce)|left|2480"
                "\nadd_textbox|`9/passforce `#(Start passforce . Need to click door)|left|2480"
                "\nadd_textbox|`9/dropwl (count) `#(Drop `9wl `#gived count)|left|2480"
                "\nadd_textbox|`9/dropdl (count) `#(Drop `1dl `#gived count)|left|2480"
                "\nadd_textbox|`9/dropbgl (count) `#(Drop `1bgl `#gived count)|left|2480"
                "\nadd_textbox|`9/fcount (count) `#(Set count for fast drop)|left|2480"
                "\nadd_textbox|`9/fdrop `#(Open fast drop. Drop amount is count! (/fcount))|left|2480"
                "\nadd_textbox|`9/buywl , buy10wl, buysl, buybl, buyhl |left|2480"
                "\nadd_textbox|`9 |left|2480"
                "\nadd_textbox|`9 |left|2480"

                "\nadd_textbox|`9 |left|2480"
                //       "\nadd_textbox|`9 |left|2480"

                "\nadd_spacer|small"
                "\nadd_quick_exit|"
                "\nend_dialog|end|Cancel|Okay|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);
            return true;
        }
        return false;
    }


    if (packet.find("game_version|") != -1) {
        if (events::out::loggedin == 1)
        {
            rtvar var = rtvar::parse(packet);
            auto mac = utils::generate_mac();
            auto hash_str = mac + "RT";
            auto hash2 = utils::hash((uint8_t*)hash_str.c_str(), hash_str.length());
            var.set("mac", mac);
            var.set("wk", utils::generate_rid());
            var.set("rid", utils::generate_rid());
            var.set("fz", std::to_string(utils::random(INT_MIN, INT_MAX)));
            var.set("zf", std::to_string(utils::random(INT_MIN, INT_MAX)));
            var.set("hash", std::to_string(utils::random(INT_MIN, INT_MAX)));
            var.set("hash2", std::to_string(hash2));
            var.set("meta", utils::random(utils::random(6, 10)) + ".com");
            var.set("game_version", gt::version);
            var.set("country", gt::flag);
            var.set("player_age", "20");
            if (var.find("tankIDName")) {
                //wlbalance
                name = var.find("tankIDName")->m_values[0];
                pass = var.find("tankIDPass")->m_values[0];
                mac123 = mac;
                rid123;
                
            }
            packet = var.serialize();
            gt::in_game = false;
            print::set_color(LightGreen);
            PRINTS("Spoofing login info\n");
            g_server->send(false, packet);
            return true;
        }
    }

    return false;
}

bool events::out::gamemessage(std::string packet) {
    PRINTS("Game message: %s\n", packet.c_str());
    if (packet == "action|quit") {
        g_server->quit();
        return true;
    }

    return false;
}
int sayi = 1;
bool events::out::state(gameupdatepacket_t* packet) {
    if (!g_server->m_world.connected)
        return false;
    auto& bruh = g_server->m_world.local;
    //deneme
    g_server->m_world.local.pos = vector2_t{ packet->m_vec_x, packet->m_vec_y };
 
    int playerx = bruh.pos.m_x / 32;
    int playery = bruh.pos.m_y / 32;

    if (events::out::autohosts == true)
    {
        int playerx = bruh.pos.m_x / 32;
        int playery = bruh.pos.m_y / 32;
        variantlist_t va{ "OnNameChanged" };
        va[1] = bruh.name + " `4[" + std::to_string(playerx) + "," + std::to_string(playery) + "]" + " `4[" + std::to_string(bruh.netid) + "]``" + " `#[" + to_string(bruh.userid) + "]``";
        g_server->send(true, va, bruh.netid, -1);



    }
    g_server->m_world.local.pos = vector2_t{ packet->m_vec_x, packet->m_vec_y };
    PRINTS("local pos: %.0f %.0f\n", packet->m_vec_x, packet->m_vec_y);
    

    if (fakeblink) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        variantlist_t varlist{ "OnSetPos" };
        varlist[1] = g_server->m_world.local.pos;
        g_server->m_world.local.pos = g_server->m_world.local.pos;
        g_server->send(true, varlist, g_server->m_world.local.netid, -1);
        return true;
    }

    eff_flags1 = packet->effect_flags;
    eff_flags2 = packet->flags;

    if (gt::antighost) {//remove mindcontrol mod
        packet->flags &= ~(1 << 11);
        eff_flags2 &= ~(1 << 11);
    }

    if (gt::ghost)
        return true;
    return false;
}
bool events::in::variantlist(gameupdatepacket_t* packet) {
    variantlist_t varlist{};
    auto extended = utils::get_extended(packet);
    extended += 4; //since it casts to data size not data but too lazy to fix this
    varlist.serialize_from_mem(extended);
    PRINTC("varlist: %s\n", varlist.print().c_str());
    auto func = varlist[0].get_string();

    //probably subject to change, so not including in switch statement.
    if (func.find("OnSuperMainStartAcceptLogon") != -1)
        gt::in_game = true;

    switch (hs::hash32(func.c_str())) {
        //solve captcha
    case fnv32("onShowCaptcha"): {
        auto menu = varlist[1].get_string();
        gt::solve_captcha(menu);
        return true;
    } break;//OnPlayPositioned


    case fnv32("OnTextOverlay"): {
        std::string overlay = varlist.get(1).get_string();
        if (autorespull == true) {
            if (overlay.find("You were pulled by") != -1) {
                g_server->send(true, varlist);
                g_server->send(false, "action|respawn");
            }
            else {
                g_server->send(true, varlist);
            }
            return true;
        }
        return true;
    } break;

    case fnv32("OnRequestWorldSelectMenu"): {
        auto& world = g_server->m_world;
        world.players.clear();
        world.local = {};
        world.connected = false;
        world.name = "EXIT";
    } break;

    case fnv32("OnSendToServer"): g_server->redirect_server(varlist); return true;

    case fnv32("OnConsoleMessage"): {
        auto wry = varlist[1].get_string();
        if (worldbanjoinmod == true)
        {
            if (wry.find("Removed your access from all locks.") != -1)
            {
                gt::send_log("`$Leaving the world due to having Mod bypass on and due to having a `#mod `$in the world!");
                g_server->send(false, "action|join_request\nname|exit", 3);
            }
        }
        if (visualspin == true) {
            if (wry.find("`7[```w" + name + "`` spun the wheel and got") != -1) {
                if (std::to_string(ruletsayi) == "0") {
                    varlist[1] = "`7[```w" + name + "`` spun the wheel and got `2" + std::to_string(ruletsayi) + "``!`7]``";
                }
                else {
                    varlist[1] = "`7[```w" + name + "`` spun the wheel and got `4" + std::to_string(ruletsayi) + "``!`7]``";
                }
                g_server->send(true, varlist, -1, 1900);
                return true;
            }
            else {
                if (wry.find("`7[```2" + name + "`` spun the wheel and got") != -1) {
                    if (std::to_string(ruletsayi) == "0") {
                        varlist[1] = "`7[```2" + name + "`` spun the wheel and got `2" + std::to_string(ruletsayi) + "``!`7]``";
                    }
                    else {
                        varlist[1] = "`7[```2" + name + "`` spun the wheel and got `4" + std::to_string(ruletsayi) + "``!`7]``";
                    }
                    g_server->send(true, varlist, -1, 1900);
                    return true;
                }
                else
                {
                    varlist[1] = packets + varlist[1].get_string();
                    g_server->send(true, varlist);
                    return true;
                }
            }
        }
        else {
            varlist[1] = packets + varlist[1].get_string();
            g_server->send(true, varlist);
            return true;
        }
    } break;
    case fnv32("OnTalkBubble"): {
        auto wry = varlist[2].get_string();
        auto netid = varlist[1].get_int32();
        auto content = varlist[2].get_string();
        auto bbl = varlist[2].get_string();
        if (passforce == true) {
            if (bbl.find("`2The door opens!") != -1) {
                pwd = 0;
                return true;
            }
        }
        else {
            g_server->send(true, varlist);
            return true;
        }
        if (autobanfire == true) {
            if (content.find("`7[```4MWAHAHAHA!! FIRE FIRE FIRE") != -1) {
                g_server->send(false, "action|wrench\n|netid|" + std::to_string(netid));
                g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(netid) + "|\nnetID|" + std::to_string(netid) + "|\nbuttonClicked|worldban");
                return true;
            }
        }
        if (visualspin == true) {
            if (wry.find("`7[```w" + name + "`` spun the wheel and got") != -1) {
                if (std::to_string(ruletsayi) == "0") {
                    varlist[2] = "`7[```w" + name + "`` spun the wheel and got `2" + std::to_string(ruletsayi) + "``!`7]``";
                }
                else {
                    varlist[2] = "`7[```w" + name + "`` spun the wheel and got `4" + std::to_string(ruletsayi) + "``!`7]``";
                }
                g_server->send(true, varlist, -1, 1900);
                return true;
            }
            else
            {
                if (wry.find("`7[```2" + name + "`` spun the wheel and got") != -1) {
                    if (std::to_string(ruletsayi) == "0") {
                        varlist[2] = "`7[```2" + name + "`` spun the wheel and got `2" + std::to_string(ruletsayi) + "``!`7]``";
                    }
                    else {
                        varlist[2] = "`7[```2" + name + "`` spun the wheel and got `4" + std::to_string(ruletsayi) + "``!`7]``";
                    }
                    g_server->send(true, varlist, -1, 1900);
                    return true;
                }
            }
        }
        else {
            g_server->send(true, varlist);
            return true;
        }
    } break;
    case fnv32("OnDialogRequest"): {

        auto content = varlist[1].get_string();

        if (passforce == true) {
            if (content.find("The door requires a password") != -1) {
                int passdx = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                int passdy = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                g_server->send(false, "action|dialog_return\ndialog_name|password_reply\ntilex|" + std::to_string(passdx) + "|\ntiley|" + std::to_string(passdy) + "|\npassword|" + std::to_string(pwd));
                gt::send_log("Guess : `4" + std::to_string(pwd));
                pwd = pwd + 1;
                return true;
            }
        }



        if (gt::resolving_uid2 && (content.find("friend_all|Show offline") != -1 || content.find("Social Portal") != -1) ||
            content.find("Are you sure you wish to stop ignoring") != -1) {
            return true;
        }
        else if (content.find("add_label_with_icon|small|Remove Your Access From World|left|242|") != -1) {
            if (worldbanjoinmod == true)
            {
                g_server->send(false, "action|dialog_return\ndialog_name|unaccess");
                return true;
            }
        }
        else if (gt::resolving_uid2 && content.find("Ok, you will now be able to see chat") != -1) {
            gt::resolving_uid2 = false;
            return true;
        }
        else if (gt::resolving_uid2 && content.find("`4Stop ignoring") != -1) {
            int pos = content.rfind("|`4Stop ignoring");
            auto ignore_substring = content.substr(0, pos);
            auto uid = ignore_substring.substr(ignore_substring.rfind("add_button|") + 11);
            auto uid_int = atoi(uid.c_str());
            if (uid_int == 0) {
                gt::resolving_uid2 = false;
                gt::send_log("name resolving seems to have failed.");
            }
            else {
                gt::send_log("`9Target UID: `#" + uid);
                gt::send_log("`9worldlock troll UID set to: `#" + uid);
                uidz = stoi(uid);
                g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|" + uid);
                g_server->send(false, "action|dialog_return\ndialog_name|friends_remove\nfriendID|" + uid + "|\nbuttonClicked|remove");
            }
            return true;
        }
        if (wltroll == true)
        {
            if (content.find("add_label_with_icon|big|`wEdit World Lock``|left|242|") != -1)
            {
                int x = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                int y = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                g_server->send(false, "action|dialog_return\ndialog_name|lock_edit\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\ncheckbox_" + to_string(uidz) + "|0\ncheckbox_public|0\ncheckbox_disable_music|0\ntempo|100\ncheckbox_disable_music_render|0\ncheckbox_set_as_home_world|0\nminimum_entry_level|1");
                return true;
            }
        }
        if (content.find("add_label_with_icon|big|`wEdit World Lock``|left|") != -1)
        {
            gt::send_log("find world lock x,y path.");
            wlx = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
            wly = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
        }
        if (dicemod == true)
        {
            if (content.find("add_label_with_icon|big|`wEdit Dice Block``|left|456|") != -1)
            {
                int x = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                int y = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                if (content.find("add_checkbox|checkbox_public|Usable by public|0") != -1)
                {
                    gt::send_log("`4 (X:" + to_string(x) + " Y:" + to_string(y) + ")`9 Dice is `2Public");
                    g_server->send(false, "action|dialog_return\ndialog_name|boombox_edit\ntilex|" + to_string(x) + "|\ntiley|" + to_string(y) + "|\ncheckbox_public|1\ncheckbox_silence|1");
                    return true;

                }
                else
                {
                    gt::send_log("`4 (X:" + to_string(x) + " Y:" + to_string(y) + ")`9 Dice is `4Not Public");
                    g_server->send(false, "action|dialog_return\ndialog_name|boombox_edit\ntilex|" + to_string(x) + "|\ntiley|" + to_string(y) + "|\ncheckbox_public|0\ncheckbox_silence|1");
                    return true;

                }
                return true;
            }
        }
        if (content.find("set_default_color|`o") != -1)
        {
            if (content.find("add_label_with_icon|big|`wAre you Human?``|left|206|") != -1)
            {
                gt::solve_captcha(content);
                return true;
            }
        }
        if (content.find("add_label_with_icon|big|`wThe Growtopia Gazette``|left|5016|") != -1)
        {
            gt::send_log("Blocked : Gazette");
            return true;
        }
        if (taxsystem == true)
        {
            if (content.find("Drop") != -1) {
                if (content.find("embed_data|itemID|") != -1) {
                    std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
                    //gt::send_log(itemid);
                    if (itemid.find("242") != -1) {
                        std::string count = content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1);
                        int bruh = stoi(count) % 10;
                        int bruh2 = stoi(count);
                        if (bruh == 1) {
                            bruh2 = bruh2 - 1;
                        }
                        if (bruh == 2) {
                            bruh2 = bruh2 - 2;
                        }
                        if (bruh == 3) {
                            bruh2 = bruh2 - 3;
                        }
                        if (bruh == 4) {
                            bruh2 = bruh2 - 4;
                        }
                        if (bruh == 5) {
                            bruh2 = bruh2 + 5;
                        }
                        if (bruh == 6) {
                            bruh2 = bruh2 + 4;
                        }
                        if (bruh == 7) {
                            bruh2 = bruh2 + 3;
                        }
                        if (bruh == 8) {
                            bruh2 = bruh2 + 2;
                        }
                        if (bruh == 9) {
                            bruh2 = bruh2 + 1;
                        }
                        int bruh3 = bruh2 * yuzde / 100;
                        int bruh4 = stoi(count) - bruh3;
                        if (fastmmode == true)
                        {
                            g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemid + "|\ncount|" + std::to_string(bruh4));
                            return true;

                        }
                        else
                        {
                            varlist[1] = "set_default_color|`o\nadd_label_with_icon|big|`wDrop World Lock``|left|242|\nadd_textbox|How many to drop?|left|\nadd_text_input|count||" + to_string(bruh4) + "|5|\nembed_data|itemID|242\nend_dialog|drop_item|Cancel|OK|";
                            g_server->send(true, varlist);
                            return true;
                        }
                    }
                }
            }

        }
        if (fastdrop == true) {
            std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
            std::string count = content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1);
            if (content.find("embed_data|itemID|") != -1) {
                if (content.find("Drop") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemid + "|\ncount|" + count);
                    return true;
                }
            }
        }
        if (dropwitem == true) {

            std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
            std::string count = content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1);
            if (content.find("embed_data|itemID|") != -1) {
                if (content.find("Drop") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + itemid + "|\ncount|" + randomdrop);
                    return true;
                }
            }


        }

        if (taxsystem == true)
        {
            if (fastmmode == true) {
                std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
                std::string count = content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1);
                if (content.find("embed_data|itemID|") != -1) {
                    if (content.find("Drop") != -1) {
                        if (itemid.find("242") != -1) {
                            return true;
                        }
                    }
                }
            }

        }
        if (fasttrash == true) {
            std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
            std::string count = content.substr(content.find("you have ") + 9, content.length() - content.find("you have ") - 1);
            std::string s = count;
            std::string delimiter = ")";
            std::string token = s.substr(0, s.find(delimiter));
            if (content.find("embed_data|itemID|") != -1) {
                if (content.find("Trash") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|trash_item\nitemID|" + itemid + "|\ncount|" + token);
                    return true;
                }
            }
        }
        if (wrenchpull == true) {
            std::string netidyarragi = content.substr(content.find("embed_data|netID") + 17, content.length() - content.find("embed_data|netID") - 1);
            if (content.find("set_default_color|`o") != -1) { // trash bug fix
                if (content.find("embed_data|netID") != -1) {
                    return true;
                }
            }
        }
        else if (content.find("add_button|report_player|`wReport Player``|noflags|0|0|") != -1) {
            content = content.insert(content.find("add_button|report_player|`wReport Player``|noflags|0|0|"), "\nadd_button|whitelist|`2Add to Whitelist``|noflags|0|0|\nadd_button|blacklist|`4Add to Blacklist``|noflags|0|0|\n");
            varlist[1] = content;
            g_server->send(true, varlist, -1, -1);
            return true;
        }
        else if (content.find("add_button|my_worlds|`$My Worlds``|noflags|0|0|") != -1)
        {
            content = content.insert(content.find("add_button|my_worlds|`$My Worlds``|noflags|0|0|"), "\nadd_button|whitelisted_players|`2Whitelisted Players``|noflags|0|0|\n\nadd_button|blacklisted_players|`4Blacklisted Players``|noflags|0|0|\nadd_button|optionzzz|`5Options``|noflags|0|0|\n");

            varlist[1] = content;
            g_server->send(true, varlist, -1, -1);
            return true;
        }

        if (content.find("Please enter your password") != -1) {
            content = content.insert(content.find("add_button|show_recoveryanswer|`wRecover Password``|noflags|0|0|"), "add_button|show_recoveryanswer|`wRecover Password``|noflags|0|0|\nadd_button|bypass_safevault|`wBypass Safe Vault``|noflags|0|0|");
            varlist[1] = content;
            g_server->send(true, varlist, -1, -1);
            return true;
        }

        if (content.find("Storage Box Xtreme - Level") != -1) {
            if (vault)
            {
                g_server->send(false, "action|dialog_return\ndialog_name|storageboxxtreme\ntilex|" + std::to_string(posx) + "|\ntiley|" + std::to_string(posy) + "|\nitemid|" + std::to_string(iditemm) + "|\nbuttonClicked|do_add\nitemcount|1");
                gt::send_log("`9Bypassed!");
            }
            return true;
        }
        

        if (content.find("add_label_with_icon|big|`wVending Machine``|left|2978") != -1)
        {
            if (fastvend) {
                if (content.find("How many would you like to buy?") != -1) {
                    int x = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                    int y = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                    int itemid = std::stoi(content.substr(content.find("embed_data|expectitem|") + 22, content.length() - content.find("embed_data|expectitem|") - 1));
                    int expectprice = std::stoi(content.substr(content.find("embed_data|expectprice|") + 23, content.length() - content.find("embed_data|expectprice|") - 1));
                    int yourwl = std::stoi(content.substr(content.find("add_textbox|You have") + 20, content.length() - content.find("add_textbox|You have") - 1));
                    if (yourwl - expectprice < 0) {
                    }
                    else {
                        g_server->send(false, "action|dialog_return\ndialog_name|vending\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nverify|1|\nbuycount|" + expectcount + "|\nexpectprice|" + std::to_string(expectprice) + "|\nexpectitem|" + std::to_string(itemid) + "|");
                        return true;
                    }
                }


            }

        }
        if (content.find("add_label_with_icon|big|`wGaia's Beacon``|left|6946|") != -1 || content.find("add_label_with_icon|big|`wUnstable Tesseract``|left|6948|") != -1 || content.find("add_label_with_icon|big|`wMAGPLANT 5000``|left|5638|") != -1)
        {
            std::string modes = content;
            if (somurucu)
            {
                if (content.find("The machine is currently empty!") != -1)
                {

                }
                else
                {
                    xP = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                    yP = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                    if (modes.find("Gaia's Beacon") != -1)
                    {
                        g_server->send(false, "action|dialog_return\ndialog_name|itemsucker_seed\ntilex|" + std::to_string(xP) + "|\ntiley|" + std::to_string(yP) + "|\nbuttonClicked|retrieveitem\n\nchk_enablesucking|1");
                  
                        Sleep(10); //uyusun biraz yavrumuz
                        g_server->send(false, "action|dialog_return\ndialog_name|itemremovedfromsucker\ntilex|" + std::to_string(xP) + "|\ntiley|" + std::to_string(yP) + "|\nitemtoremove|200|");
                    
                    }
                    else
                    {
                        g_server->send(false, "action|dialog_return\ndialog_name|itemsucker_block\ntilex|" + std::to_string(xP) + "|\ntiley|" + std::to_string(yP) + "|\nbuttonClicked|retrieveitem\n\nchk_enablesucking|1");
                        Sleep(10);
                        g_server->send(false, "action|dialog_return\ndialog_name|itemremovedfromsucker\ntilex|" + std::to_string(xP) + "|\ntiley|" + std::to_string(yP) + "|\nitemtoremove|200|");

                    
                    }
                    return true;
                }

            }
        }
    } break;
    case fnv32("OnRemove"): {
        auto text = varlist.get(1).get_string();
        if (text.find("netID|") == 0) {
            auto netid = atoi(text.substr(6).c_str());

            if (netid == g_server->m_world.local.netid)
                g_server->m_world.local = {};

            auto& players = g_server->m_world.players;
            for (size_t i = 0; i < players.size(); i++) {
                auto& player = players[i];
                if (player.netid == netid) {
                    players.erase(std::remove(players.begin(), players.end(), player), players.end());
                    break;
                }
            }
        }
    } break;
    case fnv32("OnSpawn"): {

        std::string meme = varlist.get(1).get_string();
        rtvar var = rtvar::parse(meme);
        auto name = var.find("name");

        auto netid = var.find("netID");
        auto onlineid = var.find("onlineID");
        auto userid = var.find("userID");
        //
        if (apull == true) {
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                for (auto& player : g_server->m_world.players) {
                    auto name_2 = player.name.substr(2); //remove color
                    string outputas = player.name.substr(2).substr(0, player.name.length() - 4);
                    g_server->send(false, "action|input\n|text|/pull " + outputas);
                }
            }
            catch (std::exception) { gt::send_log("Critical Error : Invalid String Position"); }
        }
        if (automsg == true) {
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                for (auto& player : g_server->m_world.players) {
                    auto name_2 = player.name.substr(2);
                    string outputas = player.name.substr(2).substr(0, player.name.length() - 4);
                    g_server->send(false, "action|input\n|text|/msg " + outputas + " " + msgspam);
                }
            }
            catch (std::exception) { gt::send_log("Critical Error : Invalid String Position"); }
        }
        if (aban == true) {
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                for (auto& player : g_server->m_world.players) {
                    auto name_2 = player.name.substr(2); //remove color
                    string outputas = player.name.substr(2).substr(0, player.name.length() - 4);
                    g_server->send(false, "action|input\n|text|/ban " + outputas);
                }
            }
            catch (std::exception) { gt::send_log("Critical Error : Invalid String Position"); }
        }
        if (akick == true) {
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                for (auto& player : g_server->m_world.players) {
                    auto name_2 = player.name.substr(2); //remove color
                    string outputas = player.name.substr(2).substr(0, player.name.length() - 4);
                    g_server->send(false, "action|input\n|text|/kick " + outputas);
                }
            }
            catch (std::exception) { gt::send_log("Critical Error : Invalid String Position"); }
        }


        if (name->m_value.find("`^") != -1)
        {
            std::string unaccess = userid->m_value;
            if (unaccesspaketi.find("\ncheckbox_" + userid->m_value + "|0") != -1)
            {
            }
            else
            {
                unaccesspaketi += "\ncheckbox_" + userid->m_value + "|0";

            }

        }
        if (name && netid && onlineid) {
            player ply{};
            if (var.find("invis")->m_value != "1") {
                ply.name = name->m_value;
                ply.country = var.get("country");
                if (events::out::autohosts == true)
                {
                    name->m_values[0] += " `4[" + netid->m_value + "]``" + " `#[" + userid->m_value + "]``";

                }
                auto pos = var.find("posXY");
                if (pos && pos->m_values.size() >= 2) {
                    auto x = atoi(pos->m_values[0].c_str());
                    auto y = atoi(pos->m_values[1].c_str());
                    ply.pos = vector2_t{ float(x), float(y) };
                }
            }
            else {
                gt::send_log("Moderator entered the world. 1/2");
                ply.mod = true;
                ply.invis = true;
            }
            if (var.get("mstate") == "1" || var.get("smstate") == "1")
                ply.mod = true;
            ply.userid = var.get_int("userID");
            ply.netid = var.get_int("netID");
            if (meme.find("type|local") != -1) {
                var.find("mstate")->m_values[0] = "1";
                g_server->m_world.local = ply;
            }
            g_server->m_world.players.push_back(ply);
            auto str = var.serialize();
            utils::replace(str, "onlineID", "onlineID|");
            varlist[1] = str;
            PRINTC("new: %s\n", varlist.print().c_str());
            g_server->send(true, varlist, -1, -1);
            return true;
        }
    } break;
    }
    return false;
}
bool events::in::generictext(std::string packet) {
    PRINTC("Generic text: %s\n", packet.c_str());

    return false;
}

bool events::in::gamemessage(std::string packet) {
    PRINTC("Game message: %s\n", packet.c_str());

    if (gt::resolving_uid2) {
        if (packet.find("PERSON IGNORED") != -1) {
            g_server->send(false, "action|dialog_return\ndialog_name|friends_guilds\nbuttonClicked|showfriend");
            g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|friend_all");
        }
        else if (packet.find("Nobody is currently online with the name") != -1) {
            gt::resolving_uid2 = false;
            gt::send_log("Target is offline, cant find uid.");
        }
        else if (packet.find("Clever perhaps") != -1) {
            gt::resolving_uid2 = false;
            gt::send_log("Target is a moderator, can't ignore them.");
        }
    }
    return false;
}

bool events::in::sendmapdata(gameupdatepacket_t* packet) {
    g_server->m_world = {};
    auto extended = utils::get_extended(packet);
    extended += 4;
    auto data = extended + 6;
    auto name_length = *(short*)data;

    char* name = new char[name_length + 1];
    memcpy(name, data + sizeof(short), name_length);
    char none = '\0';
    memcpy(name + name_length, &none, 1);

    g_server->m_world.name = std::string(name);
    g_server->m_world.connected = true;
    delete[] name;
    PRINTC("world name is %s\n", g_server->m_world.name.c_str());

    return false;
}

bool events::in::state(gameupdatepacket_t* packet) {
    if (!g_server->m_world.connected)
        return false;
    if (packet->m_player_flags == -1)
        return false;

    auto& players = g_server->m_world.players;
    for (auto& player : players) {
        if (player.netid == packet->m_player_flags) {
            player.pos = vector2_t{ packet->m_vec_x, packet->m_vec_y };
            PRINTC("player %s position is %.0f %.0f\n", player.name.c_str(), player.pos.m_x, player.pos.m_y);
            break;
        }
    }
    return false;
}

bool events::in::tracking(std::string packet) {
    PRINTC("Tracking packet: %s\n", packet.c_str());
    if (packet.find("eventName|102_PLAYER.AUTHENTICATION") != -1)
    {
        string wlbalance = packet.substr(packet.find("Worldlock_balance|") + 18, packet.length() - packet.find("Worldlock_balance|") - 1);

        if (wlbalance.find("PLAYER.") != -1)
        {
            gt::send_log("`9World Lock Balance: `#0");
        }
        else
        {
            gt::send_log("`9World Lock Balance: `#" + wlbalance);

        }
        if (packet.find("Authenticated|1") != -1)
        {
            gt::send_log("`9Player Authentication `2Successfuly.");
        }
        else
        {
            gt::send_log("`9Player Authentication `4Failed.");
        }

    }
    if (packet.find("eventName|100_MOBILE.START") != -1)
    {
        string gem = packet.substr(packet.find("Gems_balance|") + 13, packet.length() - packet.find("Gems_balance|") - 1);
        string level = packet.substr(packet.find("Level|") + 6, packet.length() - packet.find("Level|") - 1);
        string uidd = packet.substr(packet.find("GrowId|") + 7, packet.length() - packet.find("GrowId|") - 1);
        gt::send_log("`9Gems Balance: `#" + gem);
        gt::send_log("`9Account Level: `#" + level);
        gt::send_log("`9Your Current UID: `#" + uidd);
        currentuid = uidd;
    }
    if (packet.find("eventName|300_WORLD_VISIT") != -1)
    {
        if (packet.find("Locked|0") != -1)
        {
            gt::send_log("`4This world is not locked by a world lock.");
        }
        else
        {
            gt::send_log("`2This world is locked by a world lock.");

            if (packet.find("World_owner|") != -1)
            {
                string uidd = packet.substr(packet.find("World_owner|") + 12, packet.length() - packet.find("World_owner|") - 1);
                gt::send_log("`9World Owner UID: `#" + uidd);
                owneruid = uidd;
            }
        }
    }

    return true;
}
