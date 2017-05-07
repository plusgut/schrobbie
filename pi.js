#! /usr/bin/env node

var Joystick     = require('joystick');
var stick        = new Joystick(0, 3500, 350);

const RUNLOOP_INTERVAL = 1000;
const OFF = 0;
const ON = 1;
var state = {};

const PINS = {
  M1IN1: 'D1',
  M1IN2: 'D2',
  M2IN1: 'D28',
  M2IN2: 'D29',
};

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
    return OFF;
  } else {
    return ON;
  }
}

function getPin(motor, direction) {
  return PINS[motor + direction];
}

function writeCommand(pin, value) {
  return 'digitalWrite(' + pin +', ' + value + ');';
}

function saveState(motor, direction, value) {
  var command = '';
  var pin = getPin(motor, direction); 
  var otherPin = getPin(motor, getOtherDirection(direction));

  if(value !== state[pin]) {
    state[pin] = value;
    command += writeCommand(pin, value); 
  }
  if(state[otherPin] !== OFF) {
    state[otherPin] = OFF;
    command += writeCommand(otherPin, OFF);                     
  }

  if(command !== '') {
    console.log(command);
  }
}

stick.on('axis', (evt) => {
  var motor = getMotor(evt.number);
  if(motor) {
    var direction = getDirection(motor, evt.value);
    saveState(motor, direction, getValue(evt.value));
  }
});

setInterval(() => {
  var result = "";
  for(var pin in state) {
    if(state.hasOwnProperty(pin)) {
      result += writeCommand(pin, state[pin]);
    }
  }
 // console.log(result);
},RUNLOOP_INTERVAL);

function init() {
  var result = "";
  for(var index in PINS) {
    if(PINS.hasOwnProperty(index)) {
      result += 'pinMode(' + PINS[index] +',"output");';
    }
  }
  console.log(result);
}

init();
