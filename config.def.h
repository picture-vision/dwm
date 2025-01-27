#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 10;       /* vertical padding of bar */
static const int sidepad            = 10;       /* horizontal padding of bar */
static const int user_bh            = 32;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "CaskaydiaCove Nerd Font:size=12" };
static const char dmenufont[]       = "CaskaydiaCove Nerd Font:size=12";
static const char col_background[]       = "#2e3440";
static const char col_secondary[]       = "#434c5e";
static const char col_primary_light[]       = "#a3be8c";
static const char col_selected[]       = "#ebcb8b";
static const char col_primary[]        = "#d8a657";
static const unsigned int baralpha = 0xd0;
// static const unsigned int baralpha = OPAQUE;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_primary_light, col_background, col_background },
	[SchemeSel]  = { col_primary, col_background,  col_primary  },
	[SchemeStatus]  = { col_primary_light, col_background,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { col_primary, col_background,  "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm]  = { col_primary_light, col_background,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]  = { col_primary, col_background,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]  = { col_primary_light, col_background,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};
static const unsigned int alphas[][3]      = {
    /*               fg      bg        border*/
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
    [SchemeSel]  = { OPAQUE, baralpha, borderalpha },
    [SchemeStatus] = { OPAQUE, baralpha, borderalpha },
    [SchemeTagsSel]  = { OPAQUE, baralpha, borderalpha },
    [SchemeTagsNorm] = { OPAQUE, baralpha, borderalpha },
    [SchemeInfoSel]  = { OPAQUE, baralpha, borderalpha },
    [SchemeInfoNorm]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { " ", " ", " ", "󰂫 ", "󰬱 ", "󰊯 ", "󰛮 ", " ", " " };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class	                instance    title       tags mask     isfloating   monitor    float x,y,w,h         floatborderpx*/
	{ "Gimp",	                NULL,       NULL,       1 << 3,       1,           -1,        10,52,1920,1200,      5 },
	{ "looking-glass-client",	NULL,       NULL,       1 << 4,       1,           -1,        10,52,3420,1378,      0 },
	{ "Chromium",			NULL,       NULL,       1 << 5,       0,           -1,        10,52,500,500,        5 },
	{ "SPOTIFY",	            	NULL,       NULL,       1 << 7,       1,           -1,        10,52,1200,700,       5 },
	{ "AUDIOVIS",	            	NULL,       NULL,       1 << 7,       1,           -1,        2230,52,1200,700,     5 },
	{ "firefox-esr",            	NULL,       NULL,       1 << 8,       0,           -1,        10,52,500,500,        5 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_background, "-nf", col_primary_light, "-sb", col_secondary, "-sf", col_selected, "-x", "1000", "-y", "300", "-z", "1440", NULL };
static const char *dmenupasscmd[] = { "dmenu_pass", "-m", dmenumon, "-fn", dmenufont, "-nb", col_background, "-nf", col_primary_light, "-sb", col_primary, "-sf", col_selected, "-x", "1000", "-y", "300", "-z", "1440", NULL };
static const char *dmenupotpcmd[] = { "dmenu_pass_otp", "-m", dmenumon, "-fn", dmenufont, "-nb", col_background, "-nf", col_primary_light, "-sb", col_primary, "-sf", col_selected, "-x", "1000", "-y", "300", "-z", "1440", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x50", NULL };
static const char *screenshotcmd[]  = { "screenshot", NULL };

#include "movestack.c"
static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenupasscmd } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = dmenupotpcmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_s,      togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,             XK_s,  spawn,	   {.v = screenshotcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
 	{ MODKEY|Mod1Mask,              XK_h,      incrgaps,       {.i = +1 } },
 	{ MODKEY|Mod1Mask,              XK_l,      incrgaps,       {.i = -1 } },
 	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
 	{ MODKEY|Mod1Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
 	{ MODKEY|Mod1Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
 	{ MODKEY|Mod1Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
 	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
 	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
 	{ MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
 	{ MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },
 	{ MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
 	{ MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
 	{ MODKEY|Mod1Mask,              XK_y,      incrohgaps,     {.i = +1 } },
 	{ MODKEY|Mod1Mask,              XK_o,      incrohgaps,     {.i = -1 } },
 	{ MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
 	{ MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,			XK_w,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,           XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
  { 0,                            XF86XK_AudioPrev,         spawn,     SHCMD("playerctl previous; kill -45 $(pidof dwmblocks)")},
  { 0,                            XF86XK_AudioPlay,         spawn,     SHCMD("playerctl play-pause; kill -45 $(pidof dwmblocks)")},
  { 0,                            XF86XK_AudioNext,         spawn,     SHCMD("playerctl next; kill -45 $(pidof dwmblocks)")},
  { 0,                            XF86XK_AudioMute,         spawn,     SHCMD("pamixer --toggle-mute; kill -44 $(pidof dwmblocks)")},
  { 0,                            XF86XK_AudioLowerVolume,  spawn,     SHCMD("pamixer --decrease 5; kill -44 $(pidof dwmblocks)")},
  { 0,                            XF86XK_AudioRaiseVolume,  spawn,     SHCMD("pamixer --increase 5; kill -44 $(pidof dwmblocks)")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
