<?php

function read_data($data_file) {
  if(!file_exists($data_file))
    return -1;
  $data_fp = fopen($data_file, "r") or die("Unable to open file!");
  $data = fread($data_fp, filesize($data_file));
  fclose($data_fp);
  return $data;
}
function print_data($title, $data_file, $suffix) {
  if(file_exists($data_file)) {
    $data = read_data($data_file);
    echo "<tr>";
    echo "<td>" . $title . "</td>";
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
                  array("CPU Load", "cpu_util", "%"),
                  array("GPU Count", "gpu_count", "")
                 );
$data_gpu_col = array( 
                      array("GPU Name", "gpu_name", ""),
                      array("GPU Cores Util", "gpu_cores_util", "%"),
                      array("GPU Memory Util", "gpu_mem_util", "%"),
                      array("GPU Total Memory", "gpu_mem_total", "GB"),
                      array("GPU Used Memory", "gpu_mem_used", "GB"),
                      array("GPU Temperature", "gpu_temperature", "C")
                      ); 
$data_dir = "/home/mcw/cls-mng/systems-monitor/site/data";
$dir_itr = new DirectoryIterator($data_dir);
foreach ($dir_itr as $fileinfo) {
  if (!$fileinfo->isDot()) {
       $sys_name =  $fileinfo->getFilename();
       echo "<div class='sys-block'>";
       echo "<div class='sys-header'>";
       echo $sys_name;
       echo "</div>";
       echo "<table class='data-table'>";
       echo "<tr>";
       $sys_dir = $data_dir . "/" . $sys_name; 
       for($i = 0; $i < count($data_col); $i++){
         $data_file = $sys_dir . "/" . $data_col[$i][1];
         if($data_col[$i][1] != "gpu_count") {
           print_data($data_col[$i][0], $data_file, $data_col[$i][2]);
         } else {
           $gpu_count = read_data($data_file);
           if($gpu_count > 0) {
             for($gi = 0; $gi < $gpu_count; $gi++){
               for($gdi = 0; $gdi < count($data_gpu_col); $gdi++){
                 $data_file = $sys_dir . "/" . $data_gpu_col[$gdi][1] . "_" . $gi;
                 print_data("G$gi - " . $data_gpu_col[$gdi][0], $data_file, $data_gpu_col[$gdi][2]);
               }
             }
           }
         }
       }
       echo "</table>";
       echo "</div>";

  }
}
?>
