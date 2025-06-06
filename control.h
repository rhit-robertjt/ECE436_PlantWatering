const char control[] = " \ 
<!DOCTYPE html> \ 
<html lang=\"en\"> \ 
<head> \ 
<title>Plant Monitor Control Panel</title> \ 
<meta charset=\"utf-8\" name=\"viewport\" content=\"width=device-width\">\ 
<meta name=\"description\" content=\"A website that controls an automated plant watering project\"> \ 
<style> \ 
:root { \ 
--main-bg-color: #2e7d32; \ 
--second-bg-color: lightgrey; \ 
--main-color: white; \ 
--second-color: blue; \ 
--link-color: #0BEAA0;} \ 
body { \ 
background-color: var(--second-bg-color); \ 
display: flex; \ 
flex-direction: column; \ 
margin: 0; \ 
min-height: 100vh; \ 
color: var(--main-color);}\ 
.headNav { \ 
background-color: var(--main-bg-color); \ 
overflow: hidden;min-height: 50px;} \ 
.headNav a { \ 
float: left; \ 
color: var(--main-color); \ 
text-align: center; \ 
padding: 17px; \ 
text-decoration: none; \ 
font-size: 17px;}\ 
.headNav a:hover, .footNav a:hover {\ 
background-color: var(--main-color);} \ 
.headNav a.active { \ 
color: var(--main-bg-color); \ 
background-color: var(--main-color);} \ 
a:visited, a:hover { \ 
color: var(--second-color);} \ 
a { \ 
color: var(--link-color);} \ 
.right-side { \ 
float: right;} \ 
/* Start main content */ \ 
* { \ 
box-sizing: border-box; \ 
margin: 0; \ 
padding: 0; \ 
font-family: 'Segoe UI', sans-serif;}\ 
main { \ 
background-image: url('https://images.newscientist.com/wp-content/uploads/2023/11/03102701/SEI_178383054.jpg'); \ 
background-size: cover; \ 
background-position: center; \ 
flex-grow: 1; \ 
display: flex; \ 
align-items: center; \ 
justify-content: center;} \ 
.panel { \ 
background-color: rgba(255, 255, 255, 0.92); \ 
padding: 40px; \ 
border-radius: 16px; \ 
box-shadow: 0 10px 25px rgba(0, 0, 0, 0.3); \ 
width: 100%; \ 
max-width: 500px; \ 
text-align: center;}\ 
.panel h2 { \ 
color: #2e7d32; \ 
margin-bottom: 24px;} \ 
.controls { \ 
display: flex; \ 
flex-direction: column; \ 
gap: 20px;} \ 
.buttons { \ 
display: flex; \ 
gap: 15px; \ 
justify-content: center; \ 
flex-wrap: wrap;}\ 
.btn { \ 
padding: 10px 20px; \ 
border: none; \ 
background-color: #43a047; \ 
color: white; \ 
font-size: 1rem; \ 
border-radius: 30px; \ 
cursor: pointer; \ 
transition: background-color 0.3s ease;}\ 
.btn:hover { \ 
background-color: #2e7d32;} \ 
.dropdowns { \ 
display: flex; \ 
gap: 15px; \ 
justify-content: center; \ 
flex-wrap: wrap;} \ 
select { \ 
padding: 10px; \ 
border-radius: 10px; \ 
border: 2px solid #a5d6a7; \ 
background-color: #f5f5f5; \ 
font-size: 1rem;color: #2e7d32;} \ 
</style>\ 
<script> \ 
window.addEventListener(\"DOMContentLoaded\", () => { \ 
let buttons = document.querySelectorAll(\"button\");\ 
buttons[0].addEventListener(\"click\", () => { \ 
if (buttons[0].textContent == \"Disable Manual Watering\") { \ 
buttons[0].textContent = \"Enable Manual Watering\"; \ 
fetch(\"/manual\", {method: \"POST\",'Content-Type': 'text/plain', \ 
body: \"off\",headers: {\"Accept\": \"html\"}}); \ 
} else { \ 
buttons[0].textContent = \"Disable Manual Watering\"; \ 
fetch(\"/manual\", {method: \"POST\",'Content-Type': 'text/plain', \ 
headers: {\"Accept\": \"html\"}, body: \"on\",}); \ 
} \ 
});\ 
buttons[1].addEventListener(\"click\", () => { \ 
if (buttons[1].textContent == \"Disable Circulation Pump\") { \ 
buttons[1].textContent = \"Enable Circulation Pump\"; \ 
fetch(\"/circ\", {method: \"POST\",'Content-Type': 'text/plain', \ 
body: \"off\", \ 
headers: {\"Accept\": \"html\"}}); \ 
} else { \ 
buttons[1].textContent = \"Disable Circulation Pump\"; \ 
fetch(\"/circ\", {method: \"POST\",'Content-Type': 'text/plain', \ 
headers: {\"Accept\": \"html\"}, body: \"on\",}); \ 
} \ 
});\ 
buttons[2].addEventListener(\"click\", () => { \ 
let onTime = prompt(\"How long (in seconds) should the plant be watered for?\"); \ 
if(onTime == null || isNaN(onTime)) {return;} \ 
let offTime = prompt(\"How long (in seconds) should the should the system wait in between waterings?\"); \ 
console.log(`${onTime} ${offTime}`); \ 
if(offTime == null || isNaN(offTime)) {return;} \ 
fetch('/setWaterings',{method:'POST','Content-Type':'text/plain', \ 
headers:{'Accept': 'html'},body:`30 50`,}); \ 
});\ 
buttons[3].addEventListener(\"click\", () => { \ 
fetch(\"/reset\", {method: \"POST\",'Content-Type': 'text/plain', \ 
body: `Welcome to the Matrix...`,headers: {\"Accept\": \"html\"}}); \ 
});\ 
let dropdowns = document.querySelectorAll(\"select\"); \ 
dropdowns[0].addEventListener(\"change\", () => { \ 
fetch(\"/plant_type\", {method: \"POST\",'Content-Type': 'text/plain', \ 
body: dropdowns[0].value,headers: {\"Accept\": \"html\"} \ 
}); \ 
});\ 
dropdowns[1].addEventListener(\"change\", () => { \ 
fetch(\"/modes\", {method: \"POST\",'Content-Type': 'text/plain', \ 
body: dropdowns[1].value,headers: {\"Accept\": \"html\"} \ 
}); \ 
}); \ 
}); \ 
</script>\ 
</head> \ 
<body> \ 
<header class=\"headNav\"> \ 
<a href=\"./webpage.html\">Main Page</a> \ 
<a class=\"active\" href=\"./control.html\">Control</a> \ 
<div class=\"right-side\"> \ 
<a href=\"./login.html\">Login</a> \ 
</div> \ 
</header>\ 
<main> \ 
<div class=\"panel\"> \ 
<h2>Plant Automation Control Panel</h2> \ 
<div class=\"controls\"> \ 
<div class=\"buttons\"> \ 
<button class=\"btn\">Enable Manual Watering</button> \ 
<button class=\"btn\">Disable Circulation Pump</button> \ 
<button class=\"btn\">Change Watering Timing</button> \ 
</div>\ 
<div class=\"dropdowns\"> \ 
<select><option disabled selected>Select Plant Type</option> \ 
<option>Succulent</option> \ 
<option>Cacti</option> \ 
<option>General</option> \ 
</select>\ 
<select> \ 
<option disabled selected>Select Mode Preset</option> \ 
<option>Manual</option> \ 
<option>Quiet</option> \ 
<option>Auto</option> \ 
</select> \ 
</div> \ 
<button class=\"btn\">Reset to Default</button> \ 
</div> \ 
</div> \ 
</main> \ 
</body> \ 
</html>";