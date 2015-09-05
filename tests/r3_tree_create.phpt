--TEST--
Check r3_tree_create
--FILE--
<?php
$tree = r3_tree_create(10);
echo $tree ? true : false;
--EXPECT--
1
