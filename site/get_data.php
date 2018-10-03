<?php
$data_col = Array("CPU Load");
$data_dir = "/home/mcw/cls-mng/systems-monitor/site/data";
$dir_itr = new DirectoryIterator($data_dir);
foreach ($dir_itr as $fileinfo) {
  if (!$fileinfo->isDot()) {
     $sys_name =  $fileinfo->getFilename();
     $cpuload_file= $data_dir . "/" . $sys_name . "/cpu_load";
     if(file_exists($cpuload_file)) {
       $cpuload_fp = fopen($cpuload_file, "r") or die("Unable to open file!");
       $cpuload = fread($cpuload_fp, filesize($cpuload_file));
       fclose($data_fp);
       echo "<div class='sys-block'>";
       echo "<div class='sys-header'>";
       echo $sys_name;
       echo "</div>";
       echo "<table class='data-table'>";
       echo "<tr>";

       for($i = 0; $i < count($data_col); $i++){
               echo "<tr>";
               echo "<td>" . $data_col[$i]   . "</td>";
               echo "<td> <div class='progress-bar' style='width:" . (int)$cpuload . "%'>" . $cpuload. "%</div></td>";
               echo "</tr>";
       }
       echo "</table>";
       echo "</div>";
     }
  }
}
?>
