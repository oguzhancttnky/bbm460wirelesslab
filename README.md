<h3>Device Setup</h3>
<p>An ESP32 microcontroller reads humidity and temperature from a DHT22 sensor. It then connects to a WiFi network and establishes a connection to an MQTT broker (test.mosquitto.org).</p>
![wokwiImage](https://github.com/oguzhancttnky/bbm460wirelesslab/assets/59288589/2533ae5f-e90c-427e-8dbb-5ae0758b47a5)
<h3>Dashboard</h3>
<p>The device publishes the sensor data with a status message to MQTT topic sensor/data. 
  Server uses the MQTT broker to subscribe to the sensor/data topic. When it receives message it updates the sensor data stored in a local variable.</p>
![dashboardImage](https://github.com/oguzhancttnky/bbm460wirelesslab/assets/59288589/bb959d0a-3764-4de5-8623-0bd5eff88beb)
<p>The server uses an HTTP endpoint (/data) which accessed with a web request, responds with the latest sensor data.</p>
