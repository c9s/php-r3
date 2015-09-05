--TEST--
Store r3 tree in persistent global
--FILE--
<?php
$tree = r3_tree_create(10);
echo $tree ? "ok\n" : "fail\n";
$ok = r3_tree_store("app", $tree);
echo $ok ? "ok\n" : "fail\n";
--EXPECT--
ok
ok
