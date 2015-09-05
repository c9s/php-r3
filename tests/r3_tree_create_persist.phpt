--TEST--
Check r3_tree_create_persist
--FILE--
<?php
$res = r3_tree_create_persist("app", 10);
echo $res ? true : false;
--EXPECT--
1
