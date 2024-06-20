<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>

    <script src="p5.min.js"></script>

</head>
<body>

    <?php
        $tex = @$_GET['tex'];
        if ($tex && file_exists($tex)) {
            echo '<script src="decode_tex_files.js"></script>';
        }

        $files = [];
        $paths = glob('./game/Graph/{*.tex,**/*.tex,*.tex2,**/*.tex2}', GLOB_BRACE);
        foreach ($paths as $path) {
            $inf = pathinfo($path);
            $inf += [
                'size' => floor( filesize($path)/1024 ) . ' KB',
                'path' => $path
            ];
            $files[] = $inf;
        }
    ?>

    <div style="width:25%;float:right;">

        <h4>Tex files:</h4>

        <ul>
            <?php foreach($files as $it): ?>
                <li>
                    <a href="?tex=<?= $it['path'] ?>">
                        <?= $it['path'] ?> (<?= $it['size'] ?>)
                    </a>
                </li>
            <?php endforeach; ?>
        </ul>

    </div>

</body>
</html>