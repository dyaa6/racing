String timer=R"(
  <!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8' name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Settings</title>
    <style>
        
  /*tap bar */
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
.row{
    width: 100%;
    display: flex;
    justify-content: space-around;
    font-size: larger;
    padding-inline-start: 40px;
    padding-inline-end: 40px;
    flex-direction: row-reverse;
    align-items: baseline;
    padding: 0px;
}
.row-container{
    display: flex;
    flex-direction: column;
    align-items: center;
    width: 100%;
}
.row1{
    width: 100%;
    align-items:center;
}
.row2{
    padding: 15px;
    padding-inline-start: 40px;
    padding-inline-end: 40px;
    justify-content: space-around;
    width: 60%;
}

#msg{
  color:red;
  margin:10px;
}
.menu{
  margin: 0;
  padding: 0px;
  display: flex;
  width: 100%;
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
input[type='number']{
    width: 70px !important;
    height: 35px;
    border: 1px double #28a136;
    border-radius: 20px;
    background: #fff;
    font-size: large;
    text-align: center;
    color: #000;
}
#save{
  width: 190px;
  height: 40px;
  border-radius: 15px;
  border-color: none;
  font-size: large;
  background-color: #fff;
}
#save:active{
  background-color: #01fff4;
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

}

/*switch*/
input[type='checkbox'] { 
  position: absolute;
  opacity: 0;
}

/* Normal Track */
input[type='checkbox'].ios-switch + div {
  vertical-align: middle;
  width: 40px;  height: 20px;
  border: 1px solid rgba(0,0,0,0.4);
  border-radius: 999px;
  background-color: rgba(0, 0, 0, 0.1);
  transition-duration: .4s;
  transition-property: background-color, box-shadow;
  box-shadow: inset 0 0 0 0px rgba(0,0,0,0.4);
  margin: 15px 1.2em 15px 2.5em;
}

/* Checked Track (Blue) */
input[type='checkbox'].ios-switch:checked + div {
  width: 40px;
  background-position: 0 0;
  background-color: #3b89ec;
  border: 1px solid #0e62cd;
  box-shadow: inset 0 0 0 10px rgba(59,137,259,1);
}

/* Tiny Track */
input[type='checkbox'].tinyswitch.ios-switch + div {
  width: 34px;  height: 18px;
}

/* Big Track */
input[type='checkbox'].bigswitch.ios-switch + div {
  width: 50px;  height: 25px;
}

/* Green Track */
input[type='checkbox'].green.ios-switch:checked + div {
  background-color: #00e359;
  border: 1px solid rgba(0, 162, 63,1);
  box-shadow: inset 0 0 0 10px rgba(0,227,89,1);
}

/* Normal Knob */
input[type='checkbox'].ios-switch + div > div {
  float: left;
  width: 18px; height: 18px;
  border-radius: inherit;
  background: #ffffff;

  -webkit-transition-timing-function: cubic-bezier(.54,1.85,.5,1);
  -webkit-transition-duration: 0.4s;
  -webkit-transition-property: transform, background-color, box-shadow;
  -moz-transition-timing-function: cubic-bezier(.54,1.85,.5,1);
  -moz-transition-duration: 0.4s;
  -moz-transition-property: transform, background-color;
  box-shadow: 0px 2px 5px rgba(0, 0, 0, 0.3), 0px 0px 0 1px rgba(0, 0, 0, 0.4);
  pointer-events: none;
  margin-top: 1px;
  margin-left: 1px;
}

/* Checked Knob (Blue Style) */
input[type='checkbox'].ios-switch:checked + div > div {
  -webkit-transform: translate3d(20px, 0, 0);
  -moz-transform: translate3d(20px, 0, 0);
  background-color: #ffffff;
  box-shadow: 0px 2px 5px rgba(0, 0, 0, 0.3), 0px 0px 0 1px rgba(8, 80, 172,1);
}

/* Tiny Knob */
input[type='checkbox'].tinyswitch.ios-switch + div > div {
  width: 16px; height: 16px;
  margin-top: 1px;
}

/* Checked Tiny Knob (Blue Style) */
input[type='checkbox'].tinyswitch.ios-switch:checked + div > div {
  -webkit-transform: translate3d(16px, 0, 0);
  -moz-transform: translate3d(16px, 0, 0);
  box-shadow: 0px 2px 5px rgba(0, 0, 0, 0.3), 0px 0px 0 1px rgba(8, 80, 172,1);
}

/* Big Knob */
input[type='checkbox'].bigswitch.ios-switch + div > div {
  width: 23px; height: 23px;
  margin-top: 1px;
}

/* Checked Big Knob (Blue Style) */
input[type='checkbox'].bigswitch.ios-switch:checked + div > div {
  -webkit-transform: translate3d(25px, 0, 0);
  -moz-transform: translate3d(16px, 0, 0);
  box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.3), 0px 0px 0 1px rgba(8, 80, 172,1);
}

/* Green Knob */
input[type='checkbox'].green.ios-switch:checked + div > div {
  box-shadow: 0px 2px 5px rgba(0, 0, 0, 0.3), 0 0 0 1px rgba(0, 162, 63,1);
}



    </style>
</head>
<body>
    <h2 id='title'>
        فصل الجهاز بعد مدة
    </h2>
<div class='row-container'>
<div class='row row1'>
        فصل الجهاز بعد مدة معينة
    <div>
        <label><input type='checkbox' id='sw1' class='ios-switch green  bigswitch' onchange='changeState()' checked /><div><div></div></div></label>
    </div>
</div>

<div class='row row2'>
    <div class='item'>فصل بعد</div>
    <div class='item'><input type='number' value='5' id='timeV'/></div>
    <div class='item'>دقائق</div>
</div>
    <div class='row' id='msg'>
      لايوجد اتصال
    </div>
    <button id='save' onclick='setTimer()'>
        حفظ
    </button>
</div>
<br>


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

        const goHome=()=>{
            window.open('index.html', '_self');
            }
    const switch1=document.getElementById('sw1'); //global
const changeState=()=>{
  document.getElementById('msg').innerHTML='';
  if(switch1.checked){
    document.getElementsByClassName('row2')[0].style.display='flex';
  }
  else{
    document.getElementsByClassName('row2')[0].style.display='none';
  }
}

const setTimer= ()=>{
    const timeValue=document.getElementById('timeV').value;
    try {
      //setResponseText(response);
      if ((timeValue === '' || timeValue === 0) && switch1.checked) {
        document.getElementById('msg').innerHTML=('يجب أن تدخل قيمة');
        document.getElementById('msg').style.color='orange';
        return;
      }
      else{
        if(switch1.checked){
        new Promise((resolve, reject) => {
          const xhr = new XMLHttpRequest();
          const url = 'http://192.168.4.1/setTimer?value='+timeValue*60+'&state=on';
          xhr.onreadystatechange = () => {
            if (xhr.readyState === 4) {
              if (xhr.status === 200) {
                  if(xhr.responseText=='Timer enabled'){
                    document.getElementById('msg').innerHTML=('تم حفظ التغييرات');
                    document.getElementById('msg').style.color='green';
                  }
              } else {
                document.getElementById('msg').innerHTML=('لايوجد اتصال بالسيارة');
                document.getElementById('msg').style.color='red';
              }
            }
          };
          xhr.open('GET', url, true);
          xhr.timeout = 500; 
          xhr.send();
        });
        
      }
        else{//not enabled
          new Promise((resolve, reject) => {
            const xhr = new XMLHttpRequest();
            const url = 'http://192.168.4.1/setTimer?value=0&state=off';
            xhr.onreadystatechange = () => {
              if (xhr.readyState === 4) {
                if (xhr.status === 200) {
                    if(xhr.responseText=='Timer enabled'){
                      document.getElementById('msg').innerHTML=('تم حفظ التغييرات');
                      document.getElementById('msg').style.color='green';
                    }
                } else {
                  document.getElementById('msg').innerHTML=('لايوجد اتصال بالسيارة');
                  document.getElementById('msg').style.color='red';
                }
              }
            };
            xhr.open('GET', url, true);
            xhr.timeout = 500; 
            xhr.send();
          });
        }
      }
      } catch (error) {
                  document.getElementById('msg').innerHTML=('لايوجد اتصال بالسيارة');
                  document.getElementById('msg').style.color='red';
    }
}
      </script>
</footer>
</html>
)";