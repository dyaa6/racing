#ifndef Index_H
#define Index_H

#include <pgmspace.h>

const char index_html[] PROGMEM = R"rawliteral(
   <!DOCTYPE html>
<html lang='ar'>
<head>
    <meta charset='UTF-8'>
    <meta http-equiv='X-UA-Compatible' content='IE=edge'>
    <meta name='viewport' content='minimal-ui,width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0, height=device-height'>
    <title>Racing project</title>
<style>
    *{
    padding: 0px;
    margin: 0px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    user-select: none;
}
:root {
  touch-action: pan-x pan-y;
  height: 100% 
}
*,
.button,
.button * {
-webkit-tap-highlight-color: transparent;
--bgColorMain:#1f1f23;
user-select: none;
}
:root{
	--button_color: #04AA6D;
}

body{
    background: var(--bgColorMain);
    background-size: cover;
    background-position: center;
    background-repeat: repeat;
    font-family: theme-font !important;
    font-size: large;
    --on: #88e661;
    --off: #f0f0f0;
    --transDur: 0.6s;
}








/*alert box*/
#alertContainer{
	width: 100%;
	position: fixed;
	left: 0px;
	bottom: 10vh;
	text-align: center;
	display: flex;
	justify-content: center;
}
.alertBox{
	width: 80%;
	height: 50px;
	line-height: 50px;
	background-color: #fff;
	border:1px solid red;
	border-radius: 10px;
	text-align: center;
	color: red;
	direction: rtl;
}
.row{
  display: flex;
  width: 98%;
  justify-content: space-around;
  margin: 10px;
  border-bottom: 1px solid #333342;
}
.item{
  color:#01fff4;
  font-size: x-large;
}




  





  


/*tap bar*/
html {
    box-sizing: border-box;
    --bgColorMenu : #333342;
    --duration: .7s;    

}




.menu{
    margin: 0;
	  padding: 0px;
    display: flex;
    /* Works well with 100% width  */
    width: 100% 	;
    font-size: large;
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
	height: 50;
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
h2{
  color:#fff;
  width: 100%;
  text-align: center;
}
</style>


    
</head>

<body>


<div id='main-container'>
 <h2>
  Data received from esp32
 </h2>
<div id="data-container">
  
  <div class="row">
    <div class="item">accX: </div>
    <div class="item" id="accX">--</div>
  </div>

  <div class="row">
    <div class="item">accY: </div>
    <div class="item" id="accY">--</div>
  </div>
  
  <div class="row">
    <div class="item">accZ: </div>
    <div class="item" id="accZ">--</div>
  </div>

  <div class="row">
    <div class="item">gyroX: </div>
    <div class="item" id="gyroX">--</div>
  </div>
  
  
  <div class="row">
    <div class="item">gyroY: </div>
    <div class="item" id="gyroY">--</div>
  </div>

  <div class="row">
    <div class="item">gyroZ: </div>
    <div class="item" id="gyroZ">--</div>
  </div>
  
  <div class="row"> </div>

  <div class="row">
    <div class="item">lat: </div>
    <div class="item" id="lat">--</div>
  </div>

  <div class="row">
    <div class="item">lon: </div>
    <div class="item" id="lon">--</div>
  </div>
</div>



<div id='alertContainer'>
<div id='isconnected' class='alertBox'>
    لايوجد اتصال
</div></div>


</div>
</body>
<footer>

    <menu class='menu'>
    
        <button class='menu__item active' style='--bgColorItem: #ff8c00;' >
            <svg class='icon' viewBox='0 0 24 24'> <path d='M12,5.69l-7,7V19h5v-6h4v6h5v-6.31l-7-7Z' /> </svg>
            الرئيسية
        </button>
    
        <button class='menu__item' style='--bgColorItem: #f54888;' onclick='goSettings()'>
            <svg class='icon' viewBox='0 0 24 24'> 
                <path d='M19.14,12.94c.04-.3.06-.61.06-.94c0-.32-.02-.64-.06-.94l2.03-1.58a.49.49,0,0,0,.12-.61l-1.92-3.32a.48.48,0,0,0-.59-.22l-2.39,1c-.5-.38-1.03-.7-1.62-.94L14.4,2.81a.491.491,0,0,0-.49-.44H9.6a.49.49,0,0,0-.49.44L9,5.04c-.59.24-1.13.57-1.62.94l-2.39-1a.48.48,0,0,0-.59.22l-1.92,3.32c-.1.18-.03.39.12.61l2.03,1.58c-.04.3-.06.61-.06.94s.02.64.06.94l-2.03,1.58a.49.49,0,0,0-.12.61l1.92,3.32c.12.21.37.29.59.22l2.39-1c.5.38,1.03.7,1.62.94l.36,2.23a.49.49,0,0,0,.49.44h3.84a.49.49,0,0,0,.49-.44l.36-2.23c.59-.24,1.13-.57,1.62-.94l2.39,1c.22.08.47,0,.59-.22l1.92-3.32a.48.48,0,0,0-.12-.61Z' /> 
            </svg>
        الاعدادات
        </button>
    
      
    
    
      
    
    
      </menu>
    
   

</footer>

<script>

setInterval(()=>{
        new Promise((resolve, reject) => {
            const xhr = new XMLHttpRequest();
            const url = 'http://192.168.4.1/state';
            xhr.onreadystatechange = async() => {
              if (xhr.readyState === 4) {
                if (xhr.status === 200) {
                  resolve(xhr.responseText);
                  document.getElementById('isconnected').style.display='none';
                  document.getElementById('accX').innerHTML=xhr.responseText.split('#')[0];
                  document.getElementById('accY').innerHTML=xhr.responseText.split('#')[1];
                  document.getElementById('accZ').innerHTML=xhr.responseText.split('#')[2];
                  document.getElementById('gyroX').innerHTML=xhr.responseText.split('#')[3];
                  document.getElementById('gyroY').innerHTML=xhr.responseText.split('#')[4];
                  document.getElementById('gyroZ').innerHTML=xhr.responseText.split('#')[5];
                  document.getElementById('lat').innerHTML=xhr.responseText.split('#')[6];
                  document.getElementById('lon').innerHTML=xhr.responseText.split('#')[7];                  
                } else {
                    document.getElementById('isconnected').style.display='block'; // no connection
                }
              }
            };
        
            xhr.open('GET', url, true);
            xhr.timeout = 400; // set the timeout to 2 seconds
            xhr.send();
          });
  }, 200);


const goSettings=()=>{
  window.open('settings.html', '_self');
}

// document.getElementById('open_btn').addEventListener('contextmenu', function(e) {
//   e.preventDefault();
// });

document.addEventListener('touchmove', function (event) {// to prevent the zoom
  if (event.scale !== 1) { event.preventDefault(); }
}, { passive: false });
    </script>
</html>
)rawliteral";

#endif // Index_H
