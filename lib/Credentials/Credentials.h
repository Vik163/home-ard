// #ifndef Credentials_h
// #define Credentials_h

// #include <Arduino.h>
// #include <EEPROM.h>

// constexpr uint8_t WORD_MAX_SIZE{32};
// constexpr uint8_t EEPROM_SIZE{255};
// constexpr uint8_t ADDRESS_SSID{0};
// constexpr uint8_t ADDRESS_SSID_PASSWORD{32};
// constexpr uint8_t ADDRESS_EMAIL{64};
// constexpr uint8_t ADDRESS_EMAIL_PASSWORD{96};
// constexpr uint8_t ADDRESS_USER_ID{128};
// constexpr uint8_t ADDRESS_DEVICE_ID{160};

// class Credentials
// {
// private:
//    bool _setSSID(char *ssid);
//    bool _setSSIDPassword(char *password);
//    bool _setEmail(char *email);
//    bool _setEmailPassword(char *password);
//    bool _setUserId(char *userId);
//    bool _clearSSID();
//    bool _clearSSIDPassword();
//    bool _clearEmail();
//    bool _clearEmailPassword();
//    bool _clearUserId();

// public:
//    Credentials();
//    void begin();
//    char *getSSID();
//    char *getSSIDPassword();
//    char *getEmail();
//    char *getEmailPassword();
//    char *getUserId();
//    char *getDeviceId();
//    bool setDeviceId(char *deviceId);
//    bool setWiFiCredentials(char *ssid, char *password);
//    bool setAccountCredentials(char *email, char *password);
//    bool setAccountCredentials(char *email, char *password, char *userId);
//    bool clear();
//    bool clearAll();
// };

// #endif