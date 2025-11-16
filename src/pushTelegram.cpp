#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include <credentialPush.hpp>

X509List cert(TELEGRAM_CERTIFICATE_ROOT);

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setupPushTelegram()
{
   secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
}

void sendMessageMinVoltage()
{

   bot.sendMessage(CHAT_ID, "Напряжение ниже 190В", "");
}
