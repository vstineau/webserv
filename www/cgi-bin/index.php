<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ph7</title>
</head>
<body>
<?php
    session_start();

    if (!isset($_SESSION["username"]))
    {
?>
    <div class="login-form">
        <form action="/login.php" method="POST">
            <div>
                <input type="text" name="username" placeholder="Username" autocomplete="off" />
            </div>
            <div>
                <input type="password" name="password" placeholder="Password" autocomplete="off" />
            </div>
            <input type="submit" name="submit" value="Login" />
        </form>
    </div>
<?php
    } else
    {
?>
        <p>Welcome <?= $_SESSION["username"] ?> <a href="/logout.php">Logout</a></p>
<?php
    }
?>
</body>
</html>
