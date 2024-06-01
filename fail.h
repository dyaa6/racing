#ifndef Fail_H
#define Fail_H

#include <pgmspace.h>

const char fail_m[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang='ar' dir='rtl'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Success</title>
    <style>
        body {background: #121212; color: #fff; font-family: Arial, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; text-align: center;}
        .message {color: red; font-size: 2em; margin-bottom: 20px; direction: rtl;}
        .back-button {padding: 10px 20px; font-size: 1em; color: #fff; background: #1a73e8; border: none; border-radius: 5px; text-decoration: none;}
        .back-button:hover {background: #135ab6;}
    </style>
</head>
<body>
    <div>
        <div class='message'>
          لم يتم تحديث نظام الجهاز!
          <br>
          تأكد من اختيار ملف التحديث المناسب.
        </div>
        <a class='back-button' href='http://2.2.2.2/update'> 
        رجوع
        </a>
    </div>
</body>
</html>
)rawliteral";

#endif // Fail_H
