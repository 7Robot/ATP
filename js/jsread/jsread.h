char *axis_names[ABS_MAX + 1] = { 
    "X", "Y", "Z", "Rx", "Ry", "Rz", "Throttle", "Rudder", 
    "Wheel", "Gas", "Brake", "?", "?", "?", "?", "?",
    "Hat0X", "Hat0Y", "Hat1X", "Hat1Y", "Hat2X", "Hat2Y", "Hat3X", "Hat3Y",
    "?", "?", "?", "?", "?", "?", "?", 
};

char *button_names[KEY_MAX - BTN_MISC + 1] = {
    "Btn0", "Btn1", "Btn2", "Btn3", "Btn4", "Btn5", "Btn6", "Btn7", "Btn8",
    "Btn9", "?", "?", "?", "?", "?", "?", "LeftBtn", "RightBtn", "MiddleBtn",
    "SideBtn", "ExtraBtn", "ForwardBtn", "BackBtn", "TaskBtn", "?", "?", "?",
    "?", "?", "?", "?", "?", "Trigger", "ThumbBtn", "ThumbBtn2", "TopBtn",
    "TopBtn2", "PinkieBtn", "BaseBtn", "BaseBtn2", "BaseBtn3", "BaseBtn4",
    "BaseBtn5", "BaseBtn6", "BtnDead", "BtnA", "BtnB", "BtnC", "BtnX", "BtnY",
    "BtnZ", "BtnTL", "BtnTR", "BtnTL2", "BtnTR2", "BtnSelect", "BtnStart",
    "BtnMode", "BtnThumbL", "BtnThumbR", "?", "?", "?", "?", "?", "?", "?",
    "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "WheelBtn", "Gear up",
};

int jsread(char * device, int(*callback)(int,int*,int,char*), int verbose);
