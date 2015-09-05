--TEST--
Check r3_tree_create_persist
--FILE--
<?php
$tree = r3_tree_create_persist(10, "app");
echo $tree ? "ok\n" : "fail\n";
$ret = r3_tree_delete_persist("app");
echo $tree ? "ok\n" : "fail\n";
--EXPECT--
ok
ok
