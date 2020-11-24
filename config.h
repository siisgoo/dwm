/* See LICENSE file for copyright and license details. */
#include "mpdcontrol.c"

void fibonacci(Monitor*, int);
void spiral(Monitor*);

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Anonymous Pro:size=12" };
static const char dmenufont[]       = { "Anonymous Pro:size=12" }; //"monospace:size=12";
static const char col_gray1[]       = "#222222";	//222222 - default
static const char col_gray2[]       = "#444444";	//444444 - default
static const char col_gray3[]       = "#bbbbbb";	//bbbbbb - default
static const char col_gray4[]       = "#eeeeee";	//eeeeee - default
static const char col_cyan[]        = "#005577";	//005577 - default
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "code", "term", "web", "chat", "book", "music", "games", "graph", "etc" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class		instance	title	tags mask	isfloating   monitor */
	{ "firefox",		NULL,		NULL,	0,		0,	-1 },
	{ "KeePass2",		NULL,		NULL,	0,		1,	-1 },
	{ "st-256color",	NULL,		NULL,	1 << 1,		0,	-1 },
	{ "IDE",		NULL,		NULL,	1 << 0,		0,	-1 },
	{ "music-pleer",	NULL,		NULL,	1 << 5,		0,	-1 },
	{ "Luakit",		NULL,		NULL,	1 << 2,		0,	-1 },
	{ "Steam",		NULL,		NULL,	1 << 6,		1,	-1 },
	{ NULL,			NULL,		"Steam",1 << 6,		1,	-1 },
	{ "Gimp",		NULL,		NULL,	1 << 7,		1,	-1 },
	{ "Sxiv",		NULL,		NULL,	0,		1,	-1 },
	{ "mpv",		NULL,		NULL,	0,		1,	-1 },
	{ "Zathura",		NULL,		NULL,	1 << 4,		0,	-1 },
	{ "TelegramDesktop",	NULL,		NULL,	1 << 3,		0,	-1 },
	{ "float",		NULL,		NULL,	0,		1,	-1 },
	{ NULL,			NULL,	"youtube",	1 << 8,		1,	-1 },
	{ "whatsdesk",		NULL,		NULL,	1 << 3,		0,	-1 },
	{ "diskcord",		NULL,		NULL,	1 << 3,		1,	-1 },
	/* libreoffice */
	{ "libreoffice",	NULL,		NULL,	1 << 7,		0,	-1 },
	{ NULL,			"libreoffice",	NULL,	1 << 7,		0,	-1 },
	{ NULL,			NULL,		"LibreOffice", 1 << 7,	1,	-1 },
	{ "Soffice",		NULL,		NULL,	1 << 7,		1,	-1 },
	/*    */
	{ "zoom",		"zoom",		NULL,	1 << 3,		0,	-1 },
	{ "Thunderbird",	NULL,		NULL,	1 << 3,		1,	-1 },
};

/* layout(s) */
static const float mfact     = 0.5;   /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;     /* number of clients in master area */
static const int resizehints = 1;     /* 1 means respect size hints in tiled resizals */
static const int attachdirection = 0; /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */ 

static const Layout layouts[] = {
	/* symbol	arrange function */
	{ "[s]",	spiral },	/* first entry is default */
	{ "[f]",	NULL },		/* no layout function means floating behavior */
	{ "[m]",	monocle },
	{ "[t]",	tile },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *nm_dmenucmd[] = { "networkmanager_dmenu", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *passmenucmd[] = { "passmenu2", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *clipmenucmd[] = { "clipmenu", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *mpdmenucmd[] = { "mpdmenu",  "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

// REWRITE THIS IN CLEAR C CODE
static const char *take_scrotcmd[] = { "dwm_take_screenshot", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key				function	argument */
	{ NULL,     	    			XF86XK_AudioNext,	    	mpd_switch_track,	{.i = -1 } },
	{ NULL,				            XF86XK_AudioPrev,	    	mpd_switch_track,	{.i = +1 } },
	{ ALTKEY,		            	XF86XK_AudioRaiseVolume,	mpd_volume_ctl,		{.i = +1 } },
	{ ALTKEY,              			XF86XK_AudioLowerVolume,	mpd_volume_ctl,		{.i = -1 } },
	{ NULL,				            XF86XK_AudioPlay,	    	mpd_switch_playback,	{0} },
	{ NULL,	            			XK_Print,			spawn,			{.v = take_scrotcmd} },
	{ MODKEY,           			XK_n,			    spawn,			{.v = nm_dmenucmd} },
	{ MODKEY,                       XK_p,				spawn,			{.v = dmenucmd } },
	{ MODKEY,                       XK_c,				spawn,			{.v = clipmenucmd } },
	{ MODKEY|ShiftMask,     		XK_m,				spawn,			{.v = mpdmenucmd } },
	{ MODKEY|ShiftMask,	        	XK_p,				spawn,			{.v = passmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,			spawn,			{.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,			XK_t,	   setlayout,	   {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
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
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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

void
fibonacci(Monitor *mon, int s) {
	unsigned int i, n, nx, ny, nw, nh;
	Client *c;

	for(n = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next), n++);
	if(n == 0)
		return;
	
	nx = mon->wx;
	ny = 0;
	nw = mon->ww;
	nh = mon->wh;
	
	for(i = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next)) {
		if((i % 2 && nh / 2 > 2 * c->bw)
		   || (!(i % 2) && nw / 2 > 2 * c->bw)) {
			if(i < n - 1) {
				if(i % 2)
					nh /= 2;
				else
					nw /= 2;
				if((i % 4) == 2 && !s)
					nx += nw;
				else if((i % 4) == 3 && !s)
					ny += nh;
			}
			if((i % 4) == 0) {
				if(s)
					ny += nh;
				else
					ny -= nh;
			}
			else if((i % 4) == 1)
				nx += nw;
			else if((i % 4) == 2)
				ny += nh;
			else if((i % 4) == 3) {
				if(s)
					nx += nw;
				else
					nx -= nw;
			}
			if(i == 0)
			{
				if(n != 1)
					nw = mon->ww * mon->mfact;
				ny = mon->wy;
			}
			else if(i == 1)
				nw = mon->ww - nw;
			i++;
		}
		resize(c, nx, ny, nw - 2 * c->bw, nh - 2 * c->bw, False);
	}
}

void
spiral(Monitor *mon) {
	fibonacci(mon, 0);
}
