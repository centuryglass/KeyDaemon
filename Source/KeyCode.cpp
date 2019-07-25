#include "KeyCode.h"
#include "KDDebug.h"
#include <algorithm>
#include <linux/input-event-codes.h>
#include <unistd.h>
#include <errno.h>

#ifdef KD_DEBUG
// Print the application and class name before all info/error messages:
static const constexpr char* messagePrefix = "KeyDaemon::KeyCode::";
#endif


namespace KeyDaemon
{
    namespace KeyCode
    {
        /**
         * @brief  Parses a keyboard code from a C string.
         *
         * @param codeString  The string representation of a key code.
         *
         * @return            The represented code, or -1 if the code was
         *                    invalid.
         */
        int parseCode(const char* codeString);
    }
}


// Parses a keyboard code from a C string.
int KeyDaemon::KeyCode::parseCode(const char* codeString)
{
    char* endPtr = nullptr;
    errno = 0;
    long codeValue = strtol(codeString, &endPtr, 10);
    switch (errno)
    {
        case 0:
            break;
        case ERANGE:
            DBG(messagePrefix << __func__ << ": Key code \"" << codeString
                    << "\" exceeds numeric limits.");
            return -1;
        default:
            DBG(messagePrefix << __func__ << ": Unexpected error code "
                    << errno << " when processing argument \"" << codeString
                    << "\".");
            return -1;
    }
    if (endPtr == codeString)
    {
        DBG(messagePrefix << __func__
                << ": Failed to parse key code from argument \"" << codeString
                << "\".");
        return -1;
    }
    if (*endPtr != '\0')
    {
        DBG(messagePrefix << __func__ << ": Argument \"" << codeString 
                << "\" contained non-numeric values.");
        return -1;
    }
    if (codeValue <= KEY_RESERVED || codeValue >= KEY_UNKNOWN)
    {
        DBG(messagePrefix << __func__ << ": Argument \"" << codeString 
                << "\" is not within the range of valid keyboard codes.");
        return -1;
    }
    return static_cast<int>(codeValue);
}


// Parses keyboard codes from a set of command line arguments, returning the
// list of codes only when no errors or invalid codes are encountered.
std::vector<int> KeyDaemon::KeyCode::parseCodes(const int argc, char** argv)
{
    using std::vector;
    // Launching via exec doesn't seem to pass the daemon's executable name
    // as an argument, so argv[0] may or may not be a key code.
    int startIndex = 1;
    if (argc > 0 && parseCode(argv[0]) != -1)
    {
        DBG_V(messagePrefix << __func__ << ": Index zero was a valid key code,"
                << " startIndex is actually zero.");
        startIndex = 0;
    }
    if ((argc - startIndex) > KD_KEY_LIMIT)
    {
        DBG(messagePrefix << __func__ << ": Key code argument count "
                << (argc - startIndex) << " exceeds maximum key code count " 
                << KD_KEY_LIMIT);
        return vector<int>();
    }
    if (argc <= startIndex)
    {
        DBG(messagePrefix << __func__ <<  ": Key code argument count "
                << argc << " is less than expected minimum count of 1.");
        return vector<int>();
    }
    vector<int> keyCodes;
    for (int i = startIndex; i < argc; i++)
    {
        int argCode = parseCode(argv[i]);
        if (argCode != -1)
        {
            keyCodes.push_back((int) argCode);
        }
        else
        {
            return vector<int>();
        }
    }
    // Make sure key codes are sorted for fast code lookup:
    std::sort(keyCodes.begin(), keyCodes.end());
    DBG_V(messagePrefix << __func__ << ": Returning " << keyCodes.size()
            << " sorted key codes.");
    return keyCodes;
}


// Gets a string representation of a linux key code.
std::string KeyDaemon::KeyCode::getKeyString(const int keyCode)
{
    switch (keyCode)
    {
        case KEY_RESERVED:
            return "RESERVED";
        case KEY_ESC:
            return "ESC";
        case KEY_1:
            return "1";
        case KEY_2:
            return "2";
        case KEY_3:
            return "3";
        case KEY_4:
            return "4";
        case KEY_5:
            return "5";
        case KEY_6:
            return "6";
        case KEY_7:
            return "7";
        case KEY_8:
            return "8";
        case KEY_9:
            return "9";
        case KEY_0:
            return "0";
        case KEY_MINUS:
            return "MINUS";
        case KEY_EQUAL:
            return "EQUAL";
        case KEY_BACKSPACE:
            return "BACKSPACE";
        case KEY_TAB:
            return "TAB";
        case KEY_Q:
            return "Q";
        case KEY_W:
            return "W";
        case KEY_E:
            return "E";
        case KEY_R:
            return "R";
        case KEY_T:
            return "T";
        case KEY_Y:
            return "Y";
        case KEY_U:
            return "U";
        case KEY_I:
            return "I";
        case KEY_O:
            return "O";
        case KEY_P:
            return "P";
        case KEY_LEFTBRACE:
            return "LEFTBRACE";
        case KEY_RIGHTBRACE:
            return "RIGHTBRACE";
        case KEY_ENTER:
            return "ENTER";
        case KEY_LEFTCTRL:
            return "LEFTCTRL";
        case KEY_A:
            return "A";
        case KEY_S:
            return "S";
        case KEY_D:
            return "D";
        case KEY_F:
            return "F";
        case KEY_G:
            return "G";
        case KEY_H:
            return "H";
        case KEY_J:
            return "J";
        case KEY_K:
            return "K";
        case KEY_L:
            return "L";
        case KEY_SEMICOLON:
            return "SEMICOLON";
        case KEY_APOSTROPHE:
            return "APOSTROPHE";
        case KEY_GRAVE:
            return "GRAVE";
        case KEY_LEFTSHIFT:
            return "LEFTSHIFT";
        case KEY_BACKSLASH:
            return "BACKSLASH";
        case KEY_Z:
            return "Z";
        case KEY_X:
            return "X";
        case KEY_C:
            return "C";
        case KEY_V:
            return "V";
        case KEY_B:
            return "B";
        case KEY_N:
            return "N";
        case KEY_M:
            return "M";
        case KEY_COMMA:
            return "COMMA";
        case KEY_DOT:
            return "DOT";
        case KEY_SLASH:
            return "SLASH";
        case KEY_RIGHTSHIFT:
            return "RIGHTSHIFT";
        case KEY_KPASTERISK:
            return "KPASTERISK";
        case KEY_LEFTALT:
            return "LEFTALT";
        case KEY_SPACE:
            return "SPACE";
        case KEY_CAPSLOCK:
            return "CAPSLOCK";
        case KEY_F1:
            return "F1";
        case KEY_F2:
            return "F2";
        case KEY_F3:
            return "F3";
        case KEY_F4:
            return "F4";
        case KEY_F5:
            return "F5";
        case KEY_F6:
            return "F6";
        case KEY_F7:
            return "F7";
        case KEY_F8:
            return "F8";
        case KEY_F9:
            return "F9";
        case KEY_F10:
            return "F10";
        case KEY_NUMLOCK:
            return "NUMLOCK";
        case KEY_SCROLLLOCK:
            return "SCROLLLOCK";
        case KEY_KP7:
            return "KP7";
        case KEY_KP8:
            return "KP8";
        case KEY_KP9:
            return "KP9";
        case KEY_KPMINUS:
            return "KPMINUS";
        case KEY_KP4:
            return "KP4";
        case KEY_KP5:
            return "KP5";
        case KEY_KP6:
            return "KP6";
        case KEY_KPPLUS:
            return "KPPLUS";
        case KEY_KP1:
            return "KP1";
        case KEY_KP2:
            return "KP2";
        case KEY_KP3:
            return "KP3";
        case KEY_KP0:
            return "KP0";
        case KEY_KPDOT:
            return "KPDOT";
        case KEY_ZENKAKUHANKAKU:
            return "ZENKAKUHANKAKU";
        case KEY_102ND:
            return "102ND";
        case KEY_F11:
            return "F11";
        case KEY_F12:
            return "F12";
        case KEY_RO:
            return "RO";
        case KEY_KATAKANA:
            return "KATAKANA";
        case KEY_HIRAGANA:
            return "HIRAGANA";
        case KEY_HENKAN:
            return "HENKAN";
        case KEY_KATAKANAHIRAGANA:
            return "KATAKANAHIRAGANA";
        case KEY_MUHENKAN:
            return "MUHENKAN";
        case KEY_KPJPCOMMA:
            return "KPJPCOMMA";
        case KEY_KPENTER:
            return "KPENTER";
        case KEY_RIGHTCTRL:
            return "RIGHTCTRL";
        case KEY_KPSLASH:
            return "KPSLASH";
        case KEY_SYSRQ:
            return "SYSRQ";
        case KEY_RIGHTALT:
            return "RIGHTALT";
        case KEY_LINEFEED:
            return "LINEFEED";
        case KEY_HOME:
            return "HOME";
        case KEY_UP:
            return "UP";
        case KEY_PAGEUP:
            return "PAGEUP";
        case KEY_LEFT:
            return "LEFT";
        case KEY_RIGHT:
            return "RIGHT";
        case KEY_END:
            return "END";
        case KEY_DOWN:
            return "DOWN";
        case KEY_PAGEDOWN:
            return "PAGEDOWN";
        case KEY_INSERT:
            return "INSERT";
        case KEY_DELETE:
            return "DELETE";
        case KEY_MACRO:
            return "MACRO";
        case KEY_MUTE:
            return "MUTE";
        case KEY_VOLUMEDOWN:
            return "VOLUMEDOWN";
        case KEY_VOLUMEUP:
            return "VOLUMEUP";
        case KEY_POWER:
            return "POWER";
        case KEY_KPEQUAL:
            return "KPEQUAL";
        case KEY_KPPLUSMINUS:
            return "KPPLUSMINUS";
        case KEY_PAUSE:
            return "PAUSE";
        case KEY_SCALE:
            return "SCALE";
        case KEY_KPCOMMA:
            return "KPCOMMA";
        case KEY_HANGEUL:
            return "HANGEUL";
        case KEY_HANJA:
            return "HANJA";
        case KEY_YEN:
            return "YEN";
        case KEY_LEFTMETA:
            return "LEFTMETA";
        case KEY_RIGHTMETA:
            return "RIGHTMETA";
        case KEY_COMPOSE:
            return "COMPOSE";
        case KEY_STOP:
            return "STOP";
        case KEY_AGAIN:
            return "AGAIN";
        case KEY_PROPS:
            return "PROPS";
        case KEY_UNDO:
            return "UNDO";
        case KEY_FRONT:
            return "FRONT";
        case KEY_COPY:
            return "COPY";
        case KEY_OPEN:
            return "OPEN";
        case KEY_PASTE:
            return "PASTE";
        case KEY_FIND:
            return "FIND";
        case KEY_CUT:
            return "CUT";
        case KEY_HELP:
            return "HELP";
        case KEY_MENU:
            return "MENU";
        case KEY_CALC:
            return "CALC";
        case KEY_SETUP:
            return "SETUP";
        case KEY_SLEEP:
            return "SLEEP";
        case KEY_WAKEUP:
            return "WAKEUP";
        case KEY_FILE:
            return "FILE";
        case KEY_SENDFILE:
            return "SENDFILE";
        case KEY_DELETEFILE:
            return "DELETEFILE";
        case KEY_XFER:
            return "XFER";
        case KEY_PROG1:
            return "PROG1";
        case KEY_PROG2:
            return "PROG2";
        case KEY_WWW:
            return "WWW";
        case KEY_MSDOS:
            return "MSDOS";
        case KEY_SCREENLOCK:
            return "SCREENLOCK";
        case KEY_ROTATE_DISPLAY:
            return "ROTATE_DISPLAY";
        case KEY_CYCLEWINDOWS:
            return "CYCLEWINDOWS";
        case KEY_MAIL:
            return "MAIL";
        case KEY_BOOKMARKS:
            return "BOOKMARKS";
        case KEY_COMPUTER:
            return "COMPUTER";
        case KEY_BACK:
            return "BACK";
        case KEY_FORWARD:
            return "FORWARD";
        case KEY_CLOSECD:
            return "CLOSECD";
        case KEY_EJECTCD:
            return "EJECTCD";
        case KEY_EJECTCLOSECD:
            return "EJECTCLOSECD";
        case KEY_NEXTSONG:
            return "NEXTSONG";
        case KEY_PLAYPAUSE:
            return "PLAYPAUSE";
        case KEY_PREVIOUSSONG:
            return "PREVIOUSSONG";
        case KEY_STOPCD:
            return "STOPCD";
        case KEY_RECORD:
            return "RECORD";
        case KEY_REWIND:
            return "REWIND";
        case KEY_PHONE:
            return "PHONE";
        case KEY_ISO:
            return "ISO";
        case KEY_CONFIG:
            return "CONFIG";
        case KEY_HOMEPAGE:
            return "HOMEPAGE";
        case KEY_REFRESH:
            return "REFRESH";
        case KEY_EXIT:
            return "EXIT";
        case KEY_MOVE:
            return "MOVE";
        case KEY_EDIT:
            return "EDIT";
        case KEY_SCROLLUP:
            return "SCROLLUP";
        case KEY_SCROLLDOWN:
            return "SCROLLDOWN";
        case KEY_KPLEFTPAREN:
            return "KPLEFTPAREN";
        case KEY_KPRIGHTPAREN:
            return "KPRIGHTPAREN";
        case KEY_NEW:
            return "NEW";
        case KEY_REDO:
            return "REDO";
        case KEY_F13:
            return "F13";
        case KEY_F14:
            return "F14";
        case KEY_F15:
            return "F15";
        case KEY_F16:
            return "F16";
        case KEY_F17:
            return "F17";
        case KEY_F18:
            return "F18";
        case KEY_F19:
            return "F19";
        case KEY_F20:
            return "F20";
        case KEY_F21:
            return "F21";
        case KEY_F22:
            return "F22";
        case KEY_F23:
            return "F23";
        case KEY_F24:
            return "F24";
        case KEY_PLAYCD:
            return "PLAYCD";
        case KEY_PAUSECD:
            return "PAUSECD";
        case KEY_PROG3:
            return "PROG3";
        case KEY_PROG4:
            return "PROG4";
        case KEY_DASHBOARD:
            return "DASHBOARD";
        case KEY_SUSPEND:
            return "SUSPEND";
        case KEY_CLOSE:
            return "CLOSE";
        case KEY_PLAY:
            return "PLAY";
        case KEY_FASTFORWARD:
            return "FASTFORWARD";
        case KEY_BASSBOOST:
            return "BASSBOOST";
        case KEY_PRINT:
            return "PRINT";
        case KEY_HP:
            return "HP";
        case KEY_CAMERA:
            return "CAMERA";
        case KEY_SOUND:
            return "SOUND";
        case KEY_QUESTION:
            return "QUESTION";
        case KEY_EMAIL:
            return "EMAIL";
        case KEY_CHAT:
            return "CHAT";
        case KEY_SEARCH:
            return "SEARCH";
        case KEY_CONNECT:
            return "CONNECT";
        case KEY_FINANCE:
            return "FINANCE";
        case KEY_SPORT:
            return "SPORT";
        case KEY_SHOP:
            return "SHOP";
        case KEY_ALTERASE:
            return "ALTERASE";
        case KEY_CANCEL:
            return "CANCEL";
        case KEY_BRIGHTNESSDOWN:
            return "BRIGHTNESSDOWN";
        case KEY_BRIGHTNESSUP:
            return "BRIGHTNESSUP";
        case KEY_MEDIA:
            return "MEDIA";
        case KEY_SWITCHVIDEOMODE:
            return "SWITCHVIDEOMODE";
        case KEY_KBDILLUMTOGGLE:
            return "KBDILLUMTOGGLE";
        case KEY_KBDILLUMDOWN:
            return "KBDILLUMDOWN";
        case KEY_KBDILLUMUP:
            return "KBDILLUMUP";
        case KEY_SEND:
            return "SEND";
        case KEY_REPLY:
            return "REPLY";
        case KEY_FORWARDMAIL:
            return "FORWARDMAIL";
        case KEY_SAVE:
            return "SAVE";
        case KEY_DOCUMENTS:
            return "DOCUMENTS";
        case KEY_BATTERY:
            return "BATTERY";
        case KEY_BLUETOOTH:
            return "BLUETOOTH";
        case KEY_WLAN:
            return "WLAN";
        case KEY_UWB:
            return "UWB";
        default:
            return "Invalid";
     }
}
