#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 32;    /* snap pixel */
static const unsigned int gappih = 20;  /* horiz inner gap between windows */
static const unsigned int gappiv = 10;  /* vert inner gap between windows */
static const unsigned int gappoh =
    10; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov =
    10; /* vert outer gap between windows and screen edge */
static int smartgaps =
    0; /* 1 means no outer gap when there is only one window */
static const int showbar = 1; /* 0 means no bar */
static const int topbar = 1;  /* 0 means bottom bar */
static const int vertpad            = 10;       /* vertical padding of bar */
static const int sidepad            = 10;       /* horizontal padding of bar */
static const int user_bh = 30; /* 0 means that dwm will calculate bar height, >=
                                 1 means dwm will user_bh as bar height */
static const char *fonts[] = {"CaskaydiaCove Nerd Font:size=12"};
static const char dmenufont[] = "CaskaydiaCove Nerd Font:size=12";
static const char col_gray1[] = "#222200";
static const char col_gray2[] = "#4444ff";
static const char col_gray3[] = "#bbbb00";
static const char col_gray4[] = "#ee0000";
static const char col_cyan[] = "#005577";

static const char col_p1[] = "#2E3440";
static const char col_p2[] = "#3B4252";
static const char col_p3[] = "#434C5E";
static const char col_p4[] = "#4C566A";

static const char col_s1[] = "#D8DEE9";
static const char col_s2[] = "#E5E9F0";
static const char col_s3[] = "#ECEFF4";

static const char col_a1[] = "#BF616A";
static const char col_a2[] = "#D08770";
static const char col_a3[] = "#EBCB8B";
static const char col_a4[] = "#A3BE8C";
static const char col_a5[] = "#B48EAD";

static const char col_f1[] = "#8FBCBB";
static const char col_f2[] = "#88C0D0";
static const char col_f3[] = "#81A1C1";
static const char col_f4[] = "#5E81AC";

static const unsigned int baralpha = 210; // 0xff;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3] = {
  /*                  fg        bg        border   */
  [SchemeNorm] =      {col_p1,  col_p1,   col_p1},
  [SchemeSel] =       {col_p4,  col_p1,   col_p1},
  [SchemeStatus] =    {col_s1,  col_p1,   col_p1}, // Statusbar right {text,background,not used but cannot be empty}
  [SchemeTagsSel] =   {col_a4,  col_p3,   col_p1}, // Tagbar left selected {text,background,not used but cannot be empty}
  [SchemeTagsNorm] =  {col_p4,  col_p1,   col_p1}, // Tagbar left unselected {text,background,not used but cannot be empty}
  [SchemeInfoSel] =   {col_f3,  col_p1,   col_p1}, // infobar middle  selected {text,background,not used but cannot be empty}
  [SchemeInfoNorm] =  {col_a3,  col_p1,   col_p1}, // infobar middle  unselected {text,background,not used but cannot be empty}
};
static const unsigned int alphas[][3] = {
    /*               fg      bg        border*/
    [SchemeNorm] = {OPAQUE, baralpha, borderalpha},
    [SchemeSel] = {OPAQUE, baralpha, borderalpha},
    [SchemeStatus] = {OPAQUE, baralpha, borderalpha},
    [SchemeTagsSel] = {OPAQUE, baralpha, borderalpha},
    [SchemeTagsNorm] = {OPAQUE, baralpha, borderalpha},
    [SchemeInfoSel] = {OPAQUE, baralpha, borderalpha},
    [SchemeInfoNorm] = {OPAQUE, baralpha, borderalpha},
};

/* tagging */
static const char *tags[] = {"󰌽", "", "", "", "󰈙", "", "", "󰛮", ""};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    {"Gimp", NULL, NULL, 0, 1, -1},
    {"Thorium-browser", NULL, NULL, 1 << 5, 0, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT                                                           \
  1 /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"|M|", centeredmaster}, 
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle}, 
    {">M>", centeredfloatingmaster},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static const char *dmenucmd[] = {"dmenu_run", "-fn", dmenufont, "-nb", "#2e3440",   "-sb", "#4c566a", "-sf", "#a3be8c", "-x", "1000", "-y", "300", "-z", "1440", NULL};
static const char *passcmd[] = {"dmenu_pass", "-fn", dmenufont, "-nb", "#2e3440",   "-sb", "#4c566a", "-sf", "#a3be8c", "-x", "1000", "-y", "300", "-z", "1440", NULL};
static const char *passotpcmd[] = {"dmenu_pass_otp", "-fn", dmenufont, "-nb", "#2e3440",   "-sb", "#4c566a", "-sf", "#a3be8c", "-x", "1000", "-y", "300", "-z", "1440", NULL};
static const char *termcmd[] = {"st", NULL};
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = {"st", "-t",     scratchpadname,
                                      "-g", "160x54", NULL};

static const Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_space, spawn, {.v = dmenucmd}},
    {MODKEY, XK_p, spawn, {.v = passcmd}},
    {MODKEY, XK_o, spawn, {.v = passotpcmd}},
    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_grave, togglescratch, {.v = scratchpadcmd}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY | ShiftMask, XK_j, rotatestack, {.i = +1}},
    {MODKEY | ShiftMask, XK_k, rotatestack, {.i = -1}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    // {MODKEY, XK_Return, zoom, {0}},
    {MODKEY | Mod1Mask, XK_u, incrgaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_u, incrgaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_i, incrigaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_i, incrigaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_o, incrogaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_o, incrogaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_6, incrihgaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_6, incrihgaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_7, incrivgaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_7, incrivgaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_8, incrohgaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_8, incrohgaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_9, incrovgaps, {.i = +1}},
    {MODKEY | Mod1Mask | ShiftMask, XK_9, incrovgaps, {.i = -1}},
    {MODKEY | Mod1Mask, XK_0, togglegaps, {0}},
    {MODKEY | Mod1Mask | ShiftMask, XK_0, defaultgaps, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_c, killclient, {0}},
    {MODKEY, XK_c, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[3]}},
    // {MODKEY, XK_o, setlayout, {.v = &layouts[4]}},
    {MODKEY | ShiftMask, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_Down, moveresize, {.v = "0x 25y 0w 0h"}},
    {MODKEY, XK_Up, moveresize, {.v = "0x -25y 0w 0h"}},
    {MODKEY, XK_Right, moveresize, {.v = "25x 0y 0w 0h"}},
    {MODKEY, XK_Left, moveresize, {.v = "-25x 0y 0w 0h"}},
    {MODKEY | ShiftMask, XK_Down, moveresize, {.v = "0x 0y 0w 25h"}},
    {MODKEY | ShiftMask, XK_Up, moveresize, {.v = "0x 0y 0w -25h"}},
    {MODKEY | ShiftMask, XK_Right, moveresize, {.v = "0x 0y 25w 0h"}},
    {MODKEY | ShiftMask, XK_Left, moveresize, {.v = "0x 0y -25w 0h"}},
    {MODKEY | ControlMask, XK_Up, moveresizeedge, {.v = "t"}},
    {MODKEY | ControlMask, XK_Down, moveresizeedge, {.v = "b"}},
    {MODKEY | ControlMask, XK_Left, moveresizeedge, {.v = "l"}},
    {MODKEY | ControlMask, XK_Right, moveresizeedge, {.v = "r"}},
    {MODKEY | ControlMask | ShiftMask, XK_Up, moveresizeedge, {.v = "T"}},
    {MODKEY | ControlMask | ShiftMask, XK_Down, moveresizeedge, {.v = "B"}},
    {MODKEY | ControlMask | ShiftMask, XK_Left, moveresizeedge, {.v = "L"}},
    {MODKEY | ControlMask | ShiftMask, XK_Right, moveresizeedge, {.v = "R"}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8){MODKEY | ShiftMask, XK_q, quit, {0}},
    {0, XF86XK_AudioMute, spawn,
     SHCMD("pamixer --toggle-mute; kill -44 $(pidof dwmblocks)")},
    {0, XF86XK_AudioLowerVolume, spawn,
     SHCMD("pamixer --decrease 5; kill -44 $(pidof dwmblocks)")},
    {0, XF86XK_AudioRaiseVolume, spawn,
     SHCMD("pamixer --increase 5; kill -44 $(pidof dwmblocks)")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
