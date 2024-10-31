/*
# Copyright (C) 2024 Subhajit Roy
# This file is part of RTOS Basic Software
#
# RTOS Basic Software is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# RTOS Basic Software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
*/

#include "mapping.h"



uint16_t u16UtilsMap(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min,
        uint16_t out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
int16_t s16UtilsMap(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
uint32_t u32UtilsMap(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min,
        uint32_t out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int32_t int32UtilsMap(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
        int32_t out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint32_t uint32UtilsMap(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min,
        uint32_t out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int32_t s32UtilsMap(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
float f32UtilsMap(float x, float in_min, float in_max, float out_min, float out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double doubleUtilsMap(double x, double in_min, double in_max, double out_min, double out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
