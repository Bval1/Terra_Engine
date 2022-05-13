#pragma once


/* Function keys */
#define SB_KEY_BACKSPACE          0x08
#define SB_KEY_TAB                0x09
#define SB_KEY_ENTER              0x0D
#define SB_KEY_LEFT_ALT           0x12

#define SB_KEY_PAUSE              0x13
#define SB_KEY_CAPS_LOCK          0x14
#define SB_KEY_ESCAPE             0x1B
#define SB_KEY_SPACE              0x20
#define SB_KEY_PAGE_UP            0x21
#define SB_KEY_PAGE_DOWN          0x22
#define SB_KEY_END                0x23
#define SB_KEY_HOME               0x24
#define SB_KEY_LEFT               0x25
#define SB_KEY_UP                 0x26
#define SB_KEY_RIGHT              0x27
#define SB_KEY_DOWN               0x28
#define SB_KEY_PRINT_SCREEN       0x2C
#define SB_KEY_INSERT             0x2D
#define SB_KEY_DELETE             0x2E

#define SB_KEY_0                  0x30
#define SB_KEY_1                  0x31
#define SB_KEY_2                  0x32
#define SB_KEY_3                  0x33
#define SB_KEY_4                  0x34
#define SB_KEY_5                  0x35
#define SB_KEY_6                  0x36
#define SB_KEY_7                  0x37
#define SB_KEY_8                  0x38
#define SB_KEY_9                  0x39
#define SB_KEY_A                  0x41
#define SB_KEY_B                  0x42
#define SB_KEY_C                  0x43
#define SB_KEY_D                  0x44
#define SB_KEY_E                  0x45
#define SB_KEY_F                  0x46
#define SB_KEY_G                  0x47
#define SB_KEY_H                  0x48
#define SB_KEY_I                  0x49
#define SB_KEY_J                  0x4A
#define SB_KEY_K                  0x4B
#define SB_KEY_L                  0x4C
#define SB_KEY_M                  0x4D
#define SB_KEY_N                  0x4E
#define SB_KEY_O                  0x4F
#define SB_KEY_P                  0x50
#define SB_KEY_Q                  0x51
#define SB_KEY_R                  0x52
#define SB_KEY_S                  0x53
#define SB_KEY_T                  0x54
#define SB_KEY_U                  0x55
#define SB_KEY_V                  0x56
#define SB_KEY_W                  0x57
#define SB_KEY_X                  0x58
#define SB_KEY_Y                  0x59
#define SB_KEY_Z                  0x5A
#define SB_LWIN_KEY				  0x5C
#define SB_RWIN_KEY				  0x5B
#define SB_KEY_WORLD_1            161 /* non-US #1 */
#define SB_KEY_WORLD_2            162 /* non-US #2 */

#define SB_KEY_KP_0               0x60
#define SB_KEY_KP_1               0x61
#define SB_KEY_KP_2               0x62
#define SB_KEY_KP_3               0x63
#define SB_KEY_KP_4               0x64
#define SB_KEY_KP_5               0x65
#define SB_KEY_KP_6               0x66
#define SB_KEY_KP_7               0x67
#define SB_KEY_KP_8               0x68
#define SB_KEY_KP_9               0x69
#define SB_KEY_KP_MULTIPLY        0x6A
#define SB_KEY_KP_ADD             0x6B
#define SB_KEY_KP_SUBTRACT        0x6D
#define SB_KEY_KP_DECIMAL         0x6E
#define SB_KEY_KP_DIVIDE          0x6F
#define SB_KEY_F1                 0x70
#define SB_KEY_F2                 0x71
#define SB_KEY_F3                 0x72
#define SB_KEY_F4                 0x73
#define SB_KEY_F5                 0x74
#define SB_KEY_F6                 0x75
#define SB_KEY_F7                 0x76
#define SB_KEY_F8                 0x77
#define SB_KEY_F9                 0x78
#define SB_KEY_F10                0x79
#define SB_KEY_F11                0x7A
#define SB_KEY_F12                0x7B
#define SB_KEY_F13                0x7C
#define SB_KEY_F14                0x7D
#define SB_KEY_F15                0x7E
#define SB_KEY_F16                0x7F
#define SB_KEY_F17                0x80
#define SB_KEY_F18                0x81
#define SB_KEY_F19                0x82
#define SB_KEY_F20                0x83
#define SB_KEY_F21                0x84
#define SB_KEY_F22                0x85
#define SB_KEY_F23                0x86
#define SB_KEY_F24                0x87
#define SB_KEY_F25                0x88

#define SB_KEY_NUM_LOCK           0x90
#define SB_KEY_SCROLL_LOCK        0x91
#define SB_KEY_LEFT_SHIFT         0xA0
#define SB_KEY_RIGHT_SHIFT        0xA1
#define SB_KEY_LEFT_CONTROL       0xA2
#define SB_KEY_RIGHT_CONTROL      0xA3
#define SB_KEY_LEFT_SUPER         SB_LWIN_KEY
#define SB_KEY_RIGHT_SUPER        SB_RWIN_KEY
#define SB_KEY_MENU               0xA4 	// VK_LMENU
#define SB_KEY_RIGHT_ALT          0xA5
#define SB_VOLUME_MUTE			  0xAD
#define SB_VOLUME_DOWN			  0xAE
#define SB_VOLUME_UP			  0xAF
#define SB_MEDIA_NEXT_TRACK		  0xB0
#define SB_MEDIA_PREV_TRACK		  0xB1
#define SB_MEDIA_PLAY_PAUSE		  0xB3
#define SB_KEY_SEMICOLON          0xBA  /* ; */
#define SB_KEY_EQUAL              0xBB  /* +/= */
#define SB_KEY_COMMA              0xBC  /* , */
#define SB_KEY_MINUS              0xBD  /* - */
#define SB_KEY_PERIOD             0xBE  /* . */
#define SB_KEY_SLASH              0xBF  /* /  For US standard keyboard*/
#define SB_KEY_GRAVE_ACCENT       0xC0  /* ` */
#define SB_KEY_LEFT_BRACKET       0xDB  /* [ */
#define SB_KEY_BACKSLASH          0xDC  /* \ */
#define SB_KEY_RIGHT_BRACKET      0xDD  /* ] */
#define SB_KEY_APOSTROPHE         0xDE  /* ' (Single quote/double quote key) */
#define SB_KEY_KP_ENTER           SB_KEY_ENTER
#define SB_KEY_KP_EQUAL           SB_KEY_ENTER

// Mouse codes
#define SB_MOUSE_BUTTON_1         0x01
#define SB_MOUSE_BUTTON_2         0x02
#define SB_MOUSE_BUTTON_4         0x03		// VK_CANCEL
#define SB_MOUSE_BUTTON_3         0x04
#define SB_MOUSE_BUTTON_5         0x07		// Undefined
#define SB_MOUSE_BUTTON_6         0x07
#define SB_MOUSE_BUTTON_7         0x05		// X1
#define SB_MOUSE_BUTTON_8         0x06		// X2 
#define SB_MOUSE_BUTTON_LEFT      SB_MOUSE_BUTTON_1
#define SB_MOUSE_BUTTON_RIGHT     SB_MOUSE_BUTTON_2
#define SB_MOUSE_BUTTON_MIDDLE    SB_MOUSE_BUTTON_3
#define SB_MOUSE_BUTTON_NEXT	  SB_MOUSE_BUTTON_7
#define SB_MOUSE_BUTTON_LAST      SB_MOUSE_BUTTON_8
