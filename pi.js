#! /usr/bin/env node

var axis = {
  1: 'left',
  4: 'right'
}

var Joystick     = require('joystick');

var stick        = new Joystick(0, 3500, 350);
stick.on('button', (evt) => {
  console.log(evt.id, evt.value);
});

stick.on('axis', (evt) => {
  if(evt.number in axis) {
    console.log(axis[evt.number], evt.value);
  }
});
