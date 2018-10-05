<html>
<head>
<title>Systems Monitor</title>
<link rel="stylesheet" href="style.css">
<link rel="icon" href="images/systems-monitor-logo-32x32.png" type="image/png" sizes="32x32">
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
<script>
function get_data() {
   $.ajax({url: "get_data.php", success: function(result){
       $("#systems-data").html(result);
   }});
}
$(document).ready(function(){
   setInterval(get_data, 500);
   // get_data();
});
</script>
</head>
<body>
<div id="systems-container">
<div id="header">
<!---<img src="images/systems-monitor-logo.png" height=50 width=50> --->
<div id="header-content">
Systems Monitor
</div>
</div>
<div class="space-filler"></div>
<div id="systems-data">
</div>
</div>
</body>
</html>
