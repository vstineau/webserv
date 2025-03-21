<?php
    session_start();
    
    $raw_post_data = file_get_contents("php://input");
// Afficher le contenu brut du corps de la requête
    echo "<pre>";
    var_dump($raw_post_data);
    echo "</pre>";
    if (isset($_POST["password"]) && $_POST["password"] == "qwerty")
    {
        $_SESSION["username"] = $_POST["username"];
        header("Location: /");
    } else
    {
?>
        <div>
            Wrong password!
        </div>
<?php
    }
?>