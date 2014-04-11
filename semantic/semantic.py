# Rappel des types struct.pack usuelles :
# B  unsigned char
# H  unsigned short
# I  unsigned int
# b  signed char
# h  signed short
# i  signed int
# f  float

from atp.protos import Packet, Proto

# yymmjjhhmm
version = 1306051704

class Common(Proto):
    def __init__(self):
        print("Common")
        super(Common, self)

    test = Packet(252, "both", [
            ("B", "B"),
            ("H", "H"),
            ("I", "I"),
            ("b", "b"),
            ("h", "h"),
            ("i", "i"),
            ("f", "f")
        ])
    error = Packet(253, "pic")
    getId = Packet(254, "arm")
    id = Packet(255, "pic", [
            ("id", "B")
        ])

#class Demo(Proto):
#    type = 1
#
#    blinkOn = Packet(1, "arm")
#    blinkOff = Packet(2, "arm")
#
#    setDelay = Packet(3, "arm", [
#            ("delay", "H")
#        ])
#
#    ledOn = Packet(4, "arm")
#    ledOff = Packet(5, "arm")
#
#    setCallback = Packet(6, "arm", [
#            ("nbloop", "B")
#        ])
#    callback = Packet(7, "pic")


class Asserv(Proto):
    type = 5

    stop = Packet(1, "arm")
    done = Packet(2, "pic")

    step = Packet(10, "arm", [
        ("period", "I"),
        ("ticsG", "I"),
        ("ticsD", "I"),
        ("consignG", "i"),
        ("consignD", "i")
        ])

    setEpsilons = Packet(11, "arm", [
        ("dist", "f"),
        ("speed", "f"),
        ("theta", "f"),
        ("omega", "f")
        ])

    dist = Packet(12, "arm", [
        ("dist", "f"),
        ("vMax", "f"),
        ("aMax", "f"),
        ])

    rot = Packet(13, "arm", [
        ("rot", "f"),
        ("vMax", "f"),
        ("aMax", "f"),
        ])

    distFree = Packet(14, "arm", [
        ("dist", "f")
        ])

    rotFree = Packet(15, "arm", [
        ("rot", "f")
        ])

    distRot = Packet(16, "arm", [
        ("dist", "f"),
        ("rot", "f"),
        ("vDistMax", "f"),
        ("aDistMax", "f"),
        ("vRotMax", "f"),
        ("aRotMax", "f")
        ])

    reachX = Packet(17, "arm", [
        ("x", "f"),
        ("vMax", "f"),
        ("aMax", "f")
        ])

    reachY = Packet(18, "arm", [
        ("y", "f"),
        ("vMax", "f"),
        ("aMax", "f")
        ])

    reachTheta = Packet(19, "arm", [
        ("theta", "f"),
        ("vMax", "f"),
        ("aMax", "f")
        ])

    speed = Packet(20, "arm", [
        ("speed", "f"),
        ("aMax", "f"),
        ("dMax", "f")
        ])

    speedFree = Packet(21, "arm", [
        ("speed", "f")
        ])

    omega = Packet(22, "arm", [
        ("omega", "f"),
        ("aMax", "f"),
        ("dMax", "f")
        ])

    speedOmega = Packet(23, "arm", [
        ("speed", "f"),
        ("omega", "f"),
        ("aDistMax", "f"),
        ("dDistMax", "f"),
        ("aRotMax", "f"),
        ("dRotMax", "f")
        ])

    getX = Packet(30, "arm")
    setX = Packet(31, "arm", [("x", "f")])
    X = Packet(32, "pic", [("x", "f")])

    getY = Packet(33, "arm")
    setY = Packet(34, "arm", [("y", "f")])
    Y = Packet(35, "pic", [("y", "f")])

    getTheta = Packet(36, "arm")
    setTheta = Packet(37, "arm", [("theta", "f")])
    theta = Packet(38, "pic", [("theta", "f")])

    getPos = Packet(39, "arm")
    pos = Packet(40, "pic", [
        ("x", "f"),
        ("y", "f"),
        ("theta", "f")
        ])

    setTicByMeter = Packet(41, "arm", [("tic_by_meter", "I")])
    setSpacing = Packet(42, "arm", [("spacing", "f")])

    odoBroadcastOn = Packet(43, "arm")
    odoBroadcastOff = Packet(44, "arm")
    odoBroadcastToggle = Packet(45, "arm")
    odoDelay = Packet(46, "arm", [("delay", "I")])

    setXTheta = Packet(47, "arm", [("x", "f"), ("theta", "f")])
    setYTheta = Packet(48, "arm", [("y", "f"), ("theta", "f")])
    setXYTheta = Packet(49, "arm", [("x", "f"), ("y", "f"), ("theta", "f")])
    setXY = Packet(50, "arm", [("x", "f"), ("y", "f")])


class Mother(Proto):
    type = 6

    # Pince pour les verres
    sortirPince = Packet(1, "arm")
    chopperVerre = Packet(2, "arm")
    lacherVerres = Packet(3, "arm")
    getNombreVerres = Packet(4, "arm")
    nombreVerres = Packet(5, "pic", [
            ("n", "H"),
        ])
    pasDeVerreEvent = Packet(6, "pic")

    # Bras pour les bougies
    BougiesHitTop = Packet(10, "arm")
    BougiesHitTopConfirm = Packet(11, "pic")

    BougiesHitBot = Packet(12, "arm")
    BougiesHitBotConfirm = Packet(13, "pic")

    BougiesOff = Packet(14, "arm")
    BougiesOffConfirm = Packet(15, "pic")

    BougiesOn = Packet(16, "arm")
    BougiesOnConfirm = Packet(17, "pic")

    # Arrêt d’urgence
    getEmergencyState = Packet(20, "arm")
    emergencyState = Packet(21, "pic", [
            ("emergency_state", "B"),
        ])

    # Trois switchs
    getSwitchOneState = Packet(30, "arm")
    switchOne = Packet(31, "pic", [
            ("state", "B"),
        ])
    getSwitchTwoState = Packet(32, "arm")
    switchTwo  = Packet(33, "pic", [
            ("state", "B"),
        ])
    getSwitchThreeState = Packet(34, "arm")
    switchThree  = Packet(35, "pic", [
            ("state", "B"),
        ])

    # Start Laisse
    getStartLaisseState = Packet(40, "arm")
    StartLaisseState = Packet(41, "pic", [
            ("state", "B"),
        ])

    # FunnyAction
    FunnyAction = Packet(50, "arm")
    StopFunnyAction = Packet(51, "arm")

    # AX12
    stopAX12 = Packet(60, "arm")
    startAX12 = Packet(61, "arm")
    getAX12Torque = Packet(62, "arm", [
            ("id", "B"),
            ("torque", "i"),
        ])
    AX12Torque = Packet(63, "pic", [
            ("id", "B"),
            ("torque", "i"),
        ])


class Turret(Proto):
    type = 8
    on = Packet(1, "arm")
    off = Packet(2, "arm")
    getPos = Packet(10, "arm", [
        ("id", "B")
    ])
    pos = Packet(11, "pic", [
        ("id", "B"),
        ("distance", "B"),
        ("angle", "B")
    ])


# Rappel des types struct.pack usuelles :
# B  unsigned char
# H  unsigned short
# I  unsigned int
# b  signed char
# h  signed short
# i  signed int
# f  float
