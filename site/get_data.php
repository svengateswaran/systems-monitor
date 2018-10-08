<?php

function read_data($data_file) {
  if(!file_exists($data_file))
    return -1;
  $data_fp = fopen($data_file, "r") or die("Unable to open file!");
  $data = fread($data_fp, filesize($data_file));
  fclose($data_fp);
  return $data;
}
function print_data($title, $data_file, $suffix, $util, $rowspan) {
  if(file_exists($data_file)) {
    $data = read_data($data_file);
    $util_style = "";
    if($util < 0 or $util > 100)
       $util = 0;

    if(!$util)
      $util_style = "style='background:#aaa'";

    if($suffix == "%" or $suffix == "C") {
      if($data < 0 or $data > 100)
         $data = 0;
      echo "<td rowspan=$rowspan $util_style> <div class='progress-bar' style='width:" . (int)$data . "%'>" . $data . "$suffix</div></td>";
    } else {
      echo "<td rowspan=$rowspan $util_style> $data $suffix </td>";
    }
    return 0;
 }
 return -1;
}

$data_col = array(
                  array("CPU Load", "cpu_util", "%"),
                  array("GPU Count", "gpu_count", "")
                 );
$data_gpu_col = array( 
                      array("GPU Name", "gpu_name", ""),
                      array("GPU Cores Util", "gpu_cores_util", "%"),
                      array("GPU Memory Util", "gpu_mem_util", "%"),
                      array("GPU Used Memory", "gpu_mem_used", "GB"),
                      array("GPU Total Memory", "gpu_mem_total", "GB"),
                      array("GPU Temperature", "gpu_temperature", "C")
                      ); 

$data_dir =  "./data";

$dir_itr = new DirectoryIterator($data_dir);
echo "<table class='data-table scroll'>";
echo "<thead>";
echo "<tr id='table-header-row'>";
echo "<th> Machine </th>";
for($i = 0; $i < count($data_col); $i++){
  if($data_col[$i][1] != "gpu_count") {
    echo "<th>" . $data_col[$i][0] . "</th>";
  } else {
    for($gdi = 0; $gdi < count($data_gpu_col); $gdi++){
      echo "<th>" . $data_gpu_col[$gdi][0] . "</th>";
    }
  }
}
echo "</tr>";

echo "</thead>";
echo "<tbody>";
$row_count = 0;
$row_colors = array("#fff", "#fff");

foreach ($dir_itr as $fileinfo) {
  if (!$fileinfo->isDot()) {
       $sys_name =  $fileinfo->getFilename();
       $sys_dir = $data_dir . "/" . $sys_name; 
       echo "<tr class='system-row' style='background:" . $row_colors[$row_count % 2]. "'>";
       $gpu_count = (int)read_data("$sys_dir/gpu_count");
       if($gpu_count > 16)
         $gpu_count = 0;

       $row_span = $gpu_count > 1 ? $gpu_count : 1;
       echo "<td rowspan=" . $row_span ." class='sys-name'>";
       //echo "<td rowspan=1>";
       echo $sys_name;
       echo "</td>";
       for($i = 0; $i < count($data_col); $i++){
         $data_file = $sys_dir . "/" . $data_col[$i][1];
         if($data_col[$i][1] != "gpu_count") {
           print_data($data_col[$i][0], $data_file, $data_col[$i][2], 1, $row_span);
         } else {
           if($gpu_count > 0) {
             for($gi = 0; $gi < $gpu_count; $gi++){
               if($gi) echo "</tr> <tr class='system-row'>";
               $gpu_util = read_data($sys_dir . "/gpu_cores_util_$gi");
               for($gdi = 0; $gdi < count($data_gpu_col); $gdi++){
                 $data_file = $sys_dir . "/" . $data_gpu_col[$gdi][1] . "_" . $gi;
                 print_data("GPU $gi - " . $data_gpu_col[$gdi][0], $data_file, $data_gpu_col[$gdi][2], $gpu_util, 1);
               }
               if($gi) echo "</tr>";               
             }
           }
         }
       }
       echo "</tr>";
       $row_count = $row_count + 1;
  }
}
echo "</tbody>";
echo "</table>";
?>
