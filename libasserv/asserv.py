# -*- coding: utf-8 -*-

import ctypes

DLL_PATH = "libasserv.h"

class Asserv:

    def __init__(self, _tic_by_meter, _spacing, _callback, **kwargs):

        dll_path = DLL_PATH

        for arg in kwargs:
            if arg == "lib":
                dll_path = kwargs[arg]
            else:
                print("Warning: unknow arg '%s'" %arg)

        self._lib = ctypes.CDLL(dll_path)

        self._lib.odo_get_x.restype = ctypes.c_float
        self._lib.odo_get_y.restype = ctypes.c_float
        self._lib.odo_get_theta.restype = ctypes.c_float

        CBFUNC = ctypes.CFUNCTYPE(None)
        self._callback = CBFUNC(_callback)

        self._lib.motion_init(self._callback)
        self.set_tic_by_meter(_tic_by_meter)
        self.set_spacing(_spacing)

    # Motion
    def step(self, _period, (_ticsG, _ticsD)):
        period = ctypes.c_float(_period)
        ticsG = ctypes.c_int(_ticsG)
        ticsD = ctypes.c_int(_ticsD)
        consignG = ctypes.c_int(0)
        consignD = ctypes.c_int(0)
        self._lib.motion_step(period, ticsG, ticsD,
                ctypes.byref(consignG), ctypes.byref(consignD))
        return (consignG.value, consignD.value)

    def set_epsilons(self, _dist, _speed, _theta, _omega):
        dist = ctypes.c_float(_dist)
        speed = ctypes.c_float(_speed)
        theta = ctypes.c_float(_theta)
        omega = ctypes.c_float(_omega)
        self._lib.motion_set_epsilons(dist, speed, theta, omega)

    # Motion order
    def stop(self):
        self._lib.motion_stop()
    def dist(self, _dist, _vMax, _aMax):
        dist = ctypes.c_float(_dist)
        vMax = ctypes.c_float(_vMax)
        aMax = ctypes.c_float(_aMax)
        self._lib.motion_dist(dist, vMax, aMax)
    def rot(self, _rot, _vMax, _aMax):
        rot = ctypes.c_float(_rot)
        vMax = ctypes.c_float(_vMax)
        aMax = ctypes.c_float(_aMax)
        self._lib.motion_rot(rot, vMax, aMax)
    def dist_free(self, _dist):
        dist = ctypes.c_float(_dist)
        self._lib.motion_dist_free(dist)
    def rot_free(self, _rot):
        rot = ctypes.c_float(_rot)
        self._lib.motion_rot_free(rot)
    def dist_rot(self, _dist, _rot, _vDistMax, _aDistMax, _vRotMax, _aRotMax):
        dist = ctypes.c_float(_dist)
        rot = ctypes.c_float(_rot)
        vDistMax = ctypes.c_float(_vDistMax)
        aDistMax = ctypes.c_float(_aDistMax)
        vRotMax = ctypes.c_float(_vRotMax)
        aRotMax = ctypes.c_float(_aRotMax)
        self._lib.motion_dist_rot(dist, rot, vDistMax, aDistMax, vRotMax,
                aRotMax)
    def reach_x(self, _x, _vMax, _aMax):
        x = ctypes.c_float(_x)
        vMax = ctypes.c_float(_vMax)
        aMax = ctypes.c_float(_aMax)
        self._lib.motion_reach_x(x, vMax, aMax)
    def reach_y(self, _y, _vMax, _aMax):
        y = ctypes.c_float(_y)
        vMax = ctypes.c_float(_vMax)
        aMax = ctypes.c_float(_aMax)
        self._lib.motion_reach_y(y, vMax, aMax)
    def reach_theta(self, _theta, _vMax, _aMax):
        theta = ctypes.c_float(_theta)
        vMax = ctypes.c_float(_vMax)
        aMax = ctypes.c_float(_aMax)
        self._lib.motion_reach_theta(theta, vMax, aMax)
    def speed(self, _speed, _aMax, _dMax):
        speed = ctypes.c_float(_speed)
        aMax = ctypes.c_float(_aMax)
        dMax = ctypes.c_float(_dMax)
        self._lib.motion_speed(speed, aMax, dMax)
    def speed_free(self, _speed):
        speed = ctypes.c_float(_speed)
        self._lib.motion_speed_free(speed)
    def omega(self, _omega, _aMax, _dMax):
        omega = ctypes.c_float(_omega)
        aMax = ctypes.c_float(_aMax)
        dMax = ctypes.c_float(_dMax)
        self._lib.motion_omega(omega, aMax, dMax)
    def omega_free(self, _omega):
        omega = ctypes.c_float(_omega)
        self._lib.motion_omega_free(omega)
    def speed_omega(self, _speed, _omega,
            _aDistMax, _dDistMax, _aRotMax, _dRotMax):
        speed = ctypes.c_float(_speed)
        omega = ctypes.c_float(_omega)
        aDistMax = ctypes.c_float(_aDistMax)
        dDistMax = ctypes.c_float(_dDistMax)
        aRotMax = ctypes.c_float(_aRotMax)
        dRotMax = ctypes.c_float(_dRotMax)
        self._lib.motion_speed_omega(speed, omega,
                aDistMax, dDistMax, aRotMax, dRotMax)

    # Asserv
    def set_delta_coefs(self, _p, _i, _d):
        p = ctypes.c_float(_p)
        i = ctypes.c_float(_i)
        d = ctypes.c_float(_d)
        #self._lib.asserv_set_delta_coefs(p, i, d)
    def set_alpha_coefs(self, _p, _i, _d):
        p = ctypes.c_float(_p)
        i = ctypes.c_float(_i)
        d = ctypes.c_float(_d)
        #self._lib.asserv_set_alpha_coefs(p, i, d)

    # Odo
    def get_x(self):
        return self._lib.odo_get_x()
    def get_y(self):
        return self._lib.odo_get_y()
    def get_theta(self):
        return self._lib.odo_get_theta()
    def get_pos(self):
        return (self.get_x(), self.get_y(), self.get_theta())

    def set_x(self, _x):
        x = ctypes.c_float(_x)
        self._lib.odo_set_x(x)
    def set_y(self, _y):
        y = ctypes.c_float(_y)
        self._lib.odo_set_y(y)
    def set_theta(self, _theta):
        theta = ctypes.c_float(_theta)
        self._lib.odo_set_theta(theta)

    def set_tic_by_meter(self, _tic_by_meter):
        tic_by_meter = ctypes.c_int(_tic_by_meter)
        self._lib.odo_set_tic_by_meter(tic_by_meter)
    def set_spacing(self, _spacing):
        spacing = ctypes.c_float(_spacing)
        self._lib.odo_set_spacing(spacing)

