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
            window.open('settings.html', '_self');
            }
        const goNetwork=()=>{
            window.open('network.html', '_self');
            }
        const goHome=()=>{
            window.open('/', '_self');
            }

        const goUpdate=()=>{
            window.open('update_p.html', '_self');
            }

      </script>
</footer>
</html>
)rawliteral";

#endif // Set_H