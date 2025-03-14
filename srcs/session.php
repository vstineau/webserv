<?php
		//https://www.pierre-giraud.com/php-mysql-apprendre-coder-cours/session-definition-utilisation/
    //On démarre une nouvelle session
    session_start();
    
    //On définit des variables de session
    $_SESSION['prenom'] = 'Pierre';
    $_SESSION['age'] = 29;
?>
<!DOCTYPE html>
<html>
    <head>
        <title>Cours PHP & MySQL</title>
        <meta charset="utf-8">
        <link rel="stylesheet" href="cours.css">
    </head>
    
    <body>
        <h1>Titre principal</h1>
        <?php
            /*Si la variable de session age est définie, on echo sa valeur
             *puis on la détruit avec unset()*/
            if(isset($_SESSION['age'])){
                echo 'Tu as ' .$_SESSION['age']. ' ans<br>';
                unset($_SESSION['age']);
            }
            
            /*On détruit les données de session*/
            session_destroy();
            
            //On tente d'afficher les valeurs des variables age et prenom 
            echo 'Contenu de $_SESSION[\'age\'] : ' .$_SESSION['age']. '<br>';
            echo 'Contenu de $_SESSION[\'prenom\'] : ' .$_SESSION['prenom'];
        ?>
        <p>Un paragraphe</p>
    </body>
</html>
