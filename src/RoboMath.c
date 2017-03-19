/*
 * RoboMath.c
 *
 *  Created on: Mar 6, 2017
 *      Author: Kamil
 */

#include "RoboMath.h"

const Vector3 Vector3_Zero __attribute__ ((section(".rodata"))) = (Vector3){ 0.0f, 0.0f, 0.0f };
const Vector3 Vector3_UnitX __attribute__ ((section(".rodata"))) = (Vector3){ 1.0f, 0.0f, 0.0f };
const Vector3 Vector3_UnitY __attribute__ ((section(".rodata"))) = (Vector3){ 0.0f, 1.0f, 0.0f };
const Vector3 Vector3_UnitZ __attribute__ ((section(".rodata"))) = (Vector3){ 0.0f, 0.0f, 1.0f };
const Vector3 Vector3_NegUnitX __attribute__ ((section(".rodata"))) = (Vector3){ -1.0f, 0.0f, 0.0f };
const Vector3 Vector3_NegUnitY __attribute__ ((section(".rodata"))) = (Vector3){ 0.0f, -1.0f, 0.0f };
const Vector3 Vector3_NegUnitZ __attribute__ ((section(".rodata"))) = (Vector3){ 0.0f, 0.0f, -1.0f };
const Vector3 Vector3_One __attribute__ ((section(".rodata"))) = (Vector3){ 1.0f, 1.0f, 1.0f };

const Vector2 Vector2_Zero __attribute__ ((section(".rodata"))) = (Vector2){ 0.0f, 0.0f };
const Vector2 Vector2_UnitX __attribute__ ((section(".rodata"))) = (Vector2){ 1.0f, 0.0f };
const Vector2 Vector2_UnitY __attribute__ ((section(".rodata"))) = (Vector2){ 0.0f, 1.0f };
const Vector2 Vector2_NegUnitX __attribute__ ((section(".rodata"))) = (Vector2){ -1.0f, 0.0f };
const Vector2 Vector2_NegUnitY __attribute__ ((section(".rodata"))) = (Vector2){ 0.0f, -1.0f };
const Vector2 Vector2_One __attribute__ ((section(".rodata"))) = (Vector2){ 1.0f, 1.0f };

