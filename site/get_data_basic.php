<?php

function read_data($data_file) {
  if(!file_exists($data_file))
    return -1;
  $data_fp = fopen($data_file, "r") or die("Unable to open file!");
  $data = fread($data_fp, filesize($data_file));
  fclose($data_fp);
  return $data;
}
function print_data($title, $data_file, $suffix, $util) {
  if(file_exists($data_file)) {
    $data = read_data($data_file);
    $util_style = "";
    if(!$util)
      $util_style = "style='background:#aaa'";

    echo "<tr $util_style>";
    echo "<td class='dashboard-td'>" . $title . "</td>";

    if($suffix == "%") {
      if($data < 0 or $data > 100)
         $data = 0;
      echo "<td> <div class='progress-bar' style='width:" . (int)$data . "%'>" . $data . "%</div></td>";
    } else {
      echo "<td> $data $suffix </td>";
    }
    echo "</tr>";
    return 0;
 }
 return -1;
}

$data_col = array(
                  array("CPU", "cpu_util", "%"),
                  array("GPU Count", "gpu_count", ""),
                  array("Disk", "disk_util", "%")
                 );
$data_gpu_col = array( 
                      array("", "gpu_cores_util", "%"),
                      ); 
$data_dir = "/home/mcw/cls-mng/systems-monitor/site/data";
$dir_itr = new DirectoryIterator($data_dir);
$total_gpu_count = 0;
$used_gpu_count = 0;
echo "<div class='dash-space-filler'> </div>";

foreach ($dir_itr as $fileinfo) {
  if (!$fileinfo->isDot()) {
       $sys_name =  $fileinfo->getFilename();
       $sys_dir = $data_dir . "/" . $sys_name; 
       $gpu_count = read_data($sys_dir . "/gpu_count");
       if($gpu_count > 16)
         $gpu_count = 0;
       echo "<div class='sys-block'>";
       echo "<div class='sys-header'>";
       echo $sys_name;
       echo "</div>";
       echo "<table class='data-table dashboard-table'>";
       for($i = 0; $i < count($data_col); $i++){
         $data_file = $sys_dir . "/" . $data_col[$i][1];
         if($data_col[$i][1] != "gpu_count") {
           print_data($data_col[$i][0], $data_file, $data_col[$i][2], 1);
         } else {
           if($gpu_count > 0) {
             for($gi = 0; $gi < $gpu_count; $gi++){
               $gpu_util = read_data($sys_dir . "/gpu_cores_util_$gi");
               $gpu_name = read_data($sys_dir . "/gpu_name_$gi");
               if($gpu_util < 0 or $gpu_util > 100)
                 $gpu_util = 0;
               $total_gpu_count = $total_gpu_count + 1;
               if($gpu_util > 0)
                 $used_gpu_count = $used_gpu_count + 1;
               echo "<tr style='background:#ccc;'>";
               for($gdi = 0; $gdi < count($data_gpu_col); $gdi++){
                 $data_file = $sys_dir . "/" . $data_gpu_col[$gdi][1] . "_" . $gi;
                 print_data($gpu_name , $data_file, $data_gpu_col[$gdi][2], $gpu_util);
               }
               echo "</tr>";
             }
           }
         }
       }
       echo "</table>";
       echo "</div>";

  }
}
echo "<div id='dashboard'>";
echo "<table>";
$total_gpu_util_percent = (int)(((float)$used_gpu_count / $total_gpu_count ) * 100);
echo "<tr><td rowspan=2> GPUs Usage Status (#GPUs) </td> <td> <div class='progress-bar' style='width:$total_gpu_util_percent%'> $total_gpu_util_percent%</div></td></tr>";
echo "<tr><td> $used_gpu_count / $total_gpu_count </td></tr>";
echo "</table>";
echo "</div>";
?>
