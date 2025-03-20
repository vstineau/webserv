<?php
    session_start();

    if (isset($_POST["password"]) && hash("md5", $_POST["password"]) == "1102438e142e17a78b8dc1d7d1919188")
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