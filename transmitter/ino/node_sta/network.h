#ifndef Net_H
#define Net_H

#include <pgmspace.h>

const char network[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang='ar' dir='rtl' class='dark-mode'>

<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title> 
        إتصال بشبكة واي فاي جديدة
    </title>
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




        
.menu{
    margin: 0;
    padding: 0px;
    display: flex;
    /* Works well with 100% width  */
    width: 100% 	;
    font-size: medium;
    position: fixed;
    bottom: 0px;
    left: 0px;
    align-items: center;
    justify-content: space-around;
    background-color: var(--bgColorMenu);
    height: 60px;
}

.menu__item{
    all: unset;
    flex-grow: 1;
    z-index: 100;
    display: flex;
    cursor: pointer;
    position: relative;
    border-radius: 50%;
    align-items: center;
    will-change: transform;
    justify-content: center;
    padding: 0.55em 0 0.85em;
    transition: transform var(--timeOut , var(--duration));
    flex-direction: column;
    color:#fff;
    line-height: 0.8em;
    font-size:medium;
}


.icon{
    width: 35px;
    height: 35px;
    stroke: white;
    fill: transparent;
    stroke-width: 1pt;
    stroke-miterlimit: 10;
    stroke-linecap: round;
    stroke-linejoin: round;
    stroke-dasharray: 400;
}

.menu__item.active .icon {

animation: strok 1.5s reverse;
stroke: #01fff4;
color:#01fff4;
}
.menu__item.active{

color:#01fff4;
}

@keyframes strok {

100% {

    stroke-dashoffset: 400;

}

}
    </style>
</head>

<body>
    <div class='container' id='container'>
        <h2 id='title'>تغيير كلمة المرور واسم الشبكة اللاسلكية</h2>
        <form action='/submit' method='post' id='passwordForm'>
            <input type='text' name='ssid' id='new_ssid' placeholder='اسم الشبكة اللاسلكية' >
            <input type='password' name='password' id='rep_password' placeholder='كلمة المرور' >
            <input type='submit' id='sub-button' class='submit' value='إرسال'>
            <div class='error-message' id='error-message'>
              
            </div>
        </form>
        <!--button onclick='toggleDarkMode()'>🌙</button-->
    </div>
</body>
<footer>
    <menu class='menu'>
    
        <button class='menu__item active' style='--bgColorItem: #f54888;' onclick='goSettings()'>
            <svg class='icon' viewBox='0 0 24 24'> 
                <path d='M19.14,12.94c.04-.3.06-.61.06-.94c0-.32-.02-.64-.06-.94l2.03-1.58a.49.49,0,0,0,.12-.61l-1.92-3.32a.48.48,0,0,0-.59-.22l-2.39,1c-.5-.38-1.03-.7-1.62-.94L14.4,2.81a.491.491,0,0,0-.49-.44H9.6a.49.49,0,0,0-.49.44L9,5.04c-.59.24-1.13.57-1.62.94l-2.39-1a.48.48,0,0,0-.59.22l-1.92,3.32c-.1.18-.03.39.12.61l2.03,1.58c-.04.3-.06.61-.06.94s.02.64.06.94l-2.03,1.58a.49.49,0,0,0-.12.61l1.92,3.32c.12.21.37.29.59.22l2.39-1c.5.38,1.03.7,1.62.94l.36,2.23a.49.49,0,0,0,.49.44h3.84a.49.49,0,0,0,.49-.44l.36-2.23c.59-.24,1.13-.57,1.62-.94l2.39,1c.22.08.47,0,.59-.22l1.92-3.32a.48.48,0,0,0-.12-.61Z' /> 
            </svg>
        الاعدادات
        </button>
        <button class='menu__item' style='--bgColorItem: #ff8c00;'  onclick='goHome()'>
            <svg class='icon' viewBox='0 0 24 24'> <path d='M12,5.69l-7,7V19h5v-6h4v6h5v-6.31l-7-7Z' /> </svg>
            الرئيسية
        </button>
    

      </menu>
    <script>
        const goSettings=()=>{
            window.open('settings', '_self');
            }
        const goHome=()=>{
            window.open('index', '_self');
            }
    </script>

</footer>
</html>


)rawliteral";

#endif // Net_H