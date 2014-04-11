// Generated from version 1306051704 of semantic

#include "atp.h"


void SendError() {
    char bytes[] = {
        129,
        253,
        128
    };
    SendBytes(bytes, 3);
}

// You should redefine this function
__attribute__((weak)) void OnGetId() { SendBoardId(); }

void SendId(unsigned char id) {
    char bytes[] = {
        129,
        255,
        1,
        id,
        128
    };
    SendBytes(bytes, 5);
}

// You should redefine this function
__attribute__((weak)) void OnTest(unsigned char B, unsigned int H, unsigned long int I, char b, int h, long int i, float f) { SendTest(B, H, I, b, h, i, f); }

void SendTest(unsigned char B, unsigned int H, unsigned long int I, char b, int h, long int i, float f) {
    char bytes[] = {
        129,
        252,
        1,
        B,
        2,
        ((char*)&H)[0],
        ((char*)&H)[1],
        4,
        ((char*)&I)[0],
        ((char*)&I)[1],
        ((char*)&I)[2],
        ((char*)&I)[3],
        17,
        b,
        18,
        ((char*)&h)[0],
        ((char*)&h)[1],
        20,
        ((char*)&i)[0],
        ((char*)&i)[1],
        ((char*)&i)[2],
        ((char*)&i)[3],
        36,
        ((char*)&f)[0],
        ((char*)&f)[1],
        ((char*)&f)[2],
        ((char*)&f)[3],
        128
    };
    SendBytes(bytes, 28);
}

void SendX(float x) {
    char bytes[] = {
        129,
        32,
        36,
        ((char*)&x)[0],
        ((char*)&x)[1],
        ((char*)&x)[2],
        ((char*)&x)[3],
        128
    };
    SendBytes(bytes, 8);
}

void SendY(float y) {
    char bytes[] = {
        129,
        35,
        36,
        ((char*)&y)[0],
        ((char*)&y)[1],
        ((char*)&y)[2],
        ((char*)&y)[3],
        128
    };
    SendBytes(bytes, 8);
}

// You should redefine this function
__attribute__((weak)) void OnDist(float dist, float vMax, float aMax) {}

// You should redefine this function
__attribute__((weak)) void OnDistFree(float dist) {}

// You should redefine this function
__attribute__((weak)) void OnDistRot(float dist, float rot, float vDistMax, float aDistMax, float vRotMax, float aRotMax) {}

void SendDone() {
    char bytes[] = {
        129,
        2,
        128
    };
    SendBytes(bytes, 3);
}

// You should redefine this function
__attribute__((weak)) void OnGetPos() {}

// You should redefine this function
__attribute__((weak)) void OnGetTheta() {}

// You should redefine this function
__attribute__((weak)) void OnGetX() {}

// You should redefine this function
__attribute__((weak)) void OnGetY() {}

// You should redefine this function
__attribute__((weak)) void OnOdoBroadcastOff() {}

// You should redefine this function
__attribute__((weak)) void OnOdoBroadcastOn() {}

// You should redefine this function
__attribute__((weak)) void OnOdoBroadcastToggle() {}

// You should redefine this function
__attribute__((weak)) void OnOdoDelay(unsigned long int delay) {}

// You should redefine this function
__attribute__((weak)) void OnOmega(float omega, float aMax, float dMax) {}

void SendPos(float x, float y, float theta) {
    char bytes[] = {
        129,
        40,
        36,
        ((char*)&x)[0],
        ((char*)&x)[1],
        ((char*)&x)[2],
        ((char*)&x)[3],
        36,
        ((char*)&y)[0],
        ((char*)&y)[1],
        ((char*)&y)[2],
        ((char*)&y)[3],
        36,
        ((char*)&theta)[0],
        ((char*)&theta)[1],
        ((char*)&theta)[2],
        ((char*)&theta)[3],
        128
    };
    SendBytes(bytes, 18);
}

// You should redefine this function
__attribute__((weak)) void OnReachTheta(float theta, float vMax, float aMax) {}

// You should redefine this function
__attribute__((weak)) void OnReachX(float x, float vMax, float aMax) {}

// You should redefine this function
__attribute__((weak)) void OnReachY(float y, float vMax, float aMax) {}

// You should redefine this function
__attribute__((weak)) void OnRot(float rot, float vMax, float aMax) {}

// You should redefine this function
__attribute__((weak)) void OnRotFree(float rot) {}

// You should redefine this function
__attribute__((weak)) void OnSetEpsilons(float dist, float speed, float theta, float omega) {}

// You should redefine this function
__attribute__((weak)) void OnSetSpacing(float spacing) {}

// You should redefine this function
__attribute__((weak)) void OnSetTheta(float theta) {}

// You should redefine this function
__attribute__((weak)) void OnSetTicByMeter(unsigned long int tic_by_meter) {}

// You should redefine this function
__attribute__((weak)) void OnSetX(float x) {}

// You should redefine this function
__attribute__((weak)) void OnSetXTheta(float x, float theta) {}

// You should redefine this function
__attribute__((weak)) void OnSetXYTheta(float x, float y, float theta) {}

// You should redefine this function
__attribute__((weak)) void OnSetY(float y) {}

// You should redefine this function
__attribute__((weak)) void OnSetYTheta(float y, float theta) {}

// You should redefine this function
__attribute__((weak)) void OnSpeed(float speed, float aMax, float dMax) {}

// You should redefine this function
__attribute__((weak)) void OnSpeedFree(float speed) {}

// You should redefine this function
__attribute__((weak)) void OnSpeedOmega(float speed, float omega, float aDistMax, float dDistMax, float aRotMax, float dRotMax) {}

// You should redefine this function
__attribute__((weak)) void OnStep(unsigned long int period, unsigned long int ticsG, unsigned long int ticsD, long int consignG, long int consignD) {}

// You should redefine this function
__attribute__((weak)) void OnStop() {}

void SendTheta(float theta) {
    char bytes[] = {
        129,
        38,
        36,
        ((char*)&theta)[0],
        ((char*)&theta)[1],
        ((char*)&theta)[2],
        ((char*)&theta)[3],
        128
    };
    SendBytes(bytes, 8);
}


int AtpDecode(int id,
        unsigned char *ucharv, int ucharc,
        unsigned int *ushortv, int ushortc,
        unsigned long int *uintv, int uintc,
        char *charv, int charc,
        int *shortv, int shortc,
        long int *intv, int intc,
        float *floatv, int floatc)
{
    if (id == 254) {
        OnGetId();
        return 1;
    }
    if (id == 252) {
        OnTest(ucharv[0], ushortv[0], uintv[0], charv[0], shortv[0], intv[0], floatv[0]);
        return 1;
    }
    if (id == 12) {
        OnDist(floatv[0], floatv[1], floatv[2]);
        return 1;
    }
    if (id == 14) {
        OnDistFree(floatv[0]);
        return 1;
    }
    if (id == 16) {
        OnDistRot(floatv[0], floatv[1], floatv[2], floatv[3], floatv[4], floatv[5]);
        return 1;
    }
    if (id == 39) {
        OnGetPos();
        return 1;
    }
    if (id == 36) {
        OnGetTheta();
        return 1;
    }
    if (id == 30) {
        OnGetX();
        return 1;
    }
    if (id == 33) {
        OnGetY();
        return 1;
    }
    if (id == 44) {
        OnOdoBroadcastOff();
        return 1;
    }
    if (id == 43) {
        OnOdoBroadcastOn();
        return 1;
    }
    if (id == 45) {
        OnOdoBroadcastToggle();
        return 1;
    }
    if (id == 46) {
        OnOdoDelay(uintv[0]);
        return 1;
    }
    if (id == 22) {
        OnOmega(floatv[0], floatv[1], floatv[2]);
        return 1;
    }
    if (id == 19) {
        OnReachTheta(floatv[0], floatv[1], floatv[2]);
        return 1;
    }
    if (id == 17) {
        OnReachX(floatv[0], floatv[1], floatv[2]);
        return 1;
    }
    if (id == 18) {
        OnReachY(floatv[0], floatv[1], floatv[2]);
        return 1;
    }
    if (id == 13) {
        OnRot(floatv[0], floatv[1], floatv[2]);
        return 1;
    }
    if (id == 15) {
        OnRotFree(floatv[0]);
        return 1;
    }
    if (id == 11) {
        OnSetEpsilons(floatv[0], floatv[1], floatv[2], floatv[3]);
        return 1;
    }
    if (id == 42) {
        OnSetSpacing(floatv[0]);
        return 1;
    }
    if (id == 37) {
        OnSetTheta(floatv[0]);
        return 1;
    }
    if (id == 41) {
        OnSetTicByMeter(uintv[0]);
        return 1;
    }
    if (id == 31) {
        OnSetX(floatv[0]);
        return 1;
    }
    if (id == 47) {
        OnSetXTheta(floatv[0], floatv[1]);
        return 1;
    }
    if (id == 49) {
        OnSetXYTheta(floatv[0], floatv[1], floatv[2]);
        return 1;
    }
    if (id == 34) {
        OnSetY(floatv[0]);
        return 1;
    }
    if (id == 48) {
        OnSetYTheta(floatv[0], floatv[1]);
        return 1;
    }
    if (id == 20) {
        OnSpeed(floatv[0], floatv[1], floatv[2]);
        return 1;
    }
    if (id == 21) {
        OnSpeedFree(floatv[0]);
        return 1;
    }
    if (id == 23) {
        OnSpeedOmega(floatv[0], floatv[1], floatv[2], floatv[3], floatv[4], floatv[5]);
        return 1;
    }
    if (id == 10) {
        OnStep(uintv[0], uintv[1], uintv[2], intv[0], intv[1]);
        return 1;
    }
    if (id == 1) {
        OnStop();
        return 1;
    }
    return 0;
}

