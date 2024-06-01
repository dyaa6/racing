#ifndef Remote_H
#define Remote_H

#include <pgmspace.h>

const char remote[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang='ar'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Settings</title>
    <style>
        
  /*tap bar*/
html {
    box-sizing: border-box;
    --bgColorMenu : #333342;
    --bgColorMain:#1f1f23;
    --duration: .7s;
    --fontColor:#fff;
}

body{
    background: var(--bgColorMain);
    background-size: cover;
    background-position: center;
    background-repeat: repeat;
    color:var(--fontColor);
    user-select: none;
}
#title{
    text-align: center;
}
.item-container{
    display: flex;
    justify-content: center;
    align-items: center;
     flex-direction: column;
}
.row{
    width: 100%;
    display: flex;
    justify-content: space-between;
    font-size: larger;
    padding-inline-start: 10px;
    padding-inline-end: 10px;
}
.btn{
    font-size: medium;
    border-radius: 10px;
    padding: 3px;
    padding-inline-start: 17px;
    padding-inline-end: 15px;
}
.label{
    margin-bottom: 5px;
    margin-top: -5px;
    font-size: medium;
}
.waiting{
    background: #ff8c00 !important;
    transition: all 1s;
}
.alertBox{
    width: 100%;
    text-align: center;
    color: red;
}
.item{
    background: var(--bgColorMenu);
    width: 80%;
    display: flex;
    align-items: center;
    direction: rtl;
    padding: 20px;
    border-radius: 20px;
    margin: 7px;
    font-size: large;
    display: flex;
    flex-direction: column;
    opacity: 1;
}
.item:active{
    opacity: 0.4;
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
    line-height: 1em;
    transition: all 1s;
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





@media screen and (max-width: 50em) {
.menu{
    font-size: .8em;
}
}
    </style>
</head>
<body>
    <h2 id='title'>
ريموت كنترول
    </h2>

    
    <div id='isconnected' class='alertBox' style='display: none;'>
        لايوجد اتصال
    </div>
    <div class='item-container'>
        <div class='item' id='authPower' onclick='auth(`authPower`)'>
            <div class='row'>
             إطفاء
             <svg width='45px' height='45px' viewBox='0 0 24 24' fill='none' xmlns='http://www.w3.org/2000/svg' stroke='#ffffff'>
                <g id='SVGRepo_bgCarrier' stroke-width='0'/>
                <g id='SVGRepo_tracerCarrier' stroke-linecap='round' stroke-linejoin='round'/>
                <g id='SVGRepo_iconCarrier'> <path fill-rule='evenodd' clip-rule='evenodd' d='M13 3C13 2.44772 12.5523 2 12 2C11.4477 2 11 2.44772 11 3V12C11 12.5523 11.4477 13 12 13C12.5523 13 13 12.5523 13 12V3ZM8.6092 5.8744C9.09211 5.60643 9.26636 4.99771 8.99839 4.5148C8.73042 4.03188 8.12171 3.85763 7.63879 4.1256C4.87453 5.65948 3 8.61014 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12C21 8.66747 19.1882 5.75928 16.5007 4.20465C16.0227 3.92811 15.4109 4.09147 15.1344 4.56953C14.8579 5.04759 15.0212 5.65932 15.4993 5.93586C17.5942 7.14771 19 9.41027 19 12C19 15.866 15.866 19 12 19C8.13401 19 5 15.866 5 12C5 9.3658 6.45462 7.06997 8.6092 5.8744Z' fill='#ffffff'/> </g>
            </svg>
            </div>
            <div class='row'>
                <div id='offWait' class='label'>
                    انقر لإضافة زر جديد
                </div>
            </div>
              <div class='row'>
                <button class='btn' onclick='reset(`resetPower`); event.stopPropagation();'>تصفير</button>
                <div id='offCode' class='label'>
                0
                </div>
              </div>
        </div>
        <div class='item' id='authRun' onclick='auth(`authRun`)'>
            <div class='row'>
             تشغيل
             <svg fill='#ffffff' height='45px' width='45px' version='1.1' id='Layer_1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' viewBox='0 0 511.998 511.998' xml:space='preserve' stroke='#ffffff'>
                <g id='SVGRepo_bgCarrier' stroke-width='0'/>
                <g id='SVGRepo_tracerCarrier' stroke-linecap='round' stroke-linejoin='round'/>
                <g id='SVGRepo_iconCarrier'> <g> <g> <circle cx='256.601' cy='347.768' r='21.829'/> </g> </g> <g> <g> <path d='M462.997,253.8l-36.958-36.958c-6.513-6.513-17.075-6.513-23.59,0l-9.284,9.284l-79.3-79.301V112.59 c0-9.212-7.469-16.681-16.681-16.681h-5.323V70.982h9.982c5.26,0,10.214-2.481,13.363-6.696c3.148-4.215,4.125-9.667,2.633-14.712 l-11.12-37.62C304.623,4.864,298.113,0,290.722,0h-69.366c-7.391,0-13.902,4.864-15.997,11.952l-11.12,37.62 c-1.491,5.045-0.515,10.498,2.633,14.712c3.149,4.215,8.102,6.696,13.363,6.696h9.982v24.926h-4.211 c-9.212,0-16.681,7.469-16.681,16.681v33.124l-80.453,80.454l-9.326-9.326c-3.128-3.128-7.371-4.886-11.794-4.886 s-8.667,1.757-11.794,4.886l-36.958,36.958c-6.514,6.513-6.514,17.075,0,23.59l9.435,9.435c-6.514,6.513-6.514,17.075,0,23.59 s17.075,6.514,23.59,0l16.12,16.12c-6.514,6.513-6.514,17.075,0,23.59c6.514,6.514,17.075,6.514,23.59,0l16.12,16.12 c-6.514,6.514-6.514,17.075,0,23.59c6.512,6.514,17.074,6.515,23.59,0l8.413,8.413v97.073c0,9.212,7.469,16.681,16.681,16.681 h139.005c9.212,0,16.681-7.469,16.681-16.681v-97.156l8.331-8.331c6.514,6.514,17.075,6.514,23.59,0 c6.514-6.513,6.514-17.075,0-23.59l16.12-16.12c6.514,6.514,17.075,6.514,23.59,0c6.514-6.513,6.514-17.075,0-23.59l16.12-16.12 c6.512,6.514,17.074,6.515,23.59,0c6.514-6.513,6.514-17.075,0-23.59l9.435-9.435C469.511,270.875,469.511,260.313,462.997,253.8z M121.733,302.944l-16.12-16.12l13.368-13.368l16.12,16.12L121.733,302.944z M280.504,129.269v11.54h-47.818v-11.54 C238.719,129.269,275.198,129.269,280.504,129.269z M253.578,95.908V70.982h4.922v24.926H253.578z M233.82,33.361h44.438 l1.26,4.259c-28.884,0-19.365,0-46.958,0L233.82,33.361z M171.704,332.391l-10.262,10.263l-16.12-16.12l13.368-13.368 l14.219,14.22C172.679,328.223,172.047,330.762,171.704,332.391z M308.861,391.255v87.384H203.217v-87.302 c0-44.85-0.041-41.867,0.101-44.278c-0.562-32.934,27.105-49.683,52.721-49.683c29.302,0,52.822,23.879,52.822,52.822V391.255z M350.553,342.653l-10.156-10.156c-0.353-1.684-1.002-4.286-1.247-5.175l14.155-14.155l13.368,13.368L350.553,342.653z M369.425,249.865l-16.12,16.12c-6.513-6.513-17.075-6.513-23.589,0.001c-6.514,6.513-6.514,17.075,0,23.589l-6.598,6.598 c-15.811-19.593-40-32.158-67.078-32.158c-27.1,0-51.305,12.585-67.116,32.205l-6.643-6.643c6.514-6.513,6.514-17.075,0-23.59 c-6.513-6.513-17.075-6.513-23.59,0l-16.12-16.12c0,0-3.72,3.502,75.476-75.697h75.983 C373.257,253.397,369.425,249.865,369.425,249.865z M390.263,302.943l-13.368-13.368l16.12-16.12l13.368,13.368L390.263,302.943z'/> </g> </g> </g>
            </svg>
            </div>
            <div class='row'>
                <div id='runWait' class='label'>
                    انقر لإضافة زر جديد
                </div>
            </div>
              <div class='row'>
                <button class='btn' onclick='reset(`resetRun`); event.stopPropagation();'>تصفير</button>
                <div id='runCode' class='label'>
                0
                </div>
              </div>
        </div>
        <div class='item' id='authLock' onclick='auth(`authLock`)'>
            <div class='row'>
             قفل
             <svg fill='#fff' width='37px' height='37px' viewBox='-13.39 0 122.88 122.88' version='1.1' id='Layer_1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' enable-background='new 0 0 96.108 122.88' xml:space='preserve' stroke='#fff'>
                <g id='SVGRepo_bgCarrier' stroke-width='0'/>
                <g id='SVGRepo_tracerCarrier' stroke-linecap='round' stroke-linejoin='round'/>
                <g id='SVGRepo_iconCarrier'> <g> <path fill-rule='evenodd' clip-rule='evenodd' d='M2.892,56.036h8.959v-1.075V37.117c0-10.205,4.177-19.484,10.898-26.207v-0.009 C29.473,4.177,38.754,0,48.966,0C59.17,0,68.449,4.177,75.173,10.901l0.01,0.009c6.721,6.723,10.898,16.002,10.898,26.207v17.844 v1.075h7.136c1.59,0,2.892,1.302,2.892,2.891v61.062c0,1.589-1.302,2.891-2.892,2.891H2.892c-1.59,0-2.892-1.302-2.892-2.891 V58.927C0,57.338,1.302,56.036,2.892,56.036L2.892,56.036z M26.271,56.036h45.387v-1.075V36.911c0-6.24-2.554-11.917-6.662-16.03 l-0.005,0.004c-4.111-4.114-9.787-6.669-16.025-6.669c-6.241,0-11.917,2.554-16.033,6.665c-4.109,4.113-6.662,9.79-6.662,16.03 v18.051V56.036L26.271,56.036z M49.149,89.448l4.581,21.139l-12.557,0.053l3.685-21.423c-3.431-1.1-5.918-4.315-5.918-8.111 c0-4.701,3.81-8.511,8.513-8.511c4.698,0,8.511,3.81,8.511,8.511C55.964,85.226,53.036,88.663,49.149,89.448L49.149,89.448z'/> </g> </g>
            </svg>
            </div>
            <div class='row'>
                <div id='lockWait' class='label'>
                    انقر لإضافة زر جديد
                </div>
            </div>
              <div class='row'>
                <button class='btn' onclick='reset(`resetLock`); event.stopPropagation();'>تصفير</button>
                <div id='lockCode' class='label'>
                0
                </div>
              </div>
        </div>
        <div class='item' id='authOpen' onclick='auth(`authOpen`)'>
            <div class='row'>
             فتح
             <svg fill='#fff' width='37px' height='37px' viewBox='0 -6.61 122.88 122.88' version='1.1' id='Layer_1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' enable-background='new 0 0 122.88 109.652' xml:space='preserve' stroke='#fff'>
                <g id='SVGRepo_bgCarrier' stroke-width='0'/>
                <g id='SVGRepo_tracerCarrier' stroke-linecap='round' stroke-linejoin='round'/>
                <g id='SVGRepo_iconCarrier'> <g> <path fill-rule='evenodd' clip-rule='evenodd' d='M2.585,49.871H54.77V34.054v-0.011h0.009c0.002-9.368,3.828-17.878,9.989-24.042 c6.164-6.163,14.679-9.991,24.051-9.991V0h0.005l0,0h0.012v0.009c9.368,0.002,17.878,3.828,24.042,9.989 c6.164,6.164,9.991,14.679,9.991,24.051h0.012v0.004v15.96v2.403h-2.403h-9.811h-2.404v-2.403V33.868v-0.009h0.012 c-0.002-5.332-2.195-10.189-5.722-13.715c-3.528-3.531-8.388-5.721-13.724-5.724v0.009h-0.005l0,0h-0.011V14.42 c-5.334,0.002-10.191,2.19-13.72,5.717l0.005,0.005c-3.529,3.528-5.722,8.388-5.722,13.722h0.009v0.005v16.003h13.987 c1.422,0,2.585,1.164,2.585,2.585v54.613c0,1.422-1.163,2.583-2.585,2.583H2.585c-1.424,0-2.585-1.161-2.585-2.583V52.456 C0,51.035,1.161,49.871,2.585,49.871L2.585,49.871z M43.957,79.753l4.098,18.908l-11.232,0.045l3.297-19.162 c-3.068-0.981-5.295-3.857-5.295-7.252c0-4.202,3.411-7.613,7.614-7.613c4.202,0,7.613,3.411,7.613,7.613 C50.053,75.975,47.433,79.048,43.957,79.753L43.957,79.753z'/> </g> </g>
            </svg>
            </div>
            <div class='row'>
                <div id='openWait' class='label'>
                    انقر لإضافة زر جديد
                </div>
            </div>
              <div class='row'>
                <button class='btn' onclick='reset(`resetOpen`); event.stopPropagation();'>تصفير</button>
                <div id='openCode' class='label'>
                0
                </div>
              </div>
        </div>
        <div class='item' id='authBox' onclick='auth(`authBox`)' style='margin-bottom: 70px; display:none;'>
            <div class='row'>
             صندوق
             <svg fill='#ffffff' height='45px' width='45px' version='1.1' id='Capa_1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' viewBox='0 0 302.93 302.93' xml:space='preserve' stroke='#ffffff'>
                <g id='SVGRepo_bgCarrier' stroke-width='0'/>
                <g id='SVGRepo_tracerCarrier' stroke-linecap='round' stroke-linejoin='round'/>
                <g id='SVGRepo_iconCarrier'> <path d='M294.289,126.945h-31.929l38.373-38.374c2.929-2.929,2.929-7.678,0-10.606c-2.929-2.929-7.678-2.929-10.607,0 l-43.031,43.032l-14.579-37.701c-1.117-2.89-3.897-4.795-6.995-4.795H107.519c-2.641,0-5.087,1.389-6.44,3.656l-26.732,44.787 H16.867c-3.714,0-6.869,2.719-7.417,6.392l-9.368,62.714c-0.322,2.159,0.312,4.351,1.736,6.004c1.425,1.653,3.499,2.604,5.682,2.604 H35.21c6.231,13.489,19.948,22.502,35.116,22.502c15.162,0,28.872-9.014,35.099-22.502h96.443 c6.229,13.489,19.942,22.502,35.108,22.502c15.161,0,28.871-9.014,35.097-22.502h22.218c4.142,0,7.5-3.358,7.5-7.5v-62.714 C301.789,130.303,298.431,126.945,294.289,126.945z M286.789,189.659h-19.904c-3.342,0-6.281,2.211-7.206,5.422 c-2.899,10.056-12.235,17.08-22.704,17.08c-10.474,0-19.814-7.024-22.716-17.081c-0.926-3.211-3.865-5.421-7.206-5.421H100.238 c-3.342,0-6.281,2.211-7.207,5.422c-2.899,10.056-12.236,17.08-22.705,17.08c-10.477,0-19.821-7.024-22.725-17.082 c-0.926-3.21-3.865-5.42-7.206-5.42H16.204l7.126-47.714h55.274c2.641,0,5.087-1.389,6.44-3.656l26.732-44.787h108.604 l16.878,43.648c1.117,2.89,3.897,4.795,6.995,4.795h42.535V189.659z'/> </g>
            </svg>
            </div>
            <div class='row'>
                <div id='boxWait' class='label'>
                    انقر لإضافة زر جديد
                </div>
            </div>
              <div class='row'>
                <button class='btn' onclick='reset(`resetBox`); event.stopPropagation();'>تصفير</button>
                <div id='trunkCode' class='label'>
                0
                </div>
              </div>
        </div><br><br><br>
    

</div>
</body>
<footer>
    
    <menu class='menu'>
    
        <button class='menu__item' style='--bgColorItem: #ff8c00;'  onclick='goHome()'>
            <svg class='icon' viewBox='0 0 24 24'> <path d='M12,5.69l-7,7V19h5v-6h4v6h5v-6.31l-7-7Z' /> </svg>
            الرئيسية
        </button>
    
        <button class='menu__item active' style='--bgColorItem: #f54888;' onclick='goSettings()'>
            <svg class='icon' viewBox='0 0 24 24'> 
                <path d='M19.14,12.94c.04-.3.06-.61.06-.94c0-.32-.02-.64-.06-.94l2.03-1.58a.49.49,0,0,0,.12-.61l-1.92-3.32a.48.48,0,0,0-.59-.22l-2.39,1c-.5-.38-1.03-.7-1.62-.94L14.4,2.81a.491.491,0,0,0-.49-.44H9.6a.49.49,0,0,0-.49.44L9,5.04c-.59.24-1.13.57-1.62.94l-2.39-1a.48.48,0,0,0-.59.22l-1.92,3.32c-.1.18-.03.39.12.61l2.03,1.58c-.04.3-.06.61-.06.94s.02.64.06.94l-2.03,1.58a.49.49,0,0,0-.12.61l1.92,3.32c.12.21.37.29.59.22l2.39-1c.5.38,1.03.7,1.62.94l.36,2.23a.49.49,0,0,0,.49.44h3.84a.49.49,0,0,0,.49-.44l.36-2.23c.59-.24,1.13-.57,1.62-.94l2.39,1c.22.08.47,0,.59-.22l1.92-3.32a.48.48,0,0,0-.12-.61Z' /> 
            </svg>
        الاعدادات
        </button>

      </menu>

      <script>
        const goSettings=()=>{
            window.open('settings', '_self');
            }
        const goNetwork=()=>{
            window.open('network', '_self');
            }
        const goHome=()=>{
            window.open('index', '_self');
            }

        const setWaitingState=(element,state)=>{
            let el=document.getElementById(element);
            if(state){ el.classList.add('waiting');}
            else{
                if(el.classList.contains('waiting')){
                    el.classList.remove('waiting');
                }
            }
        }
  const reset=(button)=>{
    new Promise((resolve, reject) => {
        const xhr = new XMLHttpRequest();
        const url = 'http://2.2.2.2/'+button;
        xhr.onreadystatechange = () => {
            if (xhr.readyState === 4) {
            if (xhr.status === 200) {
                setTimeout(()=>{
                    updateKeys();
                },500);
            } 
            }
        };
        xhr.open('GET', url, true);
        xhr.timeout = 300; 
        xhr.send();
        })
  }
  const updateKeys=()=>{
    new Promise((resolve, reject) => {
        const xhr = new XMLHttpRequest();
        const url = 'http://2.2.2.2/remoteCodes';
    
        xhr.onreadystatechange = async() => {
          if (xhr.readyState === 4) {
            if (xhr.status === 200) {
              resolve(xhr.responseText);
              document.getElementById('isconnected').style.display = 'none';
              document.getElementById('offCode').innerText=xhr.responseText.split('#')[0];
              document.getElementById('runCode').innerText=xhr.responseText.split('#')[1];
              document.getElementById('openCode').innerText=xhr.responseText.split('#')[2];
              document.getElementById('lockCode').innerText=xhr.responseText.split('#')[3];
              document.getElementById('trunkCode').innerText=xhr.responseText.split('#')[4];
            } else {
                document.getElementById('isconnected').style.display = 'block';
              reject('error');
            }   
          }
        };
        xhr.open('GET', url, true);
        xhr.timeout = 500;
        xhr.send();
      })
      .catch((error) => {
        document.getElementById('isconnected').style.display = 'block';
      });
    }


        const auth=(name)=>{
            setWaitingState(name,true);
            switch(name){
                case 'authPower':
                    document.getElementById('offWait').innerHTML='جار انتضار الضغط على الزر..';
                    break;
                case 'authRun':
                    document.getElementById('runWait').innerHTML='جار انتضار الضغط على الزر..';
                    break;
                case 'authLock':
                    document.getElementById('lockWait').innerHTML='جار انتضار الضغط على الزر..';
                    break;
                case 'authOpen':
                    document.getElementById('openWait').innerHTML='جار انتضار الضغط على الزر..';
                    break;
                case 'authBox':
                    document.getElementById('boxWait').innerHTML='جار انتضار الضغط على الزر..';
                    break;

            }
            try {
                new Promise((resolve, reject) => {
                            const xhr = new XMLHttpRequest();
                            const url = 'http://2.2.2.2/'+name;
                            xhr.onreadystatechange = () => {
                              if (xhr.readyState === 4) {
                                if (xhr.status === 200) {
                                    updateKeys();
                                    setWaitingState('authPower', false);
                                    setWaitingState('authRun', false);
                                    setWaitingState('authLock', false);
                                    setWaitingState('authOpen', false);
                                    setWaitingState('authBox', false);
                                    document.getElementById('offWait').innerHTML = 'انقر لإضافة زر جديد';
                                    document.getElementById('runWait').innerHTML = 'انقر لإضافة زر جديد';
                                    document.getElementById('lockWait').innerHTML = 'انقر لإضافة زر جديد';
                                    document.getElementById('openWait').innerHTML = 'انقر لإضافة زر جديد';
                                    document.getElementById('boxWait').innerHTML = 'انقر لإضافة زر جديد';
                                    document.getElementById('isconnected').style.display = 'none';
                                } else {
                                    setWaitingState('authPower', false);
                                    setWaitingState('authRun', false);
                                    setWaitingState('authLock', false);
                                    setWaitingState('authOpen', false);
                                    setWaitingState('authBox', false);
                                    document.getElementById('offWait').innerHTML = 'انقر لإضافة زر جديد';
                                    document.getElementById('runWait').innerHTML = 'انقر لإضافة زر جديد';
                                    document.getElementById('lockWait').innerHTML = 'انقر لإضافة زر جديد';
                                    document.getElementById('openWait').innerHTML = 'انقر لإضافة زر جديد';
                                    document.getElementById('boxWait').innerHTML = 'انقر لإضافة زر جديد';
                                    document.getElementById('isconnected').style.display = 'block';
                                }
                              }
                            };
                            xhr.open('GET', url, true);
                            xhr.timeout = 7000; // set the timeout to 2 seconds
                            xhr.send();
                          })
                } catch (error) {
                    setWaitingState('powerOn', false);
                    setWaitingState('authPower', false);
                    document.getElementById('offWait').innerHTML='انقر لإضافة زر جديد';
                    setWaitingState('lock', false);
                    setWaitingState('open', false);
                    setWaitingState('trunk', false);
                    document.getElementById('isconnected').style.display = 'block';
                }

            
        }//end auth

        setInterval(()=>{
            updateKeys();
        },300);
      </script>
</footer>
</html>
)rawliteral";

#endif // Remote_H