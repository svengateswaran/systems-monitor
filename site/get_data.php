<?php
$data_col = array(
                  array("CPU Load", "cpu_util", 1),
                  array("GPU Count", "gpu_count", 0)
                 );
$data_gpu_col = array( 
                      array("GPU Cores Util", "gpu_cores_util", 1),
                      array("GPU Memory Util", "gpu_mem_util", 1),
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

       for($i = 0; $i < count($data_col); $i++){

         $data_file= $data_dir . "/" . $sys_name . "/" . $data_col[$i][1];
         if(file_exists($data_file)) {
           $data_fp = fopen($data_file, "r") or die("Unable to open file!");
           $data = fread($data_fp, filesize($data_file));
           fclose($data_fp);

           echo "<tr>";
           echo "<td>" . $data_col[$i][0]   . "</td>";
           if($data_col[$i][2])
             echo "<td> <div class='progress-bar' style='width:" . (int)$data . "%'>" . $data . "%</div></td>";
           else
             echo "<td>" . $data . "</td>";

           echo "</tr>";
         }
       }
       echo "</table>";
       echo "</div>";

  }
}
?>
