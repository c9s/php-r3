--TEST--
Check r3_tree_create_persist
--FILE--
<?php
$res = r3_tree_create_persist(10, "app");
echo $res ? true : false;
--EXPECT--
1
