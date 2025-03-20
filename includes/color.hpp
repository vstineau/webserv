
#ifndef COLOR_H
#define COLOR_H

//------------------------------
// BLINK

#define BLINK "\033[5m"

//------------------------------
// PIXEL COLOR

#define P_WHITE 0xFFFFFFFF
#define P_RED 0x00FF0000
#define P_CYAN 0x0000FFFF
#define P_MAGENTA 0x00FF00FF
#define P_YELLOW 0x00FFFF00
#define P_GREEN 0x0000FF00
#define P_BLUE 0x000000FF
#define P_BLACK 0x00000000
//------------------------------

//------------------------------
// REGULAR COLOR

#define BLACK "\001\033[0;30m\002"
#define RED "\033[0;31m\002"
#define GREEN "\033[0;32m\002"
#define YELLOW "\033[0;33m\002"
#define BLUE "\033[0;34m\002"
#define MAGENTA "\033[0;35m\002"
#define CYAN "\033[0;36m\002"
#define WHITE "\033[0;37m\002"
#define RESET "\001\033[0m\002"
//------------------------------

//------------------------------
// BOLD

#define B_BLACK "\001\033[1;30m\002"
#define B_RED "\001\033[1;31m\002"
#define B_GREEN "\001\033[1;32m\002"
#define B_YELLOW "\001\033[1;33m\002"
#define B_BLUE "\001\033[1;34m\002"
#define B_MAGENTA "\001\033[1;35m\002"
#define B_CYAN "\001\033[1;36m\002"
#define B_WHITE "\001\033[1;37m\002"
//------------------------------

//------------------------------
// UNDERLINE COLOR

#define U_BLACK "\001\033[4;30m\002"
#define U_RED "\001\033[4;31m\002"
#define U_GREEN "\001\033[4;32m\002"
#define U_YELLOW "\001\033[4;33m\002"
#define U_BLUE "\001\033[4;34m\002"
#define U_MAGENTA "\001\033[4;35m\002"
#define U_CYAN "\001\033[4;36m\002"
#define U_WHITE "\001\033[4;37m\002"
//------------------------------

//------------------------------
// BACKGROUD COLOR

#define BG_BLACK "\001\033[40m\002"
#define BG_RED "\001\033[41m\002"
#define BG_GREEN "\001\033[42m\002"
#define BG_YELLOW "\001\033[43m\002"
#define BG_BLUE "\001\033[44m\002"
#define BG_MAGENTA "\001\033[45m\002"
#define BG_CYAN "\001\033[46m\002"
#define BG_WHITE "\001\033[47m\002"
//------------------------------

//------------------------------
// HIGHT INTENSITY COLOR (plus clair)

#define HI_BLACK "\001\033[90m\002"
#define HI_RED "\001\033[91m\002"
#define HI_GREEN "\001\033[92m\002"
#define HI_YELLOW "\001\033[93m\002"
#define HI_BLUE "\001\033[94m\002"
#define HI_MAGENTA "\001\033[95m\002"
#define HI_CYAN "\001\033[96m\002"
#define HI_WHITE "\001\033[97m\002"
//------------------------------

//------------------------------
// UNDERLINE HIGHT INTENSITY COLOR (plus clair)

#define UHI_BLACK "\001\033[4;90m\002"
#define UHI_RED "\001\033[4;91m\002"
#define UHI_GREEN "\001\033[4;92m\002"
#define UHI_YELLOW "\001\033[4;93m\002"
#define UHI_BLUE "\001\001\033[4;94m\002"
#define UHI_MAGENTA "\001\033[4;95m\002"
#define UHI_CYAN "\001\033[4;96m\002"
#define UHI_WHITE "\001\033[4;97m\002"
//------------------------------

//------------------------------
// BOLD HIGHT INTENSITY COLOR (plus clair)

#define BHI_BLACK "\001\033[1;90m\002"
#define BHI_RED "\001\033[1;91m\002"
#define BHI_GREEN "\001\033[1;92m\002"
#define BHI_YELLOW "\001\033[1;93m\002"
#define BHI_BLUE "\001\033[1;94m\002"
#define BHI_MAGENTA "\001\033[1;95m\002"
#define BHI_CYAN "\001\033[1;96m\002"
#define BHI_WHITE "\001\033[1;97m\002"
//------------------------------

//------------------------------
// HIGH INTENSITY BACKGROUND COLOR (plus clair)

#define BGHI_BLACK "\001\033[0;100m\002"
#define BGHI_RED "\001\033[0;101m\002"
#define BGHI_GREEN "\001\033[0;102m\002"
#define BGHI_YELLOW "\001\033[0;103m\002"
#define BGHI_BLUE "\001\033[0;104m\002"
#define BGHI_MAGENTA "\001\033[0;105m\002"
#define BGHI_CYAN "\001\033[0;106m\002"
#define BGHI_WHITE "\001\033[0;107m\002"
//------------------------------

#endif
