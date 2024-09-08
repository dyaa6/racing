#ifndef Default_H
#define Default_H

#include <pgmspace.h>

const char defaultNet[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang='ar' dir='rtl' class='dark-mode'>

<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>تغيير كلمة المرور واسم الشبكة الإفتراضية</title>
    <style>
           html{
            --bgColorMain:#1f1f23;
            --bgColorMenu : #333342;
            --fontColor:#fff;
            --tintColor:#01fff4;

        }
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            height: 100vh;
            transition: background-color 0.3s;
            background-color: var(--bgColorMain);
            user-select: none;
            direction:rtl;
        }

        .container {
            padding: 20px;
            width: 80%;
        }

        h2 {
            text-align: center;
            color: var(--fontColor);
        }

        form {
            display: flex;
            flex-direction: column;
            align-items: center;
        }

        input[type='text'],
        input[type='password'] {
            width: 100%;
            padding: 10px;
            margin: 10px 0;
            border: 1px solid #ccc;
            border-radius: 5px;
            box-sizing: border-box;
            transition: border-color 0.3s;
            text-align: right;
            background:#fff;
            font-size: 1em;
        }

        input[type='text']:focus,
        input[type='password']:focus {
            outline: none;
            border-color: var(--tintColor);
        }

        input[type='submit'] {
            width: 100%;
            padding: 7px;
            margin-top: 20px;
            background-color: var(--tintColor);
            color: var(--bgColorMenu);
            border: none;
            border-radius: 5px;
            cursor: pointer;
            transition: background-color 0.3s;
            font-size: large;
        }

        input[type='submit']:hover {
            background-color: #6a5acd;
        }

        .error-message {
            color: red;
            margin-top: 10px;
        }


    </style>
</head>

<body>
    <div class='container' id='container'>
        <h2 id='title'>تغيير كلمة المرور واسم الشبكة الإفتراضية</h2>
        <form action='#' method='post' id='passwordForm'>
            <input type='text' name='new_password' id='new_password' placeholder='كلمة المرور الافتراضية'>
            <input type='text' name='new_ssid' id='new_ssid' placeholder='اسم الشبكة اللاسلكية الافتراضية'>
            <input type='submit' id='sub-button' class='submit' value='حفظ'>
            <div class='error-message' id='error-message'></div>
        </form>
    </div>

    <script>
        document.getElementById('sub-button').addEventListener('click', function (event) {
            event.preventDefault(); // Prevent the default form submission

            // Get form data
            const newPassword = document.getElementById('new_password').value;
            const newSsid = document.getElementById('new_ssid').value;

            // Validate input
            if (newPassword.length >= 8) {
                document.getElementsByClassName('error-message')[0].innerText = ''; // Clear previous error message

                // Send data to server
                try {
                    fetch('http://2.2.2.2/defaultNetwork?defPass=' + newPassword + '&ssid=' + newSsid)
                        .then(response => {
                            if (response.ok) {
                                return response.text();
                            } else {
                                throw new Error('Network error');
                            }
                        })
                        .then(data => {
                            // Handle successful response
                            let msg=document.getElementsByClassName('error-message')[0];
                            msg.innerText = 'تم حفظ الاعدادات بنجاح';
                            msg.style.color='green';
                        })
                        .catch(error => {
                            // Handle errors
                            console.error('Error fetching data:', error);
                            document.getElementsByClassName('error-message')[0].innerText = 'هناك مشكلة';
                        });
                } catch (e) {
                    // Handle other errors
                    console.error('Error:', e);
                    document.getElementById('error-message').innerText = 'هناك خطأ ما!';
                }
            } else {
                // Display error message for password length
                document.getElementsByClassName('error-message')[0].innerText = 'يجب إدخال 8 حروف او أرقام على الأقل';
            }
        });
    </script>
</body>

</html>

  )rawliteral";

#endif // Default_H