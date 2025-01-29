<!DOCTYPE html>
<html lang="PL-pl">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Weather</title>
    <link rel="stylesheet" href="lato.css">
</head>
<body>
<?php 
 include('config.php');
 $Dbconnect = mysqli_connect($db_host, $db_user, $db_pass, $db_name);
 
 $table = mysqli_query($Dbconnect, "SELECT * FROM `S_01`");

 while ( $row = mysqli_fetch_row($table )){

        $temperature = $row[2];
        $pressure = $row[5];
        $light =  $row[6];
}

 mysqli_close($Dbconnect) ;
 ?>

    <header>
        <h1><i>Witamy w ckz nr.1 w Dębicy!!</i></h1>
    </header>
    <section>
        <div class="container">
            <!-- <div class="image-column">
                <div class="image-container" id="imageContainer">
                    <img src="img/ladne.jpg" alt="Zdj ^ycie CKZ">
                </div>
            </div> -->
            <div class="data-column">
                <div class="weather-data">
                    <div class="data-item">
                        <img src="ico/temp.png" alt="Ikona temperatury">
                        <p>Temperatura: <span id="temperature"><?php echo $temperature ?></span>   ^C</p>
                    </div>
                    <div class="data-item">
                        <img src="ico/cisc.png" alt="Ikona ciśnienia">
                        <p>Ciśnienie: <span id="pressure"><?php echo $pressure?></span> hPa</p>
                    </div>
                    <div class="data-item">
                        <img src="ico/slon.png" alt="Ikona nas Nasłonecznienia">
                        <p>Nasłonecznienie: <span id="sunshine"><?php echo $light ?></span> lux</p>
                    </div>
                </div>
            </div>
        </div>
    </section>    
    
    <script async defer>
            // Funkcja do odświeżania strony
        function odswiezStrone() {
            location.reload();
        }
            // Uruchomienie funkcji odświeżania co minutę (60 000 milisekund)
        setInterval(odswiezStrone, 60000);
    </script>
</body>
</html>