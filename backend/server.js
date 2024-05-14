const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const mqtt = require('mqtt');

const app = express();
const port = 8080;

app.use(cors());
app.use(bodyParser.json());

const mqttClient = mqtt.connect('mqtt://test.mosquitto.org');

let sensorData = {
  temperature: 0,
  humidity: 0
};

mqttClient.on('connect', () => {
  mqttClient.subscribe('sensor/data');
});

mqttClient.on('message', (topic, message) => {
    const messageString = message.toString();
    try {
      if (messageString !== 'Connected'){
        const data = JSON.parse(message.toString());
        sensorData.temperature = data.temp;
        sensorData.humidity = data.humid;
        console.log('Updated sensor data:', sensorData);
      }
    } catch (error) {
      console.error('Failed to parse message as JSON:', messageString);
    }
  });

app.get('/data', (req, res) => {
  res.json(sensorData);
});

app.listen(port, () => {
  console.log(`Server running on http://localhost:${port}`);
});
