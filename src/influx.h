
int counter = 0;

void setupInflux()
{
    counter = 0;
}

void loopInflux()
{
    HTTPClient http;
    http.begin(influx_server_string);
    http.addHeader("Content-Type", "application/binary");


    if (counter > 6699){ // random arbritary number before it resets back to 0
      counter = 0;
    }
      
    String payload = "TestMeasurement,tagKey=testTag counter=" + String(counter*(counter + 1)) + "\n";

    counter++;

    TelnetStream.println(payload);

    Serial.println("posting: " + payload);
    TelnetStream.print("posting: " + payload);

    // for(int i=0; i<bufferIndex; i++)
    // {
        // pos = sprintf(pos,"moisture level=%d,threshold=%d %d000000000\n",
        //                measBuffer[i],thresholdBuffer[i],timestampBuffer[i]);
    // }    

    sleep(1);

    http.POST(payload);
    http.end();
}