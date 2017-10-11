https://drive.google.com/drive/u/0/folders/0B1pnzvhfGZgfeU93TWhFMGRZaFk

var express = require('express');
var request = require('request');
var gpio = require('rpi-gpio');
const mqtt = require('mqtt'); 
var admin = require("firebase-admin");
var serviceAccount = require("xxxx");

var defaultApp = admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://fiap-iot-19d66.firebaseio.com"
});

var app  = express();
const client = mqtt.connect('mqtt://test.mosquitto.org');
var valor_atual;
var valor_anterior;
var valor_anterior2;
var valor_anterior3;
var pinPedal = 7;
const TOPICO_SUBSCRIBE_GUITARRA = 'MQTTSensorGuitarra';
var db = admin.database();
var ref2 = db.ref("pedal");
var posPedal;

var port = process.env.PORT || 8080;


app.use(function(req, res, next) {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  next();
});

ref2.on("value", function(snapshot) {
  posPedal = snapshot.val();
  configurarServo();
  console.log("Servo atualizado: " + snapshot.val());

}, function (errorObject) {
  console.log("The read failed: " + errorObject.code);
});

client.on('connect', () => {  
  // Inform controllers that garage is connected
  console.log("Conectado na fila MQTT");
  client.subscribe(TOPICO_SUBSCRIBE_GUITARRA);
  console.log("Subscribed no tópico: " + TOPICO_SUBSCRIBE_GUITARRA);
  console.log("");
  gpio.setup(pinPedal, gpio.DIR_OUT);

  configurarServo();
  console.log("Servo Configurado!");
})

 

client.on('message', (topic, message) => {  
  valor_anterior3 = valor_anterior2;
  valor_anterior2 = valor_anterior;
  valor_anterior = valor_atual; 
  valor_atual = parseInt(message.toString());
  console.log(valor_atual);

  if ((valor_anterior3 > valor_anterior2) && (valor_anterior > valor_atual) && (valor_anterior2 <= -100 ) && (valor_anterior >=80) ){
      //write(true);
    acionarPedal();
    console.log("movimento !!");
    console.log("valor_anterior3: "+ valor_anterior3);
    console.log("valor_anterior2: "+ valor_anterior2);
    console.log("valor_anterior: "+ valor_anterior);
    console.log("valor_atual: "+ valor_atual);
    valor_anterior3 = valor_atual;
    valor_anterior2 = valor_atual;
    valor_anterior = valor_atual; 
  }

});


app.get('/hello2', function(req,res){
  configurarServo();
  //write(false);
  res.send("Hello");
});

app.get('/hello', function(req,res){
  acionarPedal();
  //write(false);
  res.send("Hello");
});

app.listen(port, function(){
  console.log('Servidor no ar na porta ' + port);
});

function write(val) {
    gpio.write(pinPedal, val, function(err) {
        if (err) throw err;
        console.log('Written to pin');
    });
}

function configurarServo(){

require('child_process').exec("gpio -g mode 18 pwm; gpio pwm-ms; gpio pwmc 192; gpio pwmr 2000; gpio -g pwm 18 "+ posPedal, function (error, stdout, stderr) {
  //console.log('stdout: ' + stdout);
  //console.log('stderr: ' + stderr);
  if (error !== null) {
    console.log('exec error: ' + error);
  }
});
}

function acionarPedal(){
var val2 = posPedal -30;
require('child_process').exec("gpio -g mode 18 pwm; gpio pwm-ms; gpio pwmc 192; gpio pwmr 2000; gpio -g pwm 18 " +val2, function (error, stdout, stderr) {
  //console.log('stdout: ' + stdout);
  //console.log('stderr: ' + stderr);
  if (error !== null) {
    console.log('exec error: ' + error);
  }
});
console.log('pedal acionado com: '+val2);
setTimeout(function(){
  require('child_process').exec("gpio -g mode 18 pwm; gpio pwm-ms; gpio pwmc 192; gpio pwmr 2000; gpio -g pwm 18 "+ posPedal, function (error, stdout, stderr) {
    //console.log('stdout: ' + stdout);
    //console.log('stderr: ' + stderr);
    if (error !== null) {
      console.log('exec error: ' + error);
    }
});}, 1000);
}


app.use(express.static('public'));

console.log('to no ar');
