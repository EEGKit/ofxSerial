//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // 1. Upload the PacketSerialReverseEcho.ino sketch (in this example's
    //    Arduino/ folder) to an Arduino board.  This sketch requires
    //    the Arduino PacketSerial library https://github.com/bakercp/PacketSerial.
    // 2. Check the "listDevices" call below to make sure the correct serial
    //    device is connected.
    // 3. Run this app.

    // Connect to the first matching device.
    bool success = device.setup(115200);

    if(success)
    {
        device.registerAllEvents(this);
        ofLogNotice("ofApp::setup") << "Successfully setup " << device.port();
    }
    else
    {
        ofLogNotice("ofApp::setup") << "Unable to setup " << device.port();
    }
}

void ofApp::exit()
{
    device.unregisterAllEvents(this);
}


void ofApp::update()
{
    // Create a byte buffer.
    ofx::IO::ByteBuffer buffer("Frame Number: " + ofToString(ofGetFrameNum()));

    // Send the byte buffer.
    // ofx::IO::PacketSerialDevice will encode the buffer, send it to the
    // receiver, and send a packet marker.
    device.send(buffer);
}


void ofApp::draw()
{
    ofBackground(0);

    ofSetColor(255);

    std::stringstream ss;

    ss << "         FPS: " << ofGetFrameRate() << std::endl;
    ss << "Connected to: " << device.port();

    ofDrawBitmapString(ss.str(), ofVec2f(20, 20));

    int x = 20;
    int y = 50;
    int height = 20;

    auto iter = serialMessages.begin();

    while (iter != serialMessages.end())
    {
        iter->fade -= 20;
        
        if (iter->fade < 0)
        {
            iter = serialMessages.erase(iter);
        }
        else
        {
            ofSetColor(255, ofClamp(iter->fade, 0, 255));
            ofDrawBitmapString(iter->message, ofVec2f(x, y));

            y += height;

            if (!iter->exception.empty())
            {
                ofSetColor(255, 0, 0, ofClamp(iter->fade, 0, 255));
                ofDrawBitmapString(iter->exception, ofVec2f(x + height, y));
                y += height;
            }

            ++iter;
        }
    }
}


void ofApp::onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args)
{
    // Decoded serial packets will show up here.
    SerialMessage message(args.buffer().toString(), "", 255);
    serialMessages.push_back(message);
}


void ofApp::onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args)
{
    // Errors and their corresponding buffer (if any) will show up here.
    SerialMessage message(args.buffer().toString(),
                          args.exception().displayText(),
                          500);

    serialMessages.push_back(message);
}
