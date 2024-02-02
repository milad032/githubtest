/*
 * Copyright (c) 2015 by Thomas Trojer <thomas@trojer.net>
 * Decawave DW1000 library for arduino.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file DW1000Time.cpp
 * Arduino driver library timestamp wrapper (source file) for the Decawave 
 * DW1000 UWB transceiver IC.
 */

#include "DW1000Time.h"

DW1000Time::DW1000Time() {
	_timestamp = 0;
}

DW1000Time::DW1000Time(byte data[]) {
	setTimestamp(data);
}

DW1000Time::DW1000Time(float timeUs) {
	setTime(timeUs);
}

DW1000Time::DW1000Time(const DW1000Time& copy) {
	setTimestamp(copy);
}

DW1000Time::DW1000Time(long value, float factorUs) {
	setTime(value, factorUs);
}

DW1000Time::~DW1000Time() { }

void DW1000Time::setTime(float timeUs) {
	_timestamp = (long long int)(timeUs*TIME_RES_INV);
}

///	function is to convert a long time value in microseconds to a float time value (microseconds),
///	ensure it stays within a specified range 


void DW1000Time::setTime(long value, float factorUs) {
	float tsValue = value*factorUs;
	tsValue = fmod(tsValue, TIME_OVERFLOW);
	setTime(tsValue);
}

/// By iterating through the data array, the function constructs a 64-bit timestamp by 
//	combining the individual bytes in a manner that respects their positions in the array. This
// timestamp is then stored in the _timestamp member variable of the DW1000Time object.

//	The purpose of this function is to allow you to set the timestamp of a DW1000Time object
//	based on a byte array, which is a common format for timestamps in many communication 
//	protocols and devices.


void DW1000Time::setTimestamp(byte data[]) {
	_timestamp = 0;
	for(int i = 0; i < LEN_STAMP; i++) {
		_timestamp |= ((long long int)data[i] << (i*8));
	}
}

void DW1000Time::setTimestamp(const DW1000Time& copy) {
	_timestamp = copy.getTimestamp();
}

void DW1000Time::setTimestamp(int value) {
	_timestamp = value;
}

long long int DW1000Time::getTimestamp() const {
	return _timestamp;
}

//	the purpose of the wrap() function is to ensure that the stored _timestamp
//	value remains within a valid range, even if it has wrapped around due to overflow.
//  If the _timestamp is negative, this function adds an offset (TIME_OVERFLOW) to bring 
//  back to a valid positive range, preventing issues related to overflow.
//  This is a common operation in systems where time values wrap around after reaching
//	a maximum value, and this function ensures that the timestamp remains accurate and consistent.

DW1000Time& DW1000Time::wrap() {
	if(_timestamp < 0) {
		_timestamp += TIME_OVERFLOW;
	}
	return *this;
}

// the getTimestamp function is used to retrieve the timestamp value stored in a DW1000Time 
// object and represent it as an array of bytes. This can be useful when you need to work with
// the timestamp at a byte level, such as when communicating with external systems or devices 
// that expect timestamp data in byte format.

void DW1000Time::getTimestamp(byte data[]) const {
	memset(data, 0, LEN_STAMP);
	for(int i = 0; i < LEN_STAMP; i++) {
		data[i] = (byte)((_timestamp >> (i*8)) & 0xFF);
	}
}

//	The purpose of this function is to convert a timestamp into a floating-point representation of time,
//  and it seems to involve some form of scaling and adjustment based on the constants TIME_OVERFLOW and TIME_RES.
//  These constants should be defined elsewhere in your code and determine the units and scaling of the final time value.

float DW1000Time::getAsFloat() const {
	return fmod((float)_timestamp, TIME_OVERFLOW)*TIME_RES;
}


// The purpose of this function is to convert a timestamp into a floating-point representation of distance in meters

float DW1000Time::getAsMeters() const {
	return fmod((float)_timestamp, TIME_OVERFLOW)*DISTANCE_OF_RADIO;
}

// this assignment operator is used to copy the timestamp value from one DW1000Time object to another,
// taking care to handle self-assignment efficiently.

DW1000Time& DW1000Time::operator=(const DW1000Time& assign) {
	if(this == &assign) {
		return *this;
	}
	_timestamp = assign.getTimestamp();
	return *this;
}

DW1000Time& DW1000Time::operator+=(const DW1000Time& add) {
	_timestamp += add.getTimestamp();
	return *this;
}

DW1000Time DW1000Time::operator+(const DW1000Time& add) const {
	return DW1000Time(*this) += add;
}

DW1000Time& DW1000Time::operator-=(const DW1000Time& sub) {
	_timestamp -= sub.getTimestamp();
	return *this;
}

DW1000Time DW1000Time::operator-(const DW1000Time& sub) const {
	return DW1000Time(*this) -= sub;
}

DW1000Time& DW1000Time::operator*=(float factor) {
	float tsValue = (float)_timestamp*factor;
	_timestamp = (long long int)tsValue;
	return *this;
}

DW1000Time DW1000Time::operator*(float factor) const {
	return DW1000Time(*this) *= factor;
}

DW1000Time& DW1000Time::operator*=(const DW1000Time& factor) {
	_timestamp *= factor.getTimestamp();
	return *this;
}

DW1000Time DW1000Time::operator*(const DW1000Time& factor) const {
	return DW1000Time(*this) *= factor;
}

DW1000Time& DW1000Time::operator/=(float factor) {
	_timestamp *= (1.0f/factor);
	return *this;
}

DW1000Time DW1000Time::operator/(float factor) const {
	return DW1000Time(*this) /= factor;
}

DW1000Time& DW1000Time::operator/=(const DW1000Time& factor) {
	_timestamp /= factor.getTimestamp();
	return *this;
}

DW1000Time DW1000Time::operator/(const DW1000Time& factor) const {
	return DW1000Time(*this) /= factor;
}

boolean DW1000Time::operator==(const DW1000Time& cmp) const {
	return _timestamp == cmp.getTimestamp();
}

boolean DW1000Time::operator!=(const DW1000Time& cmp) const {
	return !(*this == cmp);
}

//  this method converts a numerical timestamp value into a human-readable 
//  string and sends it to the serial port for debugging or monitoring.

void DW1000Time::print() {
	long long int number = _timestamp;
	unsigned char buf[64];
	uint8_t       i      = 0;
	
	if(number == 0) {
		Serial.print((char)'0');
		return;
	}
	
	// Extract the individual digits of the timestamp by repeatedly dividing it by 10.
	while(number > 0) {
		uint64_t q = number/10;
		buf[i++] = number-q*10;
		number = q;
	}
	// Print the digits from the buffer in reverse order to construct the original number.
	for(; i > 0; i--)
		Serial.print((char)(buf[i-1] < 10 ? '0'+buf[i-1] : 'A'+buf[i-1]-10));
	
	Serial.println();
	
}


