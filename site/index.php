<html>
<head>
<title>Systems Monitor</title>
<link rel="stylesheet" href="style.css">
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
<script>
function get_data() {
   $.ajax({url: "get_data.php", success: function(result){
       $("#systems-data").html(result);
   }});
}
$(document).ready(function(){
   setInterval(get_data, 10)
});
</script>
</head>
<body>
<div id="systems-container">
<div id="header">
Remote Systems Monitor
</div>
<div id="systems-data">
</div>
</div>
</body>
</html>
