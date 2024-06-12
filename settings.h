#ifndef Set_H
#define Set_H

#include <pgmspace.h>

const char settings[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
<html lang='en'>
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
.item{
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
    display: flex;
    justify-content: space-between;
    opacity: 1;
    transition: all 0.5s;
}
.item:active{
    opacity: 0.4;
}
.item:hover{
    background-color: #47475e;
    transform: scale(1.07);
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
الإعدادات
    </h2>

    <div class='item-container'>
        <div class='item' onclick='goNetwork()'>
             إعدادات الشبكة
             <svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' width='50' height='50' viewBox='0 0 50 50'>
                <image y='4' width='45' height='36' xlink:href='data:img/png;base64,iVBORw0KGgoAAAANSUhEUgAAADIAAAApCAYAAAB3LnrpAAAD6klEQVRogcWZ32tUVxDHP6Y2JlbBBqyktmBsbCWGaIr0QQUfIj70lz8atBVUEB98C/4JguS1lIhPNiBCilUULBSKyUNNmpaGllI0banRLE3UFIMSattt1a8cmZTtde+95+49G78wsLvnnJnv3HtmzszZeZIIiGeA5cBS4HlTWwvUAH8Dztg0cAeYAB6EMj0/gI5lwCagBVgJvFLiDObcPOC+fb8FTAFXgWvAKDBkv1WMSt/IG8C7wBZgHbAg58MoAj8A/cAF4NusCrI48gKwB9gLvJ6ZajZ8D5wC+oDfvVY6R1KkSVKPpHuae9wz201pPJMG6yQdlVTMQf9hzOesKBqXuji+cVvrHeBDC1wf/An8CAwDvwHXgRuWnZ619f9aJnsRaAJeBjYAbcBCTztjwGHgs7St5Tzu9Xxak5KOSdopqcFji8ZJg+lwuiY9bfdG306p8rWSRj2U9Et6X1JtDvJxUmu6+z14jBrn/znylkcsfCFpaxXIx8lWs5mEonF/7MjhlMljkrZXQKRe0uKI1Feg5z3jkIQuF+zfJZwLJ4AuC+YkLLTAdfIqsBpYYeVJKf4BxoGfgZ+Ab4CvPfV/BByMGR9xHj8n6VLEW5cq93m+fhd40x57Og7TpsNn2+7Tk2nccV80O6FG0pAN3Ja0PkXhfkkjOcjHYcR0J9lebxxlnGuiWctljI8ltSco6ZA0XAUHohg2W3E82o3rf5kzS9D1zIEDUfT48vMpGl3QnraKNw2XgQHgF+BKmYLPFZ5rgNeADqDVQ6erhHdbkohHiqfbJE2lPLWCpG5JrRWk1lZbW0ix4Ti8naQrzdBAgnKXbboCnfC1pisp+13M48iSMqnZ4YykxgAORKXRdEdxybhU7AiW3gZLFB/KQMyd5stMFmdYd6jE3uBsis0b7Fhv/ylw1rq2OLgSfYf18K5aaIyU8Tet+3M9+nkr9ePgutFOYFdJv19xsPtKm6STGZuwoq1pC8EhhBPdGcjHoftpOtIeiZ28cLqSqopEqfEJkIQ9vMlj3kOTNDhdeyolk+emsQ7oBT4oM3YO+BL4Crhrvy0BNgKbgZ1l1nwCHLAbyewIECN9JVvqQmn7mSBrbe4s+vLyCHX3+znwqzVhWeCapVXAm3kJhL7EfmoIcYkdh5dsz6+w8XGLqYmqWAt0IEalU9JMmXQ8Y2PBbVZja7VYL5KENfZ3QjDkOUficDTQnEwI/UbqrStclDLvD+sW/wplOPQbafZwApvTHNJwaEeyPOFgb4MqOOL+Eyx4zCvY3GAI7YgrDo94zDviWUj6o0rnyPGEkv54NWxWyxEne+0K1B2CTtxn91t4WxKPALu6R8SZb/JWAAAAAElFTkSuQmCC'/>
              </svg>
              
        </div>
        <div class='item' onclick='goRemote()'>
            ريموت كنترول
            <svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' width='55' height='55' viewBox='0 0 55 55'>
                <image width='45' height='45' xlink:href='data:img/png;base64,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'/>
              </svg>
        </div>
        <div class='item' onclick='goTimer()'>
            فصل الجهاز بعد مدة
            <svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' width='55' height='55' viewBox='0 0 55 55'>
                <image x='7' y='2' width='36' height='44' xlink:href='data:img/png;base64,iVBORw0KGgoAAAANSUhEUgAAACkAAAAxCAYAAABH5YAAAAAC2UlEQVRYhdWZi43bMAyG2eAGSCeoO0HdCc7dwBvUneDSCepMEHSCQydIN3AyQbKB2wnsTsCCrgS4NPWgovhwPyBccLbEzyIli/QbRIRElQDwBABFRPcRAPYAcE0xlQpJYBcA2Cr6EOhHAPilNbbRdjAqlIBg7o+Z9YVSIdWzYTSmdHpINEaQXwDgs6LP97VjclWluntVpbrbirahCgDemd9c5N7fAHBKdfUkcreylYj4jIgD6jSYfqXWpubmChE7JZhLnRlPstEiYq2F3CLiMRMc19GMT3Yadq21DKHVTfF2jNy4r2wf3DrilGs029M39n+K40+hmORPxtUj4sHhNu7Cg7lfO8ted/sAe3M9ZdE1kbDDfIFJA9Wezm0iHG9tLKAEWTi2liHCrdpWeWwVPkhpi1k8WcCdrSIcSgdo54J0xWEsIHdhbGiUDruNBCkF9E7hQu6FTtF3J9juOaQ0ixojt0K6Qm2aTXsKehI22X3ERpxTkr2Ja2OO9PzNcDJtTUk2iasgyFoA+bEyoM9uTZCPwoWfKwBJkuw+boQM7pSaMGXQKLh8cjePx/QTdB5x+6WU4/x5YciF/VeRiOWE5AWDbHGdE3I/A6W/X3MN/GCeeJ4efEgci8DeZ2Di9seNtJoyGLpFi91GgixSq18ZJNmeIM/C2M0LQUp2zzalHVhc5oovrXo2k7Re3trVzd+ZdONuZcCd4Op/XJ4j/DCrLty7bR25TsnTB+lkrD1dpzav7fmghXAjmkrYPQGfHXYLCVLK+O4N6gL8L9OMTYiQVcBubb5K3SLEJGM0wMUxQM9rhwmt9tSDLtJEuIz4QO3TaotWTaAIKwJioD5Jm3sXeJfb4/7ZcaIvTQ5VBWqcV1OLlI93ETNw8Dx9Dh1CDLGuqhKKoCH1sZU6beA3gViN0UUbz6lfxErzya6KPH9SzFHsUvKvzkZzfbYjULsw6JBg0whaCLelyADwF+ySvlYwfpuAAAAAAElFTkSuQmCC'/>
              </svg>
        </div>
        <div class='item' onclick='goOnePress()'>
            تشغيل بضغطة واحدة
            <svg width='50px' height='50px' viewBox='0 0 24 24' id='Layer_1' data-name='Layer 1' xmlns='http://www.w3.org/2000/svg' fill='#ffffff' stroke='#ffffff'>

                <g id='SVGRepo_bgCarrier' stroke-width='0'/>
                
                <g id='SVGRepo_tracerCarrier' stroke-linecap='round' stroke-linejoin='round'/>
                
                <g id='SVGRepo_iconCarrier'>
                
                <defs>
                
                <style>.cls-1{fill:none;stroke:#ffffff;stroke-miterlimit:10;stroke-width:1.488;}</style>
                
                </defs>
                
                <path class='cls-1' d='M10.12,22.31l-5-5a2,2,0,0,1-.58-1.41,2,2,0,0,1,.58-1.4,2,2,0,0,1,1.41-.59,2,2,0,0,1,1.4.59l1.3,1.29V6.48a1.94,1.94,0,0,1,1.71-2,1.89,1.89,0,0,1,2,1.86V12l5,.71a1.87,1.87,0,0,1,1.61,1.85h0a16.9,16.9,0,0,1-1.79,7.58l-.09.17'/>
                
                <path class='cls-1' d='M12.94,10.67a4.55,4.55,0,0,0,1.44-1,4.69,4.69,0,1,0-6.64,0,4.59,4.59,0,0,0,1.45,1'/>
                
                </g>
                
                </svg>
        </div>
        <div class='item' onclick='goUpdate()'>
            تحديث نظام الجهاز
            <svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' width='50' height='50' viewBox='0 0 50 50'>
                <image width='46' height='50' xlink:href='data:img/png;base64,iVBORw0KGgoAAAANSUhEUgAAAC4AAAAyCAYAAAAjrenXAAACbklEQVRoge2av04cMRDGvzmuoMwjQJE2XB0liKt4nHsDCE8S5REoUl8K0lDRUR6iRuJ6Al/ki+/fyt71jO1dIeWTViet7fFv52zveNZCEpUlAEa+izcARTosBi4iqvq5/ZrBU0BJfgRwDuAQwE8RuY/U0wO4RqmX0u4JySeSb/ynZ5KfSrElQWtFcuJBm3L3JmqDgQeJglqHUAv0WlnwwafI8XIidDn4AaDLwFce007f/VUWPhe8C3qnXln4PqCrwFvBtdDF4S3gVuii8FrwXOhi8BrwUtBF4IeCzoZPAa8FnQU/NLQZnmQ0sO4L2gRPchQx0iu0Gj7k8aGgNfCrrdvuNswXzAF8iNj9FbopIlMj5DxSdBa5vwRwGgJfADjSAoh2t7ztzxJH34XATQF5z+BPoYl5ZwHoWY8hj7thctkxXCbNOVDQ48sO5z0AuLDueObN6a42srXVVGyy7im4hr8H/QfvW+NS/ZH81if72HeqzrYGdFmBLyrrULmuyJRke5Nm1nicpFvDFy3xjFVuDT8WkWVX+43HNUuxNzyNBVxGOVvTFGg0E/vWcU4yFsklSUTUDtgb48a3aBa02UZmJmsRCfgtWmRlshKTQkfKlHKqnn2Q1ynrcjirsKLA25ylVLSC538KybRd7JUP4MrYzvbGNY7xwePxkkFW9W/ru4olg7raPVRkSrJtnZw/jO1SlBbAWb9ztmSbcpSUJVuF4THIxEMGXwB8BfDZH+9wxzpeAbz4Kgd+Ho12jnu43z++jvjyWwC/ReSmC3jDlwNeW23/vPXYx/rwzPpyRpyn3W/IoKvvrlj5uk5b+VYA/gIm4iEK/V/SzAAAAABJRU5ErkJggg=='/>
              </svg>
              
        </div>

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
            window.open('/', '_self');
            }
        const goRemote=()=>{
            window.open('remote', '_self');
            }
        const goUpdate=()=>{
            window.open('update_p', '_self');
            }
        const goTimer=()=>{
            window.open('timer', '_self');
            }
        const goOnePress=()=>{
            window.open('onepress', '_self');
            }
      </script>
</footer>
</html>
)rawliteral";

#endif // Set_H