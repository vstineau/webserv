<?php
    session_start();
    session_unset();

    header("Location: /");
?>

<p>Logout successful! Click <a href="index.php">here</a> to return to the index</p>