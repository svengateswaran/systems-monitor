<html>
<head>
<title>Systems Monitor</title>
<link rel="stylesheet" href="style.css">
<link rel="icon" href="images/systems-monitor-logo-32x32.png" type="image/png" sizes="32x32">
<script src="jquery.min.js"></script>
<script>
advanced = false;

function get_data() {

   var data_url = "get_data_basic.php";
   if(advanced)
     data_url = "get_data.php";

   $.ajax({url: data_url, success: function(result){
       $("#systems-data").html(result);
   }});
}
$(document).ready(function(){
   setInterval(get_data, 500);
    get_data();
});

</script>
</head>
<body>
<div id="systems-container">
<div id="header">
<img src="images/systems-monitor-logo.png" id="logo">
<div id="header-content">
Systems Monitor
</div>
<div id="options"> Advanced </div>
</div>
<div class="space-filler"></div>

<div id="systems-data">
</div>
</div>

<script>

$( "#options" ).click(function() {
  if(advanced) {
    advanced = false;
    $(this).text("Advanced");
  } else {
    advanced = true;
    $(this).text("Basic");
  }
  get_data();
});

</script>
</body>
</html>
