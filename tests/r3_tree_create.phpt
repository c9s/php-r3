--TEST--
Check r3_tree_create
--FILE--
<?php
$tree = r3_tree_create_persist(10, "app");
echo $tree ? true : false;
--EXPECT--
1
