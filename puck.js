var ON  = 0; // Needs to be inverted, because 0 is not holdable at espruino
var OFF = 1;

var RUNLOOP_FREQUENCE = 5;

function Pin(pin) {
  this.obj = pin;
  this.value = OFF;
  this.pwm = 0;
}

var pins = {
  M1IN1: new Pin(D1),
  M1IN2: new Pin(D2),
  M2IN1: new Pin(D28),
  M2IN2: new Pin(D29),
};

function init() {
  readPins();
  setInterval(runloop, RUNLOOP_FREQUENCE);
}

function readPins() {
  for(var type in pins) {
    if(pins.hasOwnProperty(type)) {
      var pin = pins[type];
      console.log(type, digitalRead(pin.obj));
    }
  }
}

var increment = 0;

function runloop() {
  for(var type in pins) {
    if(pins.hasOwnProperty(type)) {
      var pin = pins[type];
      var newValue = getNewValue(pin);
      console.log(newValue, newValue !== pin.value);
      if(newValue !== pin.value) {
        digitalWrite(pin.obj, newValue);
        pin.value = newValue;
      }
    }
  }
  increment++;
}

function getNewValue(pin) {
  if(pin.pwm && increment % pin.pwm === 0) {
    return ON;
  } else {
    return OFF;
  }
}

setTimeout(init, 1000); // Needs to be delayed, because of espruino delaybug, except M1IN1