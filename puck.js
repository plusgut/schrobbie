var ON  = 1; // Needs to be inverted, because 0 is not holdable at espruino
var OFF = 0;

var WRITE_ALLWAYS = 1;
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

pins.M1IN2.pwm = 0;

function init() {
  writePins();
  readPins();
  setInterval(readPins, 1000);
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

function writePins() {
  for(var type in pins) {
    if(pins.hasOwnProperty(type)) {
      var pin = pins[type];
      digitalWrite(pin.obj, pin.value);
      //console.log(type, pin.value);
    }
  }
}

var increment = 0;

function runloop() {
  for(var type in pins) {
    if(pins.hasOwnProperty(type)) {
      var pin = pins[type];
      var newValue = getNewValue(pin);

      if(newValue || newValue !== pin.value || WRITE_ALLWAYS) {
//      if(newValue !== pin.value) {
        console.log('CHANGE!', type, newValue);
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