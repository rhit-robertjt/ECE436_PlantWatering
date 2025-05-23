const char webpage[] = "<!DOCTYPE html> \
<html lang=\"en-US\">  \
<head> \
	<title> Plant Monitor Main </title> \
	<meta charset=\"utf-8\"name=\"viewport\"content=\"width=device-width\"> \
	<meta name=\"description\"content=\"A website that controls an automated plant watering project\">  \
	<style> \
	:root { \
			--main-bg-color:#2e7d32; \
			--second-bg-color:lightgrey; \
			--main-color:white; \
			--second-color:blue; \
			--link-color:# 0B EAA0; \
		} \
	*{ \
		box-sizing:border-box; \
		margin:0; \
		padding:0; \
		font-family:'SegoeUI', sans-serif; \
	} \
	body { \
		background-color:var(--second-bg-color); \
		display:flex; \
		flex-direction:column; \
		margin:0; \
		min-height:100vh; \
		color:var(--main-color); \
	} \
	.headNav { \
		background-color:var(--main-bg-color); \
		overflow:hidden; \
		min-height:50px; \
	} \
	.headNav a { \
		float:left; \
		color:var(--main-color); \
		text-align:center; \
		padding:17px; \
		text-decoration:none; \
		font-size:17px; \
	} \
	.headNav a:hover, .footNav a:hover { \
		background-color:var(--main-color); \
	}.headNav a.active { \
		color:var(--main-bg-color); \
		background-color:var(--main-color); \
	} \
	a:visited, a:hover { \
		color:var(--second-color); \
	} \
	a { \
		color:var(--link-color); \
	} \
	.right-side { \
		float:right; \
	} \
	/* Start main content */ \
	main { \
		background-image:url('https://as1.ftcdn.net/jpg/03/14/52/38/1000_F_314523891_empfkO7kiQyPtYFBorNEynQdZM4M6OOb.jpg'); \
		background-size:cover; \
		background-position:center; \
		background-repeat:no-repeat; \
		flex-grow:1; \
		justify-content:center; \
		align-items:center; \
		display:flex; \
	} \
	.overlay { \
		background-color:rgba(255, 255, 255, 0.75); \
		padding:40px; \
		border-radius:20px; \
		box-shadow:0 10px 30px rgba(0, 0, 0, 0.3); \
		text-align:center; \
		align-items:center; \
		min-width:50%; \
		max-width:80%; \
	} \
	h1 { \
		font-size:3 rem; \
		color:#2e7d32;margin-bottom:20px; \
	} \
	h2{ \
		font-size:1.5rem; \
		color:#2e7d32; \
		margin-bottom:20px; \
	} \
	p { \
		font-size:1 rem; \
		color:#27732a; \
		margin-bottom:30px; \
	} \
	</style> \
<script> \
	window.addEventListener(\"DOMContentLoaded\", async () => { \
        let response = await fetch(\"/variables\", {headers:{\"Accept\":\"html\"}}); \
        let text = await response.text();text = text.split(\",\"); \
        let paragraphs = document.querySelectorAll(\"main div p\"); \
        for (let i = 0; i <5; i++) { \
            paragraphs[i].textContent = paragraphs[i].textContent.replace(\"%s\", text[i]) \
        } \
    })  \
	</script>  \
</head> \
<body> \
	<header class=\"headNav\"> \
		<a class=\"active\"href=\"./webpage.html\">MainPage</a> \
		<a href=\"./control.html\">Control</a> \
		<div class=\"right-side\"><a href=\"./login.html\">Login</a></div> \
	</header> \
	<main> \
		<div class = \"overlay\"> \
			<h1> Welcome to Plant Monitor!</h1> \
			<h2>Here is the current state of the machine:</h2> \
			<p> Time last watered: %s</p> \
			<p>Next watering time: %s</p> \
			<p> Humidity: %s% </p> \
			<p>Soil Moisture:%s%</p> \
			<p> Water Level: %s% </p> \
		</div> \
	</main> \
</body> \
</html>";