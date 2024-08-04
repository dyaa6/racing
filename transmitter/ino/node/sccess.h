#ifndef Scc_H
#define Scc_H

#include <pgmspace.h>

const char sccess_m[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang='ar' dir='rtl'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Success</title>
    <style>
        body {background: #121212; color: #fff; font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; text-align: center;}
        .message {color: #00ff00; font-size: 2em; margin-bottom: 20px; direction: rtl;}
        .back-button {padding: 10px 20px; font-size: 1em; color: #fff; background: #1a73e8; border: none; border-radius: 5px; text-decoration: none;}
        .back-button:hover {background: #135ab6;}
    </style>
</head>
<body>
    <div>
        <div class='message'>تم تحديث نظام الجهاز بنجاح</div>
        <a class='back-button' href='http://192.168.4.1'>الصفحة الرئيسية</a>
    </div>
</body>
</html>
)rawliteral";

#endif // Scc_H
