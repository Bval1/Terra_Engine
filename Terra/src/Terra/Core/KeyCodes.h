#pragma once


/* Function keys */
#define TERRA_KEY_BACKSPACE          0x08
#define TERRA_KEY_TAB                0x09
#define TERRA_KEY_ENTER              0x0D
#define TERRA_KEY_LEFT_ALT           0x12

#define TERRA_KEY_PAUSE              0x13
#define TERRA_KEY_CAPS_LOCK          0x14
#define TERRA_KEY_ESCAPE             0x1B
#define TERRA_KEY_SPACE              0x20
#define TERRA_KEY_PAGE_UP            0x21
#define TERRA_KEY_PAGE_DOWN          0x22
#define TERRA_KEY_END                0x23
#define TERRA_KEY_HOME               0x24
#define TERRA_KEY_LEFT               0x25
#define TERRA_KEY_UP                 0x26
#define TERRA_KEY_RIGHT              0x27
#define TERRA_KEY_DOWN               0x28
#define TERRA_KEY_PRINT_SCREEN       0x2C
#define TERRA_KEY_INSERT             0x2D
#define TERRA_KEY_DELETE             0x2E

#define TERRA_KEY_0                  0x30
#define TERRA_KEY_1                  0x31
#define TERRA_KEY_2                  0x32
#define TERRA_KEY_3                  0x33
#define TERRA_KEY_4                  0x34
#define TERRA_KEY_5                  0x35
#define TERRA_KEY_6                  0x36
#define TERRA_KEY_7                  0x37
#define TERRA_KEY_8                  0x38
#define TERRA_KEY_9                  0x39
#define TERRA_KEY_A                  0x41
#define TERRA_KEY_B                  0x42
#define TERRA_KEY_C                  0x43
#define TERRA_KEY_D                  0x44
#define TERRA_KEY_E                  0x45
#define TERRA_KEY_F                  0x46
#define TERRA_KEY_G                  0x47
#define TERRA_KEY_H                  0x48
#define TERRA_KEY_I                  0x49
#define TERRA_KEY_J                  0x4A
#define TERRA_KEY_K                  0x4B
#define TERRA_KEY_L                  0x4C
#define TERRA_KEY_M                  0x4D
#define TERRA_KEY_N                  0x4E
#define TERRA_KEY_O                  0x4F
#define TERRA_KEY_P                  0x50
#define TERRA_KEY_Q                  0x51
#define TERRA_KEY_R                  0x52
#define TERRA_KEY_S                  0x53
#define TERRA_KEY_T                  0x54
#define TERRA_KEY_U                  0x55
#define TERRA_KEY_V                  0x56
#define TERRA_KEY_W                  0x57
#define TERRA_KEY_X                  0x58
#define TERRA_KEY_Y                  0x59
#define TERRA_KEY_Z                  0x5A
#define TERRA_LWIN_KEY				  0x5C
#define TERRA_RWIN_KEY				  0x5B
#define TERRA_KEY_WORLD_1            161 /* non-US #1 */
#define TERRA_KEY_WORLD_2            162 /* non-US #2 */

#define TERRA_KEY_KP_0               0x60
#define TERRA_KEY_KP_1               0x61
#define TERRA_KEY_KP_2               0x62
#define TERRA_KEY_KP_3               0x63
#define TERRA_KEY_KP_4               0x64
#define TERRA_KEY_KP_5               0x65
#define TERRA_KEY_KP_6               0x66
#define TERRA_KEY_KP_7               0x67
#define TERRA_KEY_KP_8               0x68
#define TERRA_KEY_KP_9               0x69
#define TERRA_KEY_KP_MULTIPLY        0x6A
#define TERRA_KEY_KP_ADD             0x6B
#define TERRA_KEY_KP_SUBTRACT        0x6D
#define TERRA_KEY_KP_DECIMAL         0x6E
#define TERRA_KEY_KP_DIVIDE          0x6F
#define TERRA_KEY_F1                 0x70
#define TERRA_KEY_F2                 0x71
#define TERRA_KEY_F3                 0x72
#define TERRA_KEY_F4                 0x73
#define TERRA_KEY_F5                 0x74
#define TERRA_KEY_F6                 0x75
#define TERRA_KEY_F7                 0x76
#define TERRA_KEY_F8                 0x77
#define TERRA_KEY_F9                 0x78
#define TERRA_KEY_F10                0x79
#define TERRA_KEY_F11                0x7A
#define TERRA_KEY_F12                0x7B
#define TERRA_KEY_F13                0x7C
#define TERRA_KEY_F14                0x7D
#define TERRA_KEY_F15                0x7E
#define TERRA_KEY_F16                0x7F
#define TERRA_KEY_F17                0x80
#define TERRA_KEY_F18                0x81
#define TERRA_KEY_F19                0x82
#define TERRA_KEY_F20                0x83
#define TERRA_KEY_F21                0x84
#define TERRA_KEY_F22                0x85
#define TERRA_KEY_F23                0x86
#define TERRA_KEY_F24                0x87
#define TERRA_KEY_F25                0x88

#define TERRA_KEY_NUM_LOCK           0x90
#define TERRA_KEY_SCROLL_LOCK        0x91
#define TERRA_KEY_LEFT_SHIFT         0xA0
#define TERRA_KEY_RIGHT_SHIFT        0xA1
#define TERRA_KEY_LEFT_CONTROL       0xA2
#define TERRA_KEY_RIGHT_CONTROL      0xA3
#define TERRA_KEY_LEFT_SUPER         TERRA_LWIN_KEY
#define TERRA_KEY_RIGHT_SUPER        TERRA_RWIN_KEY
#define TERRA_KEY_MENU               0xA4 	// VK_LMENU
#define TERRA_KEY_RIGHT_ALT          0xA5
#define TERRA_VOLUME_MUTE			  0xAD
#define TERRA_VOLUME_DOWN			  0xAE
#define TERRA_VOLUME_UP			  0xAF
#define TERRA_MEDIA_NEXT_TRACK		  0xB0
#define TERRA_MEDIA_PREV_TRACK		  0xB1
#define TERRA_MEDIA_PLAY_PAUSE		  0xB3
#define TERRA_KEY_SEMICOLON          0xBA  /* ; */
#define TERRA_KEY_EQUAL              0xBB  /* +/= */
#define TERRA_KEY_COMMA              0xBC  /* , */
#define TERRA_KEY_MINUS              0xBD  /* - */
#define TERRA_KEY_PERIOD             0xBE  /* . */
#define TERRA_KEY_SLASH              0xBF  /* /  For US standard keyboard*/
#define TERRA_KEY_GRAVE_ACCENT       0xC0  /* ` */
#define TERRA_KEY_LEFT_BRACKET       0xDB  /* [ */
#define TERRA_KEY_BACKSLASH          0xDC  /* \ */
#define TERRA_KEY_RIGHT_BRACKET      0xDD  /* ] */
#define TERRA_KEY_APOSTROPHE         0xDE  /* ' (Single quote/double quote key) */
#define TERRA_KEY_KP_ENTER           TERRA_KEY_ENTER
#define TERRA_KEY_KP_EQUAL           TERRA_KEY_ENTER

// Mouse codes
#define TERRA_MOUSE_BUTTON_1         0x01
#define TERRA_MOUSE_BUTTON_2         0x02
#define TERRA_MOUSE_BUTTON_4         0x03		// VK_CANCEL
#define TERRA_MOUSE_BUTTON_3         0x04
#define TERRA_MOUSE_BUTTON_5         0x07		// Undefined
#define TERRA_MOUSE_BUTTON_6         0x07
#define TERRA_MOUSE_BUTTON_7         0x05		// X1
#define TERRA_MOUSE_BUTTON_8         0x06		// X2 
#define TERRA_MOUSE_BUTTON_LEFT      TERRA_MOUSE_BUTTON_1
#define TERRA_MOUSE_BUTTON_RIGHT     TERRA_MOUSE_BUTTON_2
#define TERRA_MOUSE_BUTTON_MIDDLE    TERRA_MOUSE_BUTTON_3
#define TERRA_MOUSE_BUTTON_NEXT		 TERRA_MOUSE_BUTTON_7
#define TERRA_MOUSE_BUTTON_LAST      TERRA_MOUSE_BUTTON_8
