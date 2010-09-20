#pragma once
#include <stdbool.h>

/**
	Event possible event that can be handled by this module.
*/
typedef enum
{
	EVENT_UNKNOWN	= 0,
	EVENT_FIRST		= 0,
	EVENT_QUIT      = 7,
	EVENT_KEY_BACKSPACE	= 8,
	EVENT_KEY_TAB		= 9,
	EVENT_KEY_CLEAR		= 12,
	EVENT_KEY_RETURN	= 13,
	EVENT_KEY_PAUSE		= 19,
	EVENT_KEY_ESCAPE	= 27,
	EVENT_KEY_SPACE		= 32,
	EVENT_KEY_EXCLAIM	= 33,
	EVENT_KEY_QUOTEDBL	= 34,
	EVENT_KEY_HASH		= 35,
	EVENT_KEY_DOLLAR	= 36,
	EVENT_KEY_AMPERSAND	= 38,
	EVENT_KEY_QUOTE		= 39,
	EVENT_KEY_LEFTPAREN	= 40,
	EVENT_KEY_RIGHTPAREN= 41,
	EVENT_KEY_ASTERISK	= 42,
	EVENT_KEY_PLUS		= 43,
	EVENT_KEY_COMMA		= 44,
	EVENT_KEY_MINUS		= 45,
	EVENT_KEY_PERIOD	= 46,
	EVENT_KEY_SLASH		= 47,
	EVENT_KEY_0			= 48,
	EVENT_KEY_1			= 49,
	EVENT_KEY_2			= 50,
	EVENT_KEY_3			= 51,
	EVENT_KEY_4			= 52,
	EVENT_KEY_5			= 53,
	EVENT_KEY_6			= 54,
	EVENT_KEY_7			= 55,
	EVENT_KEY_8			= 56,
	EVENT_KEY_9			= 57,
	EVENT_KEY_COLON		= 58,
	EVENT_KEY_SEMICOLON	= 59,
	EVENT_KEY_LESS		= 60,
	EVENT_KEY_EQUALS	= 61,
	EVENT_KEY_GREATER	= 62,
	EVENT_KEY_QUESTION	= 63,
	EVENT_KEY_AT		= 64,
	/*
	   Skip uppercase letters
	 */
	EVENT_KEY_LEFTBRACKET	= 91,
	EVENT_KEY_BACKSLASH		= 92,
	EVENT_KEY_RIGHTBRACKET	= 93,
	EVENT_KEY_CARET		= 94,
	EVENT_KEY_UNDERSCORE= 95,
	EVENT_KEY_BACKQUOTE	= 96,
	EVENT_KEY_a			= 97,
	EVENT_KEY_b			= 98,
	EVENT_KEY_c			= 99,
	EVENT_KEY_d			= 100,
	EVENT_KEY_e			= 101,
	EVENT_KEY_f			= 102,
	EVENT_KEY_g			= 103,
	EVENT_KEY_h			= 104,
	EVENT_KEY_i			= 105,
	EVENT_KEY_j			= 106,
	EVENT_KEY_k			= 107,
	EVENT_KEY_l			= 108,
	EVENT_KEY_m			= 109,
	EVENT_KEY_n			= 110,
	EVENT_KEY_o			= 111,
	EVENT_KEY_p			= 112,
	EVENT_KEY_q			= 113,
	EVENT_KEY_r			= 114,
	EVENT_KEY_s			= 115,
	EVENT_KEY_t			= 116,
	EVENT_KEY_u			= 117,
	EVENT_KEY_v			= 118,
	EVENT_KEY_w			= 119,
	EVENT_KEY_x			= 120,
	EVENT_KEY_y			= 121,
	EVENT_KEY_z			= 122,
	EVENT_KEY_DELETE	= 127,
	/* End of ASCII mapped keysyms */

	/* International keyboard syms */
	EVENT_KEY_WORLD_0		= 160,		/* 0xA0 */
	EVENT_KEY_WORLD_1		= 161,
	EVENT_KEY_WORLD_2		= 162,
	EVENT_KEY_WORLD_3		= 163,
	EVENT_KEY_WORLD_4		= 164,
	EVENT_KEY_WORLD_5		= 165,
	EVENT_KEY_WORLD_6		= 166,
	EVENT_KEY_WORLD_7		= 167,
	EVENT_KEY_WORLD_8		= 168,
	EVENT_KEY_WORLD_9		= 169,
	EVENT_KEY_WORLD_10		= 170,
	EVENT_KEY_WORLD_11		= 171,
	EVENT_KEY_WORLD_12		= 172,
	EVENT_KEY_WORLD_13		= 173,
	EVENT_KEY_WORLD_14		= 174,
	EVENT_KEY_WORLD_15		= 175,
	EVENT_KEY_WORLD_16		= 176,
	EVENT_KEY_WORLD_17		= 177,
	EVENT_KEY_WORLD_18		= 178,
	EVENT_KEY_WORLD_19		= 179,
	EVENT_KEY_WORLD_20		= 180,
	EVENT_KEY_WORLD_21		= 181,
	EVENT_KEY_WORLD_22		= 182,
	EVENT_KEY_WORLD_23		= 183,
	EVENT_KEY_WORLD_24		= 184,
	EVENT_KEY_WORLD_25		= 185,
	EVENT_KEY_WORLD_26		= 186,
	EVENT_KEY_WORLD_27		= 187,
	EVENT_KEY_WORLD_28		= 188,
	EVENT_KEY_WORLD_29		= 189,
	EVENT_KEY_WORLD_30		= 190,
	EVENT_KEY_WORLD_31		= 191,
	EVENT_KEY_WORLD_32		= 192,
	EVENT_KEY_WORLD_33		= 193,
	EVENT_KEY_WORLD_34		= 194,
	EVENT_KEY_WORLD_35		= 195,
	EVENT_KEY_WORLD_36		= 196,
	EVENT_KEY_WORLD_37		= 197,
	EVENT_KEY_WORLD_38		= 198,
	EVENT_KEY_WORLD_39		= 199,
	EVENT_KEY_WORLD_40		= 200,
	EVENT_KEY_WORLD_41		= 201,
	EVENT_KEY_WORLD_42		= 202,
	EVENT_KEY_WORLD_43		= 203,
	EVENT_KEY_WORLD_44		= 204,
	EVENT_KEY_WORLD_45		= 205,
	EVENT_KEY_WORLD_46		= 206,
	EVENT_KEY_WORLD_47		= 207,
	EVENT_KEY_WORLD_48		= 208,
	EVENT_KEY_WORLD_49		= 209,
	EVENT_KEY_WORLD_50		= 210,
	EVENT_KEY_WORLD_51		= 211,
	EVENT_KEY_WORLD_52		= 212,
	EVENT_KEY_WORLD_53		= 213,
	EVENT_KEY_WORLD_54		= 214,
	EVENT_KEY_WORLD_55		= 215,
	EVENT_KEY_WORLD_56		= 216,
	EVENT_KEY_WORLD_57		= 217,
	EVENT_KEY_WORLD_58		= 218,
	EVENT_KEY_WORLD_59		= 219,
	EVENT_KEY_WORLD_60		= 220,
	EVENT_KEY_WORLD_61		= 221,
	EVENT_KEY_WORLD_62		= 222,
	EVENT_KEY_WORLD_63		= 223,
	EVENT_KEY_WORLD_64		= 224,
	EVENT_KEY_WORLD_65		= 225,
	EVENT_KEY_WORLD_66		= 226,
	EVENT_KEY_WORLD_67		= 227,
	EVENT_KEY_WORLD_68		= 228,
	EVENT_KEY_WORLD_69		= 229,
	EVENT_KEY_WORLD_70		= 230,
	EVENT_KEY_WORLD_71		= 231,
	EVENT_KEY_WORLD_72		= 232,
	EVENT_KEY_WORLD_73		= 233,
	EVENT_KEY_WORLD_74		= 234,
	EVENT_KEY_WORLD_75		= 235,
	EVENT_KEY_WORLD_76		= 236,
	EVENT_KEY_WORLD_77		= 237,
	EVENT_KEY_WORLD_78		= 238,
	EVENT_KEY_WORLD_79		= 239,
	EVENT_KEY_WORLD_80		= 240,
	EVENT_KEY_WORLD_81		= 241,
	EVENT_KEY_WORLD_82		= 242,
	EVENT_KEY_WORLD_83		= 243,
	EVENT_KEY_WORLD_84		= 244,
	EVENT_KEY_WORLD_85		= 245,
	EVENT_KEY_WORLD_86		= 246,
	EVENT_KEY_WORLD_87		= 247,
	EVENT_KEY_WORLD_88		= 248,
	EVENT_KEY_WORLD_89		= 249,
	EVENT_KEY_WORLD_90		= 250,
	EVENT_KEY_WORLD_91		= 251,
	EVENT_KEY_WORLD_92		= 252,
	EVENT_KEY_WORLD_93		= 253,
	EVENT_KEY_WORLD_94		= 254,
	EVENT_KEY_WORLD_95		= 255,		/* 0xFF */

	/* Numeric keypad */
	EVENT_KEY_KP0		= 256,
	EVENT_KEY_KP1		= 257,
	EVENT_KEY_KP2		= 258,
	EVENT_KEY_KP3		= 259,
	EVENT_KEY_KP4		= 260,
	EVENT_KEY_KP5		= 261,
	EVENT_KEY_KP6		= 262,
	EVENT_KEY_KP7		= 263,
	EVENT_KEY_KP8		= 264,
	EVENT_KEY_KP9		= 265,
	EVENT_KEY_KP_PERIOD	= 266,
	EVENT_KEY_KP_DIVIDE	= 267,
	EVENT_KEY_KP_MULTIPLY	= 268,
	EVENT_KEY_KP_MINUS	= 269,
	EVENT_KEY_KP_PLUS	= 270,
	EVENT_KEY_KP_ENTER	= 271,
	EVENT_KEY_KP_EQUALS	= 272,

	/* Arrows + Home/End pad */
	EVENT_KEY_UP		= 273,
	EVENT_KEY_DOWN		= 274,
	EVENT_KEY_RIGHT		= 275,
	EVENT_KEY_LEFT		= 276,
	EVENT_KEY_INSERT	= 277,
	EVENT_KEY_HOME		= 278,
	EVENT_KEY_END		= 279,
	EVENT_KEY_PAGEUP	= 280,
	EVENT_KEY_PAGEDOWN	= 281,

	/* Function keys */
	EVENT_KEY_F1		= 282,
	EVENT_KEY_F2		= 283,
	EVENT_KEY_F3		= 284,
	EVENT_KEY_F4		= 285,
	EVENT_KEY_F5		= 286,
	EVENT_KEY_F6		= 287,
	EVENT_KEY_F7		= 288,
	EVENT_KEY_F8		= 289,
	EVENT_KEY_F9		= 290,
	EVENT_KEY_F10		= 291,
	EVENT_KEY_F11		= 292,
	EVENT_KEY_F12		= 293,
	EVENT_KEY_F13		= 294,
	EVENT_KEY_F14		= 295,
	EVENT_KEY_F15		= 296,

	/* Key state modifier keys */
	EVENT_KEY_NUMLOCK	= 300,
	EVENT_KEY_CAPSLOCK	= 301,
	EVENT_KEY_SCROLLOCK	= 302,
	EVENT_KEY_RSHIFT	= 303,
	EVENT_KEY_LSHIFT	= 304,
	EVENT_KEY_RCTRL		= 305,
	EVENT_KEY_LCTRL		= 306,
	EVENT_KEY_RALT		= 307,
	EVENT_KEY_LALT		= 308,
	EVENT_KEY_RMETA		= 309,
	EVENT_KEY_LMETA		= 310,
	EVENT_KEY_LSUPER	= 311,		/* Left "Windows" key */
	EVENT_KEY_RSUPER	= 312,		/* Right "Windows" key */
	EVENT_KEY_MODE		= 313,		/* "Alt Gr" key */
	EVENT_KEY_COMPOSE	= 314,		/* Multi-key compose key */

	/* Miscellaneous function keys */
	EVENT_KEY_HELP		= 315,
	EVENT_KEY_PRINT		= 316,
	EVENT_KEY_SYSREQ	= 317,
	EVENT_KEY_BREAK		= 318,
	EVENT_KEY_MENU		= 319,
	EVENT_KEY_POWER		= 320,		/* Power Macintosh power key */
	EVENT_KEY_EURO		= 321,		/* Some european keyboards */
	EVENT_KEY_UNDO		= 322,		/* Atari keyboard has Undo */
	EVENT_LAST

} EventType;

/**
	A handler needs to be constructed as one of these things! The functor
	`handler` will be called when an event of type `type` is recieved during an
	`event_tick()`. `param` will be passed to `handler` when it is called.
*/
typedef struct
{
	EventType type;
	void (*handler)(void* param);
	void* param;
} EventHandler;

/**
	Initializes the event subsystem. No other method must be called until this
	one has.

	@return True if all goes well, false otherwise.
*/
bool event_init();

/**
	Adds a handler to be executed during a tick.

	@param  h  The handler to add.

	@see EventHandler
*/
void event_add_handler(EventHandler h);

/**
	Removes all handlers which have the same functor as the one passed to this
	this function.

	@param handler The functor to search for.
*/
void event_remove_handler_by_functor(void (*handler)(void*));

/**
	Removes all handlers which satisfy `predicate` (`predicate` returns true).

	@param should_remove The predicate which determines if an element is to be
	                     removed.
	@param param         The extra parameter that gets passed to the predicate.
*/
void event_remove_handler_by_predicate(bool (*should_remove)(const EventHandler* e, void* param), void* param);

/**
	Grabs one event off of the event queue and processes it, calling each of
	the registered handlers for that event in turn.
*/
void event_tick();

/**
	Call this function to free up any resources associated with the event module.
*/
void event_shutdown();
