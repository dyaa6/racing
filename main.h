#ifndef Index_H
#define Index_H

#include <pgmspace.h>

const char index_html[] PROGMEM = R"rawliteral(
 <!DOCTYPE html>
<html lang='ar'>
<head>
    <meta charset='UTF-8'>
    <meta http-equiv='X-UA-Compatible' content='IE=edge'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>My Car</title>
<style>
    *{
    padding: 0px;
    margin: 0px;
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
@font-face {
	font-family: theme-font;
	src: url(../fonts/font.ttf);
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

#title_bar{
  height: 56px;
  text-align: center;
  line-height: 50px;
  background: #208371;
}
#container{
  display:flex;
  flex-direction: column;
  align-items: center;
}



#lock-container{
    display:flex;
    flex-direction: column;
    flex-wrap: nowrap;
    align-items: baseline;
    justify-content:center;
    align-items: center;
    padding-bottom: 20px;
    width: 100%;
}
#run-button-container{
	display:flex;
	flex-direction: row;
	align-items: baseline;
	justify-content:center;
	padding-bottom: 20px;
}
.row1{
  display: flex;
  margin: 0px;
  width: 80%;
}
.row2{
  display: flex;
  margin: 0px;
  width: 80%;
}
#lock_btn{
  border-top-left-radius: 15px;
  user-select: none;
}
#jam_btn{
  border-bottom-right-radius: 15px;
}
#open_btn{
  border-top-right-radius: 15px;
}
#trunk_btn{
  border-bottom-left-radius: 15px;
}




/*signs*/
#run_sign{
	width: 70px;
	height: 70px;
	background-color: rgb(60, 56, 56);
	border: 2px solid #000;
	border-radius: 100px;
	margin: 6px;
	/*box-shadow:0px 0px 30px rgb(9, 243, 64) ;*/
}
.signs_container{
	display: flex;
	flex-direction: row;

	list-style-type: none;
}
.sign{
	width: 80px;
	height: 20px;
	background-color: #3c3838;
	border: 1px solid #000;
}
#sign1{
	border-start-start-radius: 10px;
}
#sign3{
	border-end-end-radius: 10px;
}

#wrong-password{
	display: block;
	color: red;
	direction: rtl;
	text-align: right;
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
#isconnected{
	display: none;
}

.pbutton{
	background-color: #4a4949;
	padding: 15px;
	margin: .5px;
  margin-top: 0px;
	color:#fff;
	transition: all 0.4s;
  width: 100%;
  height: 40px;
  display: flex;
  justify-content: center;
  align-items: center;
  user-select: none;
}
.pbutton:active{
	background-color: orange;
	transform: scale(1.07);
}

.power-btn{
  width: 80px;
  height: 80px;
  display: flex;
  justify-content: center;
  align-items: center;
  margin-top: 20px;
  transition: all 0.2s;
}
.power-btn:active{
  transform: scale(1.07);
}
/*switch*/


@property --color {
	syntax: '<color>';
	inherits: true;
	initial-value: red;
  }
  @property --light-on {
	syntax: '<color>';
	inherits: true;
	initial-value: #0001;
  }
  @property --light-off {
	syntax: '<color>';
	inherits: true;
	initial-value: #000d;
  }
  
  @keyframes squeeze {
	0%, 100% { width: 0.95em; aspect-ratio: 1;  }
	50% { width: 1.35em; aspect-ratio: 1.75; }
  }
  @keyframes squeeze2 {
	0%, 100% { width: 0.95em; aspect-ratio: 1;  }
	50% { width: 1.35em; aspect-ratio: 1.75; }
  }
  
  .ellastic {
	--on: #af5;
	--off: #d55;
	--duration: 0.3s;
	--color: var(--off);
	--color-semi: color-mix(in srgb, var(--color) 50%, #0000);
	--color-semi2: color-mix(in srgb, var(--color) 33%, #0000);
	--color-semi3: color-mix(in srgb, var(--color) 15%, #fff0);
	--light-on: #0001;
	--light-off: #000d;
	appearance: none;
	font-size: 3em;
	width: 2.5em;
	aspect-ratio: 2.5;
	border-radius: 3em; /*change the button size from here*/
	margin:20px;
	border: 1px solid #0000;
	position: relative;
	color: var(--color);
	filter: 
	  drop-shadow(0 0.025em 0.0125em #fff3)
	  drop-shadow(0 -0.025em 0.0125em #000c)
	  ;
	transition: 
	  --color var(--duration) linear,
	  --light-on calc(var(--duration) / 2),
	  --light-off calc(var(--duration) / 2),
	  color var(--duration) linear;
	atransition-delay: 0s, calc(var(--duration) / 1), 0s, 0s;
  
	&::after {
	  content: '';
	  position: absolute;
	  top: 50%;
	  left: 0.5em;
	  transform: translate(-50%, -50%);
	  width: 0.95em;
	  aspect-ratio: 1;
	  border-radius: 5em;
	  border: 1px solid #0000;
	  background: 
		radial-gradient(at 50% 0, var(--color-semi), #0000 40%),
		radial-gradient(at 50% 100%, var(--color-semi), #0000 40%),
		radial-gradient(50% 50% at 50% 0, #fff2, #0000 90%),
		radial-gradient(50% 50% at 50% 110%, #0007, #0000 90%),
		radial-gradient(circle, #0000 0.12em, var(--color-semi3) 0.125em, #0000 0.5em),
		radial-gradient(circle, #0000 0.12em, var(--color-semi2) 0.125em, #0000 0.4em),
		radial-gradient(circle, #0000 0.12em, #fff1 0.125em, #fff0 0.25em),
		radial-gradient(circle, #0000 0.12em, #2f2f33 0.125em);
	  ;
	  background-repeat: no-repeat;
	  box-shadow: 
		inset 0 0 0.125em var(--color-semi),
		0 0 0.2em #0005,
		0 0.15em 0.1em #0008;
	  transition: left var(--duration) linear;
	  animation: squeeze var(--duration) 1 linear forwards;
	}
  
	&::before {
	  content: '';
	  position: absolute;
	  top: -1px;
	  left: -1px;
	  width: calc(100% + 2px);
	  height: calc(100% + 2px);
	  -webkit-mask:
		radial-gradient(45% 75% at 50% -50%, #0000 99%, #000) 50% 0 / 2em 0.2em no-repeat,
		radial-gradient(45% 75% at 50% 150%, #0000 99%, #000) 50% 100% / 2em 0.2em no-repeat,
		linear-gradient(90deg, #000 0.55em, #0000 0 1.95em, #000 0),
		linear-gradient(#000 0 0) 50% 50% / 2em 0.75em no-repeat
		;
	  border-radius: 3em;
	  background: 
		/* left light */
		radial-gradient(circle at 0.505em 50%, #0004, var(--light-on) 0.11em, #0000 0.12em),
		radial-gradient(50% 100% at 0.505em 45%, #fff9 0.025em, #fff0 0.08em),
  
		radial-gradient(circle at 0.505em 50%, #0000 0.025em, #0008 0.12em, #0000 0.13em),
		radial-gradient(circle at 0.505em 50%, var(--off) 0.11em, #0000 0.12em),
		radial-gradient(circle at 0.505em 50%, #000 0.10em, #0000 0.16em),
		radial-gradient(circle at 0.505em 49%, #000 0.12em, #fff0 0.13em),
		radial-gradient(circle at 0.505em 51%, #fff7 0.12em, #fff0 0.13em),
		/* right light */
		radial-gradient(circle at 2.005em 50%, #0004, var(--light-off) 0.11em, #0000 0.12em),
		radial-gradient(50% 100% at 2.005em 45%, #fff9 0.025em, #fff0 0.08em),
		radial-gradient(circle at 2.005em 50%, #0000 0.025em, #0008 0.12em, #0000 0.13em),
		radial-gradient(circle at 2.005em 50%, var(--on) 0.11em, #0000 0.12em),
		radial-gradient(circle at 2.005em 50%, #000 0.10em, #0000 0.16em),
		radial-gradient(circle at 2.005em 49%, #000 0.12em, #fff0 0.13em),
		radial-gradient(circle at 2.005em 51%, #fff7 0.12em, #fff0 0.13em),
		/* background base */
		radial-gradient(45% 100% at 50% 130%, #0007 50%, #0000) 50% 100% / 2em 0.2em no-repeat,
		radial-gradient(45% 100% at 50% -30%, #0007 50%, #0000) 50% 0% / 2em 0.2em no-repeat,
		#0c0c10a0;
	  box-shadow:
		inset 0 0 0.1em 0.005em #000,
		inset 0 0 0.25em #0007
	}
  
	&:checked {
	  --color: var(--on); 
	  --light-on: #000d;
	  --light-off: #0001;
	  atransition-delay: 0s, 0s, calc(var(--duration) / 1), 0s;
  
	  &::after {
		left: 2em;
		animation: squeeze2 var(--duration) 1 linear forwards;
	  }
	}
  
	&[disabled] {
	  --on: #aaa;
	  --off: #aaa;
	  &, &::before, &::after {
		transition-duration: 0s !important;
		animation-duration: 0s !important;
	  }
	}
  }
  
  @media print {
	.ellastic {
	  &, &::before, &::after {
		-webkit-print-color-adjust: exact;
		print-color-adjust: exact;
	  }
	}
  }
  
  @media (prefers-reduced-motion) {
	.ellastic {
	  &, &::before, &::after {
		transition-duration: 0s !important;
		animation-duration: 0s !important;
	  }
	}
  }


#timer{
  width: 100%;
  text-align: center;
  font-size: x-large;
  color: #fff;
  margin:0 px;
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
</style>


    
</head>

<body>


<div id='main-container'>
 
<div id='container'>
    <div id='run_sign'></div>
    <ul class='signs_container'>
        <li id='sign1' class='sign'></li>
        <!-- <li id='sign2' class='sign'></li> -->
        <li id='sign3' class='sign'></li>
    </ul>
    <input type='checkbox' role='switch' id='toggle1' class='ellastic' />


</div>
<div id='run-button-container'>

    <div class='power-btn' id='run_btn'>
            <img src='image-R.png' style='width: 130px;height: 130px;' id='image' alt='تشغيل'/>
      </div>



</div>
<div id='lock-container'>
<div class='row1'>
    <div class='pbutton' id='lock_btn'>
        <a class='butt'>
            <svg fill='#fff' width='37px' height='37px' viewBox='-13.39 0 122.88 122.88' version='1.1' id='Layer_1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' enable-background='new 0 0 96.108 122.88' xml:space='preserve' stroke='#999'>
                <g id='SVGRepo_bgCarrier' stroke-width='0'/>
                <g id='SVGRepo_tracerCarrier' stroke-linecap='round' stroke-linejoin='round'/>
                <g id='SVGRepo_iconCarrier'> <g> <path fill-rule='evenodd' clip-rule='evenodd' d='M2.892,56.036h8.959v-1.075V37.117c0-10.205,4.177-19.484,10.898-26.207v-0.009 C29.473,4.177,38.754,0,48.966,0C59.17,0,68.449,4.177,75.173,10.901l0.01,0.009c6.721,6.723,10.898,16.002,10.898,26.207v17.844 v1.075h7.136c1.59,0,2.892,1.302,2.892,2.891v61.062c0,1.589-1.302,2.891-2.892,2.891H2.892c-1.59,0-2.892-1.302-2.892-2.891 V58.927C0,57.338,1.302,56.036,2.892,56.036L2.892,56.036z M26.271,56.036h45.387v-1.075V36.911c0-6.24-2.554-11.917-6.662-16.03 l-0.005,0.004c-4.111-4.114-9.787-6.669-16.025-6.669c-6.241,0-11.917,2.554-16.033,6.665c-4.109,4.113-6.662,9.79-6.662,16.03 v18.051V56.036L26.271,56.036z M49.149,89.448l4.581,21.139l-12.557,0.053l3.685-21.423c-3.431-1.1-5.918-4.315-5.918-8.111 c0-4.701,3.81-8.511,8.513-8.511c4.698,0,8.511,3.81,8.511,8.511C55.964,85.226,53.036,88.663,49.149,89.448L49.149,89.448z'/> </g> </g>
            </svg>
        </a>
      </div>
      <div class='pbutton' id='open_btn'>
          <a class='butt'>
            <svg fill='#fff' width='37px' height='37px' viewBox='0 -6.61 122.88 122.88' version='1.1' id='Layer_1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' enable-background='new 0 0 122.88 109.652' xml:space='preserve' stroke='#999'>
                <g id='SVGRepo_bgCarrier' stroke-width='0'/>
                <g id='SVGRepo_tracerCarrier' stroke-linecap='round' stroke-linejoin='round'/>
                <g id='SVGRepo_iconCarrier'> <g> <path fill-rule='evenodd' clip-rule='evenodd' d='M2.585,49.871H54.77V34.054v-0.011h0.009c0.002-9.368,3.828-17.878,9.989-24.042 c6.164-6.163,14.679-9.991,24.051-9.991V0h0.005l0,0h0.012v0.009c9.368,0.002,17.878,3.828,24.042,9.989 c6.164,6.164,9.991,14.679,9.991,24.051h0.012v0.004v15.96v2.403h-2.403h-9.811h-2.404v-2.403V33.868v-0.009h0.012 c-0.002-5.332-2.195-10.189-5.722-13.715c-3.528-3.531-8.388-5.721-13.724-5.724v0.009h-0.005l0,0h-0.011V14.42 c-5.334,0.002-10.191,2.19-13.72,5.717l0.005,0.005c-3.529,3.528-5.722,8.388-5.722,13.722h0.009v0.005v16.003h13.987 c1.422,0,2.585,1.164,2.585,2.585v54.613c0,1.422-1.163,2.583-2.585,2.583H2.585c-1.424,0-2.585-1.161-2.585-2.583V52.456 C0,51.035,1.161,49.871,2.585,49.871L2.585,49.871z M43.957,79.753l4.098,18.908l-11.232,0.045l3.297-19.162 c-3.068-0.981-5.295-3.857-5.295-7.252c0-4.202,3.411-7.613,7.614-7.613c4.202,0,7.613,3.411,7.613,7.613 C50.053,75.975,47.433,79.048,43.957,79.753L43.957,79.753z'/> </g> </g>
            </svg>
          </a>
        </div>
      </div> <!-- .row1 end -->
      <div class='row2'>
        <div class='pbutton' id='trunk_btn'>
          <a class='butt'>
            <svg xmlns='http://www.w3.org/2000/svg' fill='#fff' width='57px' height='57px' style='margin:-10px; margin-top:7px' xmlns:xlink='http://www.w3.org/1999/xlink' version='1.1' x='0px' y='0px' viewBox='0 0 100 125' style='enable-background:new 0 0 100 100;' xml:space='preserve'><title>kunci mercy</title><path d='M89.6,64.2l-1.7-16.5c-0.7-5.3-4.8-9.6-10.1-10.6l-20.5-3.2c-0.1,0-0.3-0.1-0.4-0.1c3.2-2.7,8.6-5.8,17.3-5.8V21  c-12.1,0-19.4,5-23.3,8.9c-0.3-0.3-0.5-0.6-0.7-1c-0.2-0.4-5.3-9.6-12-13.4c-8.5-4.8-27-6-27.8-6.1v6.9c4.8,0.3,18.4,1.8,24.3,5.2  c4.1,2.4,8.2,8.6,9.4,10.7c2.7,4.4,7.1,7.5,12.2,8.5l20.5,3.1c2.2,0.5,3.8,2.3,4.2,4.5l1.7,16.5c0.2,1.3-0.7,2.6-2.1,2.7  c-0.1,0-0.3,0-0.4,0l-19.8,0.2c-2.8-9.4-12.6-14.8-22-12c-5.9,1.7-10.4,6.4-12.1,12.3l-15.9,0.1v6.9l15.4-0.1  c1.2,9.7,10,16.7,19.7,15.5c8.2-1,14.7-7.5,15.5-15.8l19.2-0.2c5.2,0,9.4-4.2,9.4-9.3C89.6,64.9,89.6,64.6,89.6,64.2L89.6,64.2z   M43.4,83.7c-6,0-10.8-4.8-10.8-10.8c0-6,4.8-10.8,10.8-10.8c6,0,10.8,4.8,10.8,10.8v0C54.2,78.8,49.3,83.7,43.4,83.7z'/></svg>
                    </a>
        </div>
        <div class='pbutton' id='jam_btn'>
          <a class='butt'>
            <svg fill='#ffffff' height='37px' width='37px' version='1.1' id='Layer_1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' viewBox='0 0 491.522 491.522' xml:space='preserve' stroke='#ffffff'>
              <g id='SVGRepo_bgCarrier' stroke-width='0'/>
              <g id='SVGRepo_tracerCarrier' stroke-linecap='round' stroke-linejoin='round'/>
              <g id='SVGRepo_iconCarrier'> <g> <g> <path d='M471.042,20.481h-327.68c-4.791,0-9.43,1.68-13.111,4.747l-122.88,102.4c-4.669,3.891-7.369,9.655-7.369,15.733V245.72 c0,0.014-0.002,0.027-0.002,0.041v204.8c0,11.311,9.169,20.48,20.48,20.48h450.56c11.311,0,20.48-9.169,20.48-20.48V245.802 c0-0.014,0.002-0.027,0.002-0.041v-122.88v-81.92C491.522,29.65,482.353,20.481,471.042,20.481z M450.562,225.281h-327.68v-31.433 l59.945-50.487h267.735V225.281z M40.962,152.953l109.815-91.512h299.785v40.96h-275.21c-4.828,0-9.5,1.706-13.193,4.815 l-72.95,61.44c-4.62,3.891-7.287,9.624-7.287,15.664v40.96h-40.96V152.953z M450.56,430.081H286.722v-52.957l5.998,5.998 c7.998,7.998,20.965,7.998,28.963,0c7.998-7.998,7.998-20.965,0-28.963l-40.959-40.959c-0.477-0.477-0.98-0.93-1.503-1.358 c-0.235-0.193-0.484-0.361-0.726-0.543c-0.293-0.219-0.579-0.446-0.884-0.65c-0.291-0.195-0.593-0.363-0.891-0.541 c-0.275-0.164-0.544-0.337-0.827-0.489c-0.305-0.163-0.618-0.301-0.929-0.447c-0.295-0.139-0.585-0.286-0.887-0.412 c-0.302-0.125-0.61-0.226-0.916-0.335c-0.324-0.117-0.644-0.241-0.975-0.341c-0.307-0.093-0.619-0.161-0.929-0.239 c-0.338-0.085-0.671-0.18-1.015-0.248c-0.358-0.071-0.72-0.113-1.081-0.165c-0.3-0.043-0.595-0.1-0.9-0.13 c-1.344-0.133-2.698-0.132-4.042,0c-0.304,0.03-0.599,0.087-0.898,0.13c-0.361,0.052-0.724,0.094-1.083,0.165 c-0.343,0.068-0.676,0.162-1.013,0.247c-0.31,0.078-0.623,0.146-0.93,0.239c-0.331,0.1-0.65,0.224-0.974,0.341 c-0.307,0.11-0.616,0.211-0.918,0.336c-0.302,0.125-0.592,0.272-0.886,0.411c-0.311,0.147-0.625,0.285-0.93,0.448 c-0.283,0.151-0.551,0.324-0.826,0.488c-0.299,0.178-0.601,0.347-0.892,0.542c-0.304,0.203-0.59,0.429-0.882,0.648 c-0.242,0.182-0.491,0.35-0.727,0.544c-0.522,0.428-1.025,0.88-1.502,1.357L210.8,354.159c-7.998,7.998-7.998,20.965,0,28.963 c7.998,7.998,20.965,7.998,28.963,0l5.998-5.998v52.957H40.96v-163.84h61.442H450.56V430.081z'/> </g> </g> </g>
              </svg>
          </a>
        </div>
      </div> <!--end .row2-->

</div>

<div id='alertContainer'>
<div id='isconnected' class='alertBox'>
    لايوجد اتصال
</div></div>

<div id='timer'>-</div>
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
  const formatTime=(seconds)=> {
    if (isNaN(seconds) || seconds < 0) {
      return '-';
    }
    const hours = Math.floor(seconds / 3600);
    const minutes = Math.floor((seconds % 3600) / 60);
    const remainingSeconds = seconds % 60;
  
    const formattedHours = hours < 10 ? `0${hours}` : `${hours}`;
    const formattedMinutes = minutes < 10 ? `0${minutes}` : `${minutes}`;
    const formattedSeconds = remainingSeconds < 10 ? `0${remainingSeconds}` : `${remainingSeconds}`;
  
    return `${formattedHours}:${formattedMinutes}:${formattedSeconds}`;
  }

setInterval(()=>{
        new Promise((resolve, reject) => {
            const xhr = new XMLHttpRequest();
            const url = 'http://2.2.2.2/state';
            xhr.onreadystatechange = async() => {
              if (xhr.readyState === 4) {
                if (xhr.status === 200) {
                  resolve(xhr.responseText);
                  document.getElementById('isconnected').style.display='none';

                  if(xhr.responseText.split('#')[0]=='on'){
                    document.getElementById('toggle1').checked = true;
                    document.getElementById('sign1').style.backgroundColor='red';
                  }
                  else{
                    document.getElementById('toggle1').checked = false;
                    document.getElementById('sign1').style.backgroundColor='#3c3838';
                    }
                  if(xhr.responseText.split('#')[1]=='on')
                    document.getElementById('sign3').style.backgroundColor='green';
                  else
                    document.getElementById('sign3').style.backgroundColor='#3c3838';
                  if(xhr.responseText.split('#')[2]=='on')
                    document.getElementById('run_sign').style.backgroundColor='green';
                  else
                     document.getElementById('run_sign').style.backgroundColor='#3c3838';
                     // set the timer
                  if(xhr.responseText.split('#')[3]!=undefined)
                    document.getElementById('timer').innerHTML=formatTime(parseInt(xhr.responseText.split('#')[5]));
                    //setTimer(formatTime(parseInt(xhr.responseText.split('#')[5])));
                  else
                    setTimer('-');
                  
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
  window.open('settings', '_self');
}

const CenterLock=(ev)=>{
    new Promise((resolve, reject) => {
        const xhr = new XMLHttpRequest();
        const url = ev;
    
        xhr.onreadystatechange = () => {
          if (xhr.readyState === 4) {
            if (xhr.status === 200) {
              resolve(xhr.responseText);
              document.getElementById('isconnected').style.display='none';
            } else {
              document.getElementById('isconnected').style.display='block'; // no connection
            }
          }
        };
    
        xhr.open('GET', url, true);
        xhr.timeout = 1000; // set the timeout to 2 seconds
        xhr.send();
      });
}

const switchOneOn=()=>{
  document.getElementById('image').src='image-G.png';
    new Promise((resolve, reject) => {
        const xhr = new XMLHttpRequest();
        const url = 'http://2.2.2.2/switch1On';
    
        xhr.onreadystatechange = () => {
          if (xhr.readyState === 4) {
            if (xhr.status === 200) {
              resolve(xhr.responseText);
              document.getElementById('isconnected').style.display='none';
            } else {
              document.getElementById('isconnected').style.display='block'; // no connection
            }
          }
        };
    
        xhr.open('GET', url, true);
        xhr.timeout = 1000; // set the timeout to 2 seconds
        xhr.send();
      });
}

const switchOneOff=()=>{
  document.getElementById('image').src='image-R.png';
    new Promise((resolve, reject) => {
        const xhr = new XMLHttpRequest();
        const url = 'http://2.2.2.2/switch1Off';
    
        xhr.onreadystatechange = () => {
          if (xhr.readyState === 4) {
            if (xhr.status === 200) {
              resolve(xhr.responseText);
              document.getElementById('isconnected').style.display='none';
            } else {
              document.getElementById('isconnected').style.display='block'; // no connection
            }
          }
        };
    
        xhr.open('GET', url, true);
        xhr.timeout = 1000; // set the timeout to 2 seconds
        xhr.send();
      });
}



const motorOn=()=>{
  new Promise((resolve, reject) => {
    const xhr = new XMLHttpRequest();
    const url = 'http://2.2.2.2/motorOn';

    xhr.onreadystatechange = () => {
      if (xhr.readyState === 4) {
        if (xhr.status === 200) {
          resolve(xhr.responseText);
          document.getElementById('isconnected').style.display='none'; 
        } else {
          document.getElementById('isconnected').style.display='block'; // no connection
        }
      }
    };

    xhr.open('GET', url, true);
    xhr.timeout = 400; 
    xhr.send();
  });
}
const motorOff=()=>{
  new Promise((resolve, reject) => {
    const xhr = new XMLHttpRequest();
    const url = 'http://2.2.2.2/motorOff';

    xhr.onreadystatechange = () => {
      if (xhr.readyState === 4) {
        if (xhr.status === 200) {
          resolve(xhr.responseText);
          document.getElementById('isconnected').style.display='none'; 
        } else {
          document.getElementById('isconnected').style.display='block'; // no connection
        }
      }
    };

    xhr.open('GET', url, true);
    xhr.timeout = 400; 
    xhr.send();
  });
}

const switch1=document.getElementById('toggle1');
switch1.addEventListener('change',()=>{
    if(switch1.checked) switchOneOn();
    else switchOneOff();
})



const lock=document.getElementById('lock_btn');
lock.addEventListener('click',()=>{CenterLock('lock')});

const open=document.getElementById('open_btn');
open.addEventListener('click',()=>{CenterLock('open')});

const trunk=document.getElementById('trunk_btn');
trunk.addEventListener('click',()=>{CenterLock('trunk')});

const glass=document.getElementById('jam_btn');
glass.addEventListener('touchstart', ()=>{CenterLock('glassUP')});
glass.addEventListener('touchend', ()=>{CenterLock('glassDown')});


const runButton=document.getElementById('run_btn');
runButton.addEventListener('touchstart', motorOn);
runButton.addEventListener('touchend', motorOff);

document.getElementById('image').addEventListener('contextmenu', function(e) {
    e.preventDefault();
  });
document.getElementById('lock_btn').addEventListener('contextmenu', function(e) {
  e.preventDefault();
});
document.getElementById('open_btn').addEventListener('contextmenu', function(e) {
  e.preventDefault();
});
document.getElementById('jam_btn').addEventListener('contextmenu', function(e) {
  e.preventDefault();
});
document.getElementById('trunk_btn').addEventListener('contextmenu', function(e) {
  e.preventDefault();
});
    </script>
</html>
)rawliteral";

#endif // Index_H
