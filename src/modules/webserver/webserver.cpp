#include "webserver.h"

void WebServer::begin(const char *ssid, const char *password)
{
    long int now = millis();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print(F("Connecting to "));
    Serial.print(ssid);

    while (WiFi.status() != WL_CONNECTED && millis() - now < 10000) //
    {
        delay(500);
        Serial.print(F("."));
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(F(" not connected."));
        return;
    }
    Serial.println();

    // lambda expression to call a class method
    server.on("/", [this]()
              { this->handleRoot(); });
    server.on("/temperatures", [this]()
              { this->handleTemperatures(); });

    server.begin();
    Serial.println(F("Serwer HTTP up and running."));
}

void WebServer::handleClient()
{
    if (WiFi.status() != WL_CONNECTED)
        return;
    server.handleClient();
}

void WebServer::handleRoot()
{

    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>ESP8266 Temperatury</title>
<style>
body { font-family: Arial; text-align: center; margin-top: 40px; }
.card { 
    display: inline-block; 
    padding: 20px; 
    margin: 10px; 
    border-radius: 10px; 
    background: #f0f0f0; 
    box-shadow: 0 4px 8px rgba(0,0,0,0.2);
}
.temp { font-size: 28px; font-weight: bold; }
</style>
</head>
<body>

<h2>Temperatury DS18B20</h2>
<div id="sensors"></div>

<script>
async function loadTemps() {
    try {
        const response = await fetch('/temperatures');
        const data = await response.json();

        let html = "";
        data.forEach(sensor => {
            html += `
            <div class="card">
                <div>id: ${sensor.id}</div>
                <div class="temp">${sensor.temp.toFixed(1)} °C</div>
                <div>time: ${sensor.time}</div>
            </div>`;
        });

        document.getElementById("sensors").innerHTML = html;
    } catch (e) {
        console.log("Błąd pobierania danych", e);
    }
}

setInterval(loadTemps, 5000);
loadTemps();
</script>

</body>
</html>
)rawliteral";

    server.send(200, "text/html", html);
}

void WebServer::handleTemperatures()
{

    server.send(200, "application/json", getTemperaturesJson());
}

String WebServer::getTemperaturesJson()
{
    JsonDocument doc;
    JsonArray array = doc.to<JsonArray>();

    uint8_t count;
    SensorData *all = data.getAllSensorData(&count);

    for (uint8_t i = 0; i < count; i++)
    {
        JsonObject obj = array.add<JsonObject>();
        obj["id"] = all->sensorId;
        obj["temp"] = round(all->temperature * 10) / 10.0;
        obj["time"] = timeToString(all->sensorReadTimestamp);
        all++;
    }
    String temperaturesJson;
    serializeJson(doc, temperaturesJson);

    return temperaturesJson;
}
