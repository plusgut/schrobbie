var pins = {
  M1IN1: D1,
  M1IN2: D2,
  M2IN1: D28,
  M2IN2: D29,
};

//analogWrite(pins.M1IN1, 0);
//analogWrite(pins.M1IN2, 0);
//analogWrite(pins.M2IN1, 0);
//analogWrite(pins.M2IN2, 0);

analogWrite(pins.M1IN1, 1);

for(var type in pins) {
  if(pins.hasOwnProperty(type)) {
    var pin = pins[type];
    console.log(type, analogRead(pin));
    // M1IN1 NaN
    // M1IN2 0.99975585937
    // M2IN1 0.99975585937
    // M2IN2 0.99975585937
  }
}