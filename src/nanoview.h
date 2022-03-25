struct nv_live_power {
    uint16_t mains_voltage_v;
    uint16_t live_power_w[16];
};

struct nv_accumulated_energy {
    uint32_t counters_wh[16];
};

struct nv_firmware_version {
    uint8_t version;
};

void print_live_power(struct nv_live_power *nvlp) {

    String ipaddr = WiFi.localIP().toString();

    String payload_power = "Nanoview_Power_Measurement,source_ip="+ ipaddr +",type=live_power CH00="+String(nvlp->live_power_w[0])+",CH01="+String(nvlp->live_power_w[1])+",CH02="+String(nvlp->live_power_w[2])+",CH03="+String(nvlp->live_power_w[3])+",CH04="+String(nvlp->live_power_w[4])+",CH05="+String(nvlp->live_power_w[5])+",CH06="+String(nvlp->live_power_w[6])+",CH07="+String(nvlp->live_power_w[7])+",CH08="+String(nvlp->live_power_w[8])+",CH09="+String(nvlp->live_power_w[9])+",CH10="+String(nvlp->live_power_w[10])+",CH11="+String(nvlp->live_power_w[11])+",CH12="+String(nvlp->live_power_w[12])+",CH13="+String(nvlp->live_power_w[13])+",CH14="+String(nvlp->live_power_w[14])+",CH15="+String(nvlp->live_power_w[15]) + "\n";
    String payload_volts = "Nanoview_Power_Measurement,source_ip="+ ipaddr +",type=voltage V=" + String(nvlp->mains_voltage_v) + "\n";
    // TelnetStream.println(payload_volts);
    // TelnetStream.println(payload_power);

    int res = sendToInflux(payload_volts);
    TelnetStream.println(res);

    res = sendToInflux(payload_power);
    TelnetStream.println(res);


    // String payload = "Volts: " + String(nvlp->mains_voltage_v) + "V |";
    
    // for (int i = 0; i < 16; i++) {
    //     payload += String(nvlp->live_power_w[i]) + "W |";
    // }

    // TelnetStream.println(payload);

    // TelnetStream.printf("%-20s", "Live Energy: ");
    // TelnetStream.printf("%5dV", nvlp->mains_voltage_v);
    // for (int i = 0; i < 16; i++) {
    //     TelnetStream.printf("%5dW", nvlp->live_power_w[i]);
    // }
    // TelnetStream.printf("\n");
}

void print_accumulated_energy(struct nv_accumulated_energy *nvae) {

    String payload_power = "Nanoview_Power_Measurement,type=accumulated_power  CH00="+String(nvae->counters_wh[0])+",CH01="+String(nvae->counters_wh[1])+",CH02="+String(nvae->counters_wh[2])+",CH03="+String(nvae->counters_wh[3])+",CH04="+String(nvae->counters_wh[4])+",CH05="+String(nvae->counters_wh[5])+",CH06="+String(nvae->counters_wh[6])+",CH07="+String(nvae->counters_wh[7])+",CH08="+String(nvae->counters_wh[8])+",CH09="+String(nvae->counters_wh[9])+",CH10="+String(nvae->counters_wh[10])+",CH11="+String(nvae->counters_wh[11])+",CH12="+String(nvae->counters_wh[12])+",CH13="+String(nvae->counters_wh[13])+",CH14="+String(nvae->counters_wh[14])+",CH15="+String(nvae->counters_wh[15]) + "\n";
    TelnetStream.println(payload_power);

    int res = sendToInflux(payload_power);
    TelnetStream.println(res);
}

uint16_t nv_crc(uint8_t data[], uint8_t len) {
    uint8_t crc1 = 0, crc2 = 0;
    for (uint8_t i = 0; i < len; i++) {
        crc1 += data[i];
        crc2 ^= crc1;
    }
    return ((crc2 << 8) | crc1);
}