#define WM_UI_FIRE				(WM_APP+1) 
#define WM_D2D_NCLBUTTONDOWN	(WM_APP+2)
#define WM_D2D_UNACTIVE			(WM_APP+4)

#define WM_D2D_BUTTON_CLICK						(WM_APP+8)
#define WM_D2D_TEXTBOX_SETTEXT					(WM_APP+9)
#define WM_D2D_TEXTBOX_GETTEXT					(WM_APP+10)

#define WM_D2D_MENU_SELECT						(WM_APP+11)
#define WM_D2D_PAINT							(WM_APP+12) // WM_D2D_PAINT_CAPTURE
#define WM_D2D_GET_CONTROLS						(WM_APP+13)
#define WM_D2D_CREATE_CONTROLS					(WM_APP+14)

#define WM_D2D_TEST								(WM_APP+15)

#define WM_D2D_IDLE								(WM_APP+16)
#define WM_D2D_DROPOBJECT_SEARCH_ACCEPTOR		(WM_APP+17)
#define WM_D2D_DROPOBJECT_FOUND_ACCEPTOR		(WM_APP+18)
#define WM_D2D_OBJECT_UPDATE					(WM_APP+19)
#define WM_D2D_OBJECT_SUICIDE					(WM_APP+20)

#define WM_D2D_MESSAGEBOX_CLOSED				(WM_APP+21)
#define WM_D2D_INTERNET_GET_COMPLETED			(WM_APP+22)

#define WM_D2D_TAB_ACTIVE						(WM_APP+23)
#define WM_D2D_COMMAND							(WM_APP+24) // WM_COMMAND wparam, lparam. Lparam is security id.

#define WM_D2D_SETCAPTURE						(WM_APP+25)

#define WM_D2D_GRIDCONTROL						(WM_APP+26) // wparam:0 is add msg.

#define WM_D2D_RESTRUCT_RENDERTARGET			(WM_APP+29)
#define WM_D2D_CLOSE_CONTROL					(WM_APP+30)	// wp:none, lp:none
#define WM_D2D_DESTROY_CONTROL					(WM_APP+31)	// wp:none, lp:D2DControl* target

#define WM_D2D_EV_FIRST							(WM_APP+100)

#define WM_D2D_EVTEXTBOX_SINGLELINE_PUSHKEY		(WM_APP+101)
#define WM_D2D_EVTEXTBOX_CHAR					(WM_APP+102)
#define WM_D2D_EVSELECT_CHEANGED				(WM_APP+103)
#define WM_D2D_EVSLIDER_CHEANGED				(WM_APP+104)

#define WM_D2D_JS_FIRST							(WM_APP+500)
#define WM_D2D_JS_SETTEXT						WM_D2D_JS_FIRST
#define WM_D2D_JS_ENABLE						(WM_APP+501)
#define WM_D2D_JS_ERROR							(WM_APP+502)



// WM_MOUSEMOVE : wParam
#define MK_F_MOUSE_ENTER						0x100
#define MK_F_MOUSE_LEAVE						0x200