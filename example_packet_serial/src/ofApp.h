//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include "ofMain.h"
#include "ofxSerial.h"


struct SerialMessage
{
    std::string message;
    std::string exception;
    int fade = 0;
};


class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void exit();

    void onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args);
    void onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args);

    ofx::IO::PacketSerialDevice device;

    std::vector<SerialMessage> serialMessages;

};
