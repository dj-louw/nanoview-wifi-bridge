int sendToInflux(String payload)
{
    HTTPClient http;
    http.begin(influx_server_string);
    http.addHeader("Content-Type", "application/binary");
    int res = http.POST(payload);
    http.end();
    return res;
}