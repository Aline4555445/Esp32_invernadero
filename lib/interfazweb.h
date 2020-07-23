#include <arduino.h>

class conexionWeb
{
private:
    HTTPClient http;
    bool postSucces;
    bool getSucces;
    String url_server;

public:
    char response[200];
    char POSTresponse[200];
    char GETresponse[2048];
    conexionWeb(const char *_SSID, const char *_PASSWORD)
    {
       /* WiFi.begin(_SSID, _PASSWORD);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print("...");
        }*/
    }

    bool webPOST(StaticJsonDocument<512> _post, String _server_URL)
    {
        String data = "";
        if ((WiFi.status() == WL_CONNECTED))
        {
            serializeJson(_post, data);
            http.begin(_server_URL);
            http.addHeader("Content-Type", "application/json");
            int httpResponseCode = http.POST(data);
            if (httpResponseCode > 0)
            {
                http.getString().toCharArray(POSTresponse, sizeof(POSTresponse));
                postSucces = true;
            }
            else
            {
                postSucces = false;
            }
            http.end();
        }
        return postSucces;
    }
    void ClearPostResponse()
    {
        POSTresponse[0] = (char)0;
        GETresponse[0] = (char)0;
    }
    bool webGET(const char *URL)
    {
        if ((WiFi.status() == WL_CONNECTED))
        {
            ClearPostResponse();
            http.begin(URL);
            int httpCode = http.GET();
            if (httpCode > 0)
            {
                if (httpCode == HTTP_CODE_OK)
                {
                    getSucces = true;
                    http.getString().toCharArray(GETresponse, sizeof(GETresponse));
                }
                else
                {
                    //  ClearPostResponse();
                    getSucces = false;
                }
            }
            http.end();
        }

        return getSucces;
    }
};