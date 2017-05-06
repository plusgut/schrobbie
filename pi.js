#! /usr/bin/env node

var Joystick     = require('joystick');
var nodeEspruino = require('node-espruino');

var stick        = new Joystick(0, 3500, 350);

function getMotor(number){
  const AXIS = {
    1: 'M1',
    5: 'M2'
  }

  return AXIS[number];
}

function getDirection(motor, value) {
  if(value < 0) {
    return 'IN1';
  } else {
    return 'IN2';
  }
};

function getOtherDirection(direction) {
  if(direction === 'IN1') {
    return 'IN2';
  } else {
    return 'IN1';
  }
}

function getValue(value) {
  if(value === 0) {
    return 0;
  } else {
    return 1;
  }
}

function getPin(motor, direction) {
  const PINS = {
    M1IN1: 'D1',
    M1IN2: 'D2',
    M2IN1: 'D28',
    M2IN2: 'D29',
  }

  return PINS[motor + direction];
}

function writeCommand(motor, direction, value) {
  console.log('digitalWrite('+getPin(motor, direction) +', ' + value + ')');
}

stick.on('axis', (evt) => {
  var motor = getMotor(evt.number);
  if(motor) {
    var direction = getDirection(motor, evt.value);
    var otherDirection = getOtherDirection(direction);
    writeCommand(motor, direction, getValue(evt.value)) 
    writeCommand(motor, otherDirection, 0); 

  }

});
