--TEST--
Check r3_tree_compile
--FILE--
<?php
$tree = r3_tree_create_persist("app", 10);
echo $tree ? "ok\n" : "fail\n";

$ret = r3_tree_insert($tree, "/foo/bar", ["simplecallback"]);
echo $ret ? "ok\n" : "fail\n";

$ret = r3_tree_insert($tree, "/foo/zoo", ["simplecallback"]);
echo $ret ? "ok\n" : "fail\n";

$ret = r3_tree_insert($tree, "/hack/zzz", ["simplecallback"]);
echo $ret ? "ok\n" : "fail\n";

$ret = r3_tree_compile($tree);
echo $ret ? "ok\n" : "fail\n";

$data = r3_tree_match($tree, "/foo/bar");
// var_dump($data);

$ret = r3_tree_delete_persist("app");
echo $ret ? "ok\n" : "fail\n";
--EXPECT--
ok
ok
ok
ok
ok
ok
