#ifndef Update_H
#define Update_H

#include <pgmspace.h>

const char update[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Update</title>
    <style>
        .upload-container {
            background: #1e1e1e;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.5);
            text-align: center;
            width: 80%;
            max-width: 400px;
            margin-bottom: 27vh;
        }

        .upload-container input[type='file'] {
            display: none;
        }
        .upload-container label {
            display: inline-block;
            padding: 15px 25px;
            background-color: #007bff;
            color: white;
            border-radius: 5px;
            cursor: pointer;
            margin-bottom: 20px;
            transition: background-color 0.3s;
        }
        .upload-container label:hover {
            background-color: #0056b3;
        }
        #sub {
            padding: 15px 25px;
            background-color: #28a745;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            transition: background-color 0.3s;
            margin: 15px;
            font-size: large;
        }
        #sub:hover {
            background-color: #218838;
        }
        .file-name {
            margin-top: 10px;
            font-size: 16px;
        }

        #main-container {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            padding: 0;
        }

        html {
            box-sizing: border-box;
            --bgColorMenu: #333342;
            --bgColorMain: #1f1f23;
            --duration: .7s;    
            --fontColor: #fff;
        }

        body {
            background: var(--bgColorMain);
            background-size: cover;
            background-position: center;
            background-repeat: repeat;
            color: var(--fontColor);
            user-select: none;
            margin: 0;
            padding: 0;
        }

        #title {
            text-align: center;
            margin: 0;
            margin-bottom: 20px;
        }

        .item-container {
            display: flex;
            justify-content: center;
            align-items: center;
            flex-direction: column;
        }

        .item {
            background: var(--bgColorMenu);
            width: 80%;
            height: 30px;
            display: flex;
            align-items: center;
            direction: rtl;
            padding: 20px;
            border-radius: 20px;
            margin: 7px;
            font-size: large;
            justify-content: space-between;
            opacity: 1;
            transition: all 0.5s;
        }
        .item:active {
            opacity: 0.4;
        }
        .item:hover {
            background-color: #47475e;
            transform: scale(1.07);
        }

        .menu {
            margin: 0;
            padding: 0;
            display: flex;
            width: 100%;
            font-size: medium;
            position: fixed;
            bottom: 0;
            left: 0;
            align-items: center;
            justify-content: space-around;
            background-color: var(--bgColorMenu);
            height: 60px;
        }

        .menu__item {
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
            transition: transform var(--timeOut, var(--duration));
            flex-direction: column;
            color: #fff;
            line-height: 1em;
        }

        .icon {
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
            color: #01fff4;
        }

        .menu__item.active {
            color: #01fff4;
        }

        @keyframes strok {
            100% {
                stroke-dashoffset: 400;
            }
        }

        @media screen and (max-width: 50em) {
            .menu {
                font-size: .8em;
            }
        }
    </style>
</head>
<body>
    <div id='main-container'>
        <div class='upload-container'>
            <h2 id='title'>تحديث نظام الجهاز</h2>
            <form id='uploadForm' method='POST' action='/update' enctype='multipart/form-data'>
                <input type='file' id='fileInput' name='update' onchange='displayFileName()' accept=".bin">
                <label for='fileInput'>اختر ملف التحديث</label>
                <div id='fileName' class='file-name'></div>
                <input type='submit' id='sub' value='رفع'>
            </form>
            
        </div>
    </div>
    <script>
        function displayFileName() {
            const fileInput = document.getElementById('fileInput');
            const fileNameDiv = document.getElementById('fileName');
            const fileName = fileInput.files[0] ? fileInput.files[0].name : 'No file chosen';
            fileNameDiv.textContent = fileName;
        }

        function goSettings() {
            window.open('settings', '_self');
        }

        function goHome() {
            window.open('/', '_self');
        }
    </script>
    <footer>
        <div class='menu'>
            <button class='menu__item' style='--bgColorItem: #ff8c00;' onclick='goHome()'>
                <svg class='icon' viewBox='0 0 24 24'>
                    <path d='M12,5.69l-7,7V19h5v-6h4v6h5v-6.31l-7-7Z' />
                </svg>
                الرئيسية
            </button>
            <button class='menu__item active' style='--bgColorItem: #f54888;' onclick='goSettings()'>
                <svg class='icon' viewBox='0 0 24 24'>
                    <path d='M19.14,12.94c.04-.3.06-.61.06-.94c0-.32-.02-.64-.06-.94l2.03-1.58a.49.49,0,0,0,.12-.61l-1.92-3.32a.48.48,0,0,0-.59-.22l-2.39,1c-.5-.38-1.03-.7-1.62-.94L14.4,2.81a.491.491,0,0,0-.49-.44H9.6a.49.49,0,0,0-.49.44L9,5.04c-.59.24-1.13.57-1.62.94l-2.39-1a.48.48,0,0,0-.59.22l-1.92,3.32c-.1.18-.03.39.12.61l2.03,1.58c-.04.3-.06.61-.06.94s.02.64.06.94l-2.03,1.58a.49.49,0,0,0-.12.61l1.92,3.32c.12.21.37.29.59.22l2.39-1c.5.38,1.03.7,1.62.94l.36,2.23a.49.49,0,0,0,.49.44h3.84a.49.49,0,0,0,.49-.44l.36-2.23c.59-.24,1.13-.57,1.62-.94l2.39,1c.22.08.47,0,.59-.22l1.92-3.32a.48.48,0,0,0-.12-.61Z' />
                </svg>
                الاعدادات
            </button>
        </div>
    </footer>
</body>
</html>

)rawliteral";

#endif // Update_H
