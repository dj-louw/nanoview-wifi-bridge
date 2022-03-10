
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


    String payload = "TestMeasurement,tagKey=testTag counter=" + String(counter*(counter + 1)) + "\n";



    // for(int i=0; i<bufferIndex; i++)
    // {
        // pos = sprintf(pos,"moisture level=%d,threshold=%d %d000000000\n",
        //                measBuffer[i],thresholdBuffer[i],timestampBuffer[i]);
    // }    

    //sleep(1);

    http.POST(payload);
    http.end();
}

int sendToInflux(String payload)
{
    HTTPClient http;
    http.begin(influx_server_string);
    http.addHeader("Content-Type", "application/binary");


    //sleep(1);

    int res = http.POST(payload);
    http.end();
    return res;
}