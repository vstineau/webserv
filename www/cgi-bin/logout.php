<?php
    session_start();
    session_unset();

    header("Location: /");
?>

<p>Logout successful! Click <a href="/">here</a> to return to the index</p>