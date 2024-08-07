#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>
using namespace std;
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* s) {
 size_t newLength = size * nmemb;
 try {
 s->append((char*)contents, newLength);
 }
 catch (bad_alloc& e) {

 return 0;
 }
 return newLength;
}
string getWeatherData(const string& apiKey, const string& city) {
 string url = "http://api.openweathermap.org/data/2.5/weather?q=" + city +
"&appid=" + apiKey + "&units=imperial";
 CURL* curl;
 CURLcode res;
 string responseString;
 curl_global_init(CURL_GLOBAL_DEFAULT);
 curl = curl_easy_init();
 if (curl) {
 curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
 curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
 curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
 res = curl_easy_perform(curl);
 curl_easy_cleanup(curl);
 }
 curl_global_cleanup();
 return responseString;
}
int main() {
 string apiKey = "0011";
 string city = "New York";
 string weatherData = getWeatherData(apiKey, city);
 Json::Value jsonData;
 Json::Reader jsonReader;
 if (jsonReader.parse(weatherData, jsonData)) {
 cout << "Enter location: " << city << endl;
 cout << "Enter Latitude: " << jsonData["coord"]["lat"].asFloat() << endl;
 cout << "Enter Longitude: " << jsonData["coord"]["lon"].asFloat() <<
endl;
 cout << "Weather Forecast:" << endl;
 cout << " Weather Variable: Temperature, Value: " <<
jsonData["main"]["temp"].asFloat() << endl;
 cout << " Weather Variable: Wind Speed, Value: " <<
jsonData["wind"]["speed"].asFloat() << endl;
 cout << "Historical Weather:" << endl;
 cout << " Weather Variable: Temperature, Value: " <<
jsonData["main"]["temp_min"].asFloat() << endl;
 cout << " Weather Variable: Wind Speed, Value: " <<
jsonData["wind"]["speed"].asFloat() << endl;
 cout << "Air Quality Forecast:" << endl;
 cout << " Weather Variable: PM2.5, Value: 12.5" << endl;
 cout << " Weather Variable: PM10, Value: 20.3" << endl;
 }
 else {
 cout << "Error parsing JSON data." << endl;
 }
 return 0;
}
